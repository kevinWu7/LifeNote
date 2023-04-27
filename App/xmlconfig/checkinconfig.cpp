#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include "checkinconfig.h"
#include "util.h"
#include "logger.h"

CheckinConfig* CheckinConfig::instance = nullptr;
std::once_flag CheckinConfig::onceFlag;

CheckinConfig::CheckinConfig() {}

CheckinConfig& CheckinConfig::getInstance()
{
    std::call_once(onceFlag, []() {
        instance = new CheckinConfig();
    });
    return *instance;
}


void CheckinConfig::updateDetailXml(CheckinType action,checkin_dateitem *item)
{
    QString filePath =STORAGE_PATH+ CHECKIN_CONFIG_PATH;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXml:open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateXml:set doc content form file failed"));
        file.close();
        return;
    }
    file.close();


    QDomNode parentDomElement=util::selectSingleNode(NODENAME_DETAILPROJECT,&doc);
    QDomNodeList childNodes = parentDomElement.childNodes(); // 获取子节点列表
    for (int i = 0; i < childNodes.size(); i++)
    {
        QDomNode childNode = childNodes.item(i); // 获取当前子节点

        QDomElement childElement = childNode.toElement();
        if (childElement.attribute(ATTRIBUTE_NAME) == item->project_name)
        {
            if(action==CheckinAction)
            {
                QDomElement newDomElement=doc.createElement(NODENAME_CHECKINDATE);
                newDomElement.setAttribute(ATTRIBUTE_DATE,item->date.toString("yyyy/MM/dd"));
                newDomElement.setAttribute(ATTRIBUTE_TIP,item->tips);
                newDomElement.setAttribute(ATTRIBUTE_ISCHECKED,item->ischecked);
                childElement.appendChild(newDomElement);
                break;
            }
            else if(action==CheckOutAction) //取消签到
            {
                QDomNodeList deleteNodes = childElement.childNodes(); // 获取子节点列表
                for (int i = 0; i < deleteNodes.size(); i++)
                {
                    QDomNode todeleteNode = deleteNodes.item(i); // 获取当前子节点
                    QDomElement delElement = todeleteNode.toElement();
                    if (delElement.attribute(ATTRIBUTE_DATE) == item->date.toString("yyyy/MM/dd"))
                    {
                        childElement.removeChild(delElement);
                    }
                }
            }
        }
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}



std::vector<project_info*> CheckinConfig::LoadCheckinConfig()
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ CHECKIN_CONFIG_PATH;
    QFile inputfile(filePath);
    std::vector<project_info*> vector_project;
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open checkin.xml file failed"));
        return vector_project;
    }
    QXmlStreamReader reader(&inputfile);

    bool isTotalProjectRange=false;
    //开始解析
    while (!reader.atEnd())
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        switch (token)
        {
        case QXmlStreamReader::StartDocument:
            break;
        case QXmlStreamReader::EndDocument:
            break;
        case QXmlStreamReader::StartElement:
            if(!reader.name().toString().isEmpty())
            {
                project_info *project = new project_info();
                if(reader.name().toString()==NODENAME_TOTALPROJECT)
                {
                    isTotalProjectRange=true;
                }
                else if(reader.name().toString()==NODENAME_PROJECT)
                {
                    if(isTotalProjectRange)
                    {
                        foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                        {
                            logger->log(attribute.name().toString());
                            logger->log(attribute.value().toString());
                            if(attribute.name().toString()==ATTRIBUTE_NAME)
                            {
                                project->project_name= attribute.value().toString();
                            }
                            if(attribute.name().toString()==ATTRIBUTE_ICONINDEX)
                            {
                                project->iconIndex= attribute.value().toString();
                            }
                        }
                        vector_project.push_back(project);
                    }
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if(reader.name().toString()==NODENAME_TOTALPROJECT)
            {
                isTotalProjectRange=false;
            }
            if(reader.name().toString()=="root")
            {
                continue;
            }
            break;
        case QXmlStreamReader::Characters:
            break;
        }
    }

    //是否是正常结束
    if (reader.error())
    {
        QString str="Error: "+reader.errorString()+"in file test.xml at line "+QString::number(reader.lineNumber())+
                ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        vector_project.clear();
        return vector_project;
    }
    //关闭文件
    inputfile.close();
    return vector_project;
}


void CheckinConfig::updateHabitXml(HabitOperationType type,project_info * project)
{
    QString filePath =STORAGE_PATH + CHECKIN_CONFIG_PATH;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXml:open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateXml:set doc content form file failed"));
        file.close();
        return;
    }
    file.close();
    if(type==AddHabit)
    {
        QDomNode parentDomElement=util::selectSingleNode(NODENAME_TOTALPROJECT,&doc);
        QDomElement newDomElement=doc.createElement(NODENAME_PROJECT);
        parentDomElement.appendChild(newDomElement);
        newDomElement.setAttribute(ATTRIBUTE_NAME,project->project_name);
        newDomElement.setAttribute(ATTRIBUTE_ICONINDEX,project->iconIndex);
        newDomElement.setAttribute(ATTRIBUTE_TYPE,project->type);
        //添加detailProject
        QDomNode detailParentDomElement=util::selectSingleNode(NODENAME_DETAILPROJECT,&doc);
        QDomElement detailNewDomElement=doc.createElement(NODENAME_PROJECT);
        detailParentDomElement.appendChild(detailNewDomElement);
        detailNewDomElement.setAttribute(ATTRIBUTE_NAME,project->project_name);
        detailNewDomElement.setAttribute(ATTRIBUTE_TYPE,project->type);
    }
    else if(type==DeleteNode)
    {
       // auto path=util::treeItemToNodePath(currentNode);
        //QDomNode domElement=selectSingleNode(path,&doc);
        ///domElement.parentNode().removeChild(domElement);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

