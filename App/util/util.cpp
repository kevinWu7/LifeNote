#include "util.h"
#include "baseinfo.h"
#include <QApplication>
#include<QFile>
#include<QSvgRenderer>
#include<QPainter>
util::util()
{

}

std::map<int, std::string> util::colorBtnMap = {
    {0, "rgb(125,180,255)"}, {1, "rgb(212,171,128)"}, {2, "rgb(168,119,199)"},
    {3, "rgb(110,207,101)"}, {4, "rgb(241,92,228)"},  {5, "rgb(162,171,178)"},
    {6, "rgb(252,100,110)"}, {7, "rgb(252,77,6)"},    {8, "rgb(156,184,120)"},
    {9, "rgb(181,169,70)"},  {10, "rgb(57,179,173)"}, {11, "rgb(107,105,141)"},
};

std::vector<QString> util::fontVector={
    "7","8","9","10","13","15","17","20","24","28","32","36","40","44"
};

//获取节点的路径xml记录的path,如 笔记本/每日工作/无标题.html ，返回 笔记本/每日工作/无标题
QString util::treeItemToNodePath(QTreeWidgetItem *treeItem)
{
    QString fullPath = treeItem->text(0);

    while (treeItem->parent() != NULL)
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

//return the full path，default type is 0
//if the type is 0(Node),return full file storagePath for NodeItem, for instance - D://笔记本/工作.html
//if the type is 1(NodeGroup),return full storagePath for newItemGroup（means a file directoy，not a file） for instance - D://笔记本/新建笔记本1
QString util::treeItemToFullFilePath(QTreeWidgetItem* treeItem,BaseInfo::NodeType type)
{
    auto currentPath= QCoreApplication::applicationDirPath();
    //保存上一个节点的内容
    QString nodePath=treeItemToNodePath(treeItem);
    if(type==BaseInfo::Child)
    {
        return QString("%1/storage/%2.html").arg(currentPath,nodePath);
    }
    else if(type==BaseInfo::Parent)
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
//

