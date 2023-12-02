#include "firstinit.h"
#include <QString>
#include <QApplication>
#include <QFile>
#include <QXmlStreamReader>
#include "logger.h"
#include "util.h"


//获取是否是第一次登陆，若是则全量返回数据。若否则返回差量数据
bool firstinit::getIsFirstInit()
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ FIRSTINIT_CONFIG_PATH;
    QFile inputfile(filePath);
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open init.xml file failed"));
        return false;
    }
    QXmlStreamReader reader(&inputfile);

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
            if(reader.name().toString()=="FirstInit")
            {
               QString isInitText= reader.readElementText();
               bool isInit=isInitText=="true";
               return isInit;
            }
            break;
        case QXmlStreamReader::EndElement:
            break;
        case QXmlStreamReader::Characters:
            break;
        }
    }

    //是否是正常结束
    if (reader.error())
    {
        QString str="Error: "+reader.errorString()+"FirstInit.xml LoadConfig at line "+QString::number(reader.lineNumber())+
                ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        return false;
    }
    //关闭文件
    inputfile.close();
    return false;
}


void firstinit::updateInitNode(bool isInit)
{
    QString filePath =STORAGE_PATH+ FIRSTINIT_CONFIG_PATH;
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateInitNode: open local xml failed"));
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        logger->log(QString("updateInitNode: set doc content form file failed"));
        file.close();
        return;
    }
    file.close();

    QDomNodeList currentDomElement = doc.elementsByTagName("FirstInit");
    if (currentDomElement.size() == 0)
    {
        logger->log(QString("updateXml: firstInit node not found"));
        return;
    }

    QDomElement initElement = currentDomElement.at(0).toElement();
    initElement.firstChild().setNodeValue(isInit?"true":"false");

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}







