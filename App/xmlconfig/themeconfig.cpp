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


QString themeConfig::LoadThemeConfig()
{
    //设置输入文件
    QString filePath =STORAGE_PATH+ THEME_CONFIG_PATH;
    QFile inputfile(filePath);
    QString themeId="";
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        logger->log(QString("loadConfigXML: Open checkin.xml file failed"));
        return themeId;
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
                foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                {
                    if(attribute.name().toString()=="id""")
                    {
                        themeId= attribute.value().toString();
                        break;
                    }
                }
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
        QString str="Error: "+reader.errorString()+"in file test.xml at line "+QString::number(reader.lineNumber())+
                ",column "+QString::number(reader.columnNumber());
        logger->log(str);
        return themeId;
    }
    //关闭文件
    inputfile.close();
    return themeId;
}


void themeConfig::updateXml(QString themeId)
{
    QString filePath =STORAGE_PATH+ THEME_CONFIG_PATH;
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

    QDomNode parentDomElement=util::selectSingleNode("ThemeId",&doc);
    parentDomElement.toElement().setAttribute("id",themeId);
    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}
