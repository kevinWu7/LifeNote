#ifndef UTIL_H
#define UTIL_H

#include <QTreeWidgetItem>
#include "baseinfo.h"
#include <QDomElement>


class util
{
public:
    util();
    static QString treeItemToNodeDirPath(QTreeWidgetItem* treeItem);//返回path,如 笔记本/每日工作
    static QString treeItemToNodePath(QTreeWidgetItem* treeItem);//返回path,如 笔记本/每日工作
    static QString treeItemToFileName(QTreeWidgetItem* treeItem);//返回本地存储文件名，如工作.html
    static QString treeItemToFullFilePath(QTreeWidgetItem* treeItem,BaseInfo::NodeType type=BaseInfo::Child );//返回本地存储路径，如D://笔记本/工作.html
    static QString NoRepeatNodeName( QList<QTreeWidgetItem*> &listNode,QString nodeName);
    static bool isStartWidthDigit(const QString& nodeName);
    static void SetDomAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
    static QIcon CreateColorSvgIcon(const QString &path,const QString &color,const QString& opacity="1");
    static std::map<int,std::string> colorBtnMap;

};

#endif // UTIL_H
