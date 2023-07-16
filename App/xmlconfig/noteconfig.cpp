#include <QCoreApplication>
#include "noteconfig.h"
#include "extraqtreewidgetitem.h"
#include "util.h"
#include "logger.h"


#define CONFIG_PATH "/config/node.xml"

noteconfig::noteconfig()
{

}
struct node_info
{
    QString node_name;
    ExtraQTreeWidgetItem *widgetitem;
};

//XML struct is seem like qtreeWidget ,is a tree
//The node is added to the treewidget by loop the xml document
void noteconfig::loadConfigXML(QTreeWidget *tree_widget)
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ CONFIG_PATH;
    QFile inputfile(filePath);
    logger->log(filePath);
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open node.xml file failed"));
        return ;
    }
    QXmlStreamReader reader(&inputfile);
    std::vector<node_info*> vector_node;
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
                node_info *node = new node_info();
                node->node_name=reader.name().toString();
                NodeType isParent=ChildNode;
                node->widgetitem=new ExtraQTreeWidgetItem(isParent);

                foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                {
                    logger->log(attribute.name().toString());
                    logger->log(attribute.value().toString());
                    if(attribute.name().toString()==ATTRIBUTE_NOTETYPE)
                    {
                        isParent= attribute.value().toString()=="0"?ChildNode:ParentNode;
                    }
                    else if(attribute.name().toString()==ATTRIBUTE_COLORINDEX)
                    {
                          node->widgetitem->colorIndex=attribute.value().toString();
                    }
                    else if(attribute.name().toString()==ATTRIBUTE_STARTFLAG)//若是有startflag标记的，还原成原来的字符串
                    {
                        if(node->node_name.startsWith(START_FLAG))
                        {
                           node->node_name=node->node_name.mid(1,node->node_name.length()-1);
                        }
                    }
                }
                node->widgetitem->nodeType=isParent;
                if( node->node_name=="root")
                {
                    continue;
                }
                if(vector_node.size()==0)
                {
                    tree_widget->addTopLevelItem(node->widgetitem);
                }
                else if(vector_node.size()>0)
                {
                    auto preview_node=vector_node.at(vector_node.size()-1);
                    preview_node->widgetitem->addChild(node->widgetitem);
                }
                vector_node.push_back(node);
                node->widgetitem->setText(0,node->node_name);
            }
            break;
        case QXmlStreamReader::EndElement:
            if(reader.name().toString()=="root")
            {
                continue;
            }
            if(vector_node.size()>0)
            {
                //delete last node of vector，then we can always get the parent through previousNode
                vector_node.pop_back();
            }
            break;
        case QXmlStreamReader::Characters:
            break;
        }
    }

    //是否是正常结束
    if (reader.error())
    {
        QString str="Error: "+reader.errorString()+"loadConfigXML noteconfig "
                     "at line "+QString::number(reader.lineNumber())+
                     ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        return ;
    }

    //关闭文件
    inputfile.close();
    return;
}

//currentNode is The node which is being operated
//newNode is the Node in the Add  OperationType and UPDATE
void noteconfig::updateXml(OperationType type,QTreeWidgetItem *currentNode,QTreeWidgetItem *newNode)
{
    QString filePath =STORAGE_PATH+ CONFIG_PATH;
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
    if(type==AddNode||type==AddNodeGroup)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode parentDomElement=util::selectSingleNode(path,&doc);
        //Check whether the node name starts with a digit， xml nodename is invaild start with digit
        bool startWithDigit=util::isStartWidthDigit(newNode->text(0));
        QDomElement newDomElement=doc.createElement(startWithDigit?(START_FLAG+newNode->text(0)):newNode->text(0));
        if(startWithDigit)
        {
            newDomElement.setAttribute(ATTRIBUTE_STARTFLAG,"true");
        }
        parentDomElement.appendChild(newDomElement);
        newDomElement.setAttribute(ATTRIBUTE_NOTETYPE,type==AddNode?0:1);
        if(type==AddNodeGroup&&currentNode!=NULL)
        {
            newDomElement.setAttribute(ATTRIBUTE_COLORINDEX,dynamic_cast<ExtraQTreeWidgetItem*>(currentNode)->colorIndex);
        }
    }
    else if(type==MoveNode)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode domElement=util::selectSingleNode(path,&doc);
        //remove the currentNode
        domElement.parentNode().removeChild(domElement);
        //newdomNode  add the current
        auto newPath=util::treeItemToNodePath(newNode);
        QDomNode newDomNode=util::selectSingleNode(newPath,&doc);
        newDomNode.appendChild(domElement);
    }
    else if(type==InsertNode)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode domElement=util::selectSingleNode(path,&doc);
        //remove the currentNode
        domElement.parentNode().removeChild(domElement);
        //newdomNode  add the current
        auto newPath=util::treeItemToNodePath(newNode);
        QDomNode newDomNode=util::selectSingleNode(newPath,&doc);
        newDomNode.parentNode().insertBefore(domElement,newDomNode);
    }
    else if(type==DeleteNode)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode domElement=util::selectSingleNode(path,&doc);
        domElement.parentNode().removeChild(domElement);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void noteconfig::updateXmlAddTopLevelNode(ExtraQTreeWidgetItem *newNode,QTreeWidgetItem *collectNode)
{
    QString filePath =STORAGE_PATH+ CONFIG_PATH;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXmlAddTopLevelNode:open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateXmlAddTopLevelNode:set doc content form file failed"));
        file.close();
        return;
    }
    file.close();
    auto path=util::treeItemToNodePath(collectNode);
    QDomNode parentDomElement=util::selectSingleNode(path,&doc);
    bool startWithDigit=util::isStartWidthDigit(newNode->text(0));
    QDomElement newDomElement=doc.createElement(startWithDigit?(START_FLAG+newNode->text(0)):newNode->text(0));

    if(startWithDigit)
    {
        newDomElement.setAttribute(ATTRIBUTE_STARTFLAG,"true");
    }
    doc.documentElement().insertBefore(newDomElement,parentDomElement);
    newDomElement.setAttribute(ATTRIBUTE_NOTETYPE,1);
    newDomElement.setAttribute(ATTRIBUTE_COLORINDEX,newNode->colorIndex);
    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void noteconfig::updateXmlRenameNode(const QString& oldPath,QTreeWidgetItem *currentNode)
{
    QString filePath =STORAGE_PATH+ CONFIG_PATH;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXmlRenameNode: open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateXmlRenameNode: set doc content form file failed"));
        file.close();
        return;
    }
    file.close();
    QDomNode currentDomElement=util::selectSingleNode(oldPath,&doc);
    //Check whether the node name starts with a digit， xml nodename is invaild start with digit
    bool startWithDigit=util::isStartWidthDigit(currentNode->text(0));
    currentDomElement.toElement().setTagName(startWithDigit?(START_FLAG+currentNode->text(0)):currentNode->text(0));
    if(startWithDigit)
    {
        currentDomElement.toElement().setAttribute(ATTRIBUTE_STARTFLAG,"true");
    }
    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}




