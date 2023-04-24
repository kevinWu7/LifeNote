#include "util.h"
#include "baseinfo.h"
#include "logger.h"
#include "extraqtreewidgetitem.h"
#include <QApplication>
#include <QFile>
#include <QSvgRenderer>
#include <QPainter>
#include <QDir>

std::map<int,std::string> util::iconMap={
    {0,"computer.png" },{1,"run.png" },{2,"english.png" },{3,"study.png" },{4,"train.png" },
    {5,"english2.png" },{6,"notes.png" },{7,"game.png" },{8,"bowl.png" },{9,"tableware.png" },
    {10,"cat.png" },{11,"dog.png" },{12,"coffee.png" },{13,"train2.png" },
};

std::map<int, std::string> util::colorBtnMap = {
    {0, "rgb(125,180,255)"}, {1, "rgb(212,171,128)"}, {2, "rgb(168,119,199)"},
    {3, "rgb(110,207,101)"}, {4, "rgb(241,92,228)"},  {5, "rgb(162,171,178)"},
    {6, "rgb(252,100,110)"}, {7, "rgb(252,77,6)"},    {8, "rgb(156,184,120)"},
    {9, "rgb(181,169,70)"},  {10, "rgb(57,179,173)"}, {11, "rgb(107,105,141)"},
};

std::vector<QString> util::fontVector={
    "9","11","13","14","16","18","20","22","24","28","32","36","40","44","50","55","60","70"
};

//获取节点的路径xml记录的path,如 笔记本/每日工作/无标题.html,返回 笔记本/每日工作/无标题
QString util::treeItemToNodePath(QTreeWidgetItem *treeItem)
{
    QString fullPath = treeItem->text(0);

    while (treeItem->parent() != nullptr)
    {
        fullPath = treeItem->parent()->text(0) + "/" + fullPath;
        treeItem = treeItem->parent();
    }
    return fullPath;
}

//获取节点的物理路径， 如笔记本/每日工作/无标题.html， 返回笔记本/每日工作
QString util::treeItemToNodeDirPath(QTreeWidgetItem *treeItem)
{
    auto path = treeItemToFullFilePath(treeItem);
    int first = path.lastIndexOf("/");
    QString fileName = path.left(first); //文件名称，如xxx.html
    return fileName;
}
//返回本地存储文件名，如无标题.html
QString util::treeItemToFileName(QTreeWidgetItem* treeItem)
{
    auto path=treeItemToFullFilePath(treeItem);
    int first = path.lastIndexOf ("/");
    QString fileName = path.right(path.length()-first-1); //文件名称，如xxx.html
    return fileName;
}

//return the full path
//if the type is child,return full file storagePath for NodeItem, for instance - D://笔记本/工作.html
//if the type is parent,return full storagePath for newItemGroup（means a file directoy，not a file） for instance - D://笔记本/新建笔记本1
QString util::treeItemToFullFilePath(QTreeWidgetItem* treeItem)
{
    auto currentPath=STORAGE_PATH;
    //保存上一个节点的内容
    QString nodePath=treeItemToNodePath(treeItem);
    auto type= dynamic_cast<ExtraQTreeWidgetItem*>(treeItem)->nodeType;
    if(type==ChildNode)
    {
        return QString("%1/storage/%2.html").arg(currentPath,nodePath);
    }
    else if(type==ParentNode)
    {
        return QString("%1/storage/%2").arg(currentPath,nodePath);
    }
    return "";
}

//传入parentnode,输出一个不重复的标题
QString util::NoRepeatNodeName( QList<QTreeWidgetItem*> &listNode,QString nodeName)
{
    int count=listNode.count();
    int index=0;
    while(true)
    {
        bool isEqual=false;
        QString Name=nodeName+(index==0?"":QString::number(index));
        for(int i=0;i<count;i++)
        {
            auto child= listNode.at(i);
            if(child->text(0)==Name)
            {
                index++;
                isEqual=true;
                break;
            }
        }
        if(!isEqual)//若循环完，都不相同，那么跳出while循环
        {
            break;
        }
    }
    return nodeName+(index==0?"":QString::number(index));
}

bool util::isStartWidthDigit(const QString& nodeName)
{
    if(nodeName.length()>0)
    {
        auto c=nodeName.left(1).toStdString();
        return std::isdigit(c[0]);
    }
    return false;
}


QIcon util::CreateColorSvgIcon(const QString &path,const QString &color,const QString &opacity)
{
    logger->log(QString("createcolor"));
    // open svg resource load contents to qbytearray
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    // load svg contents to xml document and edit contents
    QDomDocument doc;
    doc.setContent(baData);
    auto domelement=doc.documentElement();
    // recurivelly change color
    QDomElement element=doc.documentElement();
    util::SetDomAttrRecur(element, "path", "fill", color);
    //set opacity
    util::SetDomAttrRecur(element, "path", "fill-opacity", opacity);

    // create svg renderer with edited contents
    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize());
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);
    // Use icon ....
    return myicon;
}


void util::SetDomAttrRecur( QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
    {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        auto ele=elem.childNodes().at(i).toElement();
        util::SetDomAttrRecur(ele, strtagname, strattr, strattrval);
    }
}

//剪切文件夹。比如把整个/bin 剪切到c://aaa下面，就变成了c:/aaa/bin
bool util::cutDir(const QString &source, const QString &destination, bool override)
{
    int first = source.lastIndexOf ("/");
    QString foldername = source.right(source.length()-first-1); //文件夹名
    auto desfolder=QString("%1/%2").arg(destination,foldername);
    QDir dirdes;
    dirdes.mkdir(desfolder);
    bool result=copyDir(source,desfolder,override);
    if(result)
    {
       QDir dir(source);
       return dir.removeRecursively();//删除原文件
    }
    return false;
}

/**
 * @brief 拷贝文件夹内的所有文件到目的文件夹。比如/bin下面有a.txt,b.txt,这两个会拷贝过去。bin文件夹在原地
 * @param source 源文件夹全路径，比如  "F:/tx" ,"F:/txd/des/desd"
 * @param source 要COPY到的目的路径 比如 "F:/tx/des/desd"
 * @param override 如果目的文件存在，比如 "F:/txd/des/desd" 存在，是否覆盖，true表示覆盖
 */
bool util::copyDir(const QString &source, const QString &destination, bool override)
{
    QDir directory(source);
    if (!directory.exists())
    {
        return false;
    }

    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();

    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                //QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            error=QFile::copy(srcFilePath, dstFilePath);//若是相同文件拷贝，则会导致文件为空
            if(!error)
            {
                return false;
            }
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }
    return !error;
}

bool util::isChildItem(QTreeWidgetItem* parentItem, QTreeWidgetItem* childItem)
{
    while (childItem != nullptr)
    {
        if (childItem == parentItem)
        {
            return true;
        }
        childItem = childItem->parent();
    }
    return false;
}




