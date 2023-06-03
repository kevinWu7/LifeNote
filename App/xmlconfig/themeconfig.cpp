#include <QApplication>
#include <QFile>
#include <QXmlStreamReader>
#include "logger.h"
#include "themeconfig.h"
#include "util.h"
#include "logger.h"


themeConfig* themeConfig::instance = nullptr;
std::once_flag themeConfig::onceFlag;

themeConfig::themeConfig()
{

}

themeConfig& themeConfig::getInstance()
{
    std::call_once(onceFlag, []() {
        instance = new themeConfig();
    });
    return *instance;
}


themeData themeConfig::LoadThemeConfig()
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ THEME_CONFIG_PATH;
    QFile inputfile(filePath);
    themeData _data;
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open checkin.xml file failed"));
        return _data;
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
            if(reader.name().toString()=="ThemeId")
            {
                _data.themeId= reader.readElementText();
            }
            else if(reader.name().toString()=="PictureThemeId")
            {
                _data.picture_themeId= reader.readElementText();
            }
            else if(reader.name().toString()=="Transparency")
            {
                _data.transparency= reader.readElementText().toDouble();
            }
            else if(reader.name().toString()=="LeftTransparency")
            {
                _data.leftTransparency= reader.readElementText().toDouble();
            }
            else if(reader.name().toString()=="RightTransparency")
            {
                _data.rightTransparency= reader.readElementText().toDouble();
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
        QString str="Error: "+reader.errorString()+"LoadThemeConfig at line "+QString::number(reader.lineNumber())+
                ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        return _data;
    }
    //关闭文件
    inputfile.close();
    return _data;
}


void themeConfig::updateXml(const QString& fieldType,const QString& value)
{
    QString filePath = STORAGE_PATH + THEME_CONFIG_PATH;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        logger->log(QString("updateXml: open local xml failed"));
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        logger->log(QString("updateXml: set doc content from file failed"));
        file.close();
        return;
    }
    file.close();

    QDomNodeList themeIdNodes = doc.elementsByTagName(fieldType);
    if (themeIdNodes.size() == 0)
    {
        logger->log(QString("updateXml: ThemeId node not found"));
        return;
    }

    QDomElement themeIdElement = themeIdNodes.at(0).toElement();
    themeIdElement.firstChild().setNodeValue(value);

    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        logger->log(QString("updateXml: open local xml for writing failed"));
        return;
    }

    QTextStream outStream(&file);
    doc.save(outStream, 4);
    file.close();
}
