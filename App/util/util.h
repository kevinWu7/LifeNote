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
    static QString NoRepeatNodeName(QTreeWidgetItem* parentNode,QString nodeName);
    static bool isStartWidthDigit(const QString& nodeName);
    static void SetDomAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
    static QIcon ChangeSVGColor(const QString &path,const QString &color);
    inline static std::map<int,std::string> colorBtnMap=
        {
          {0,"rgb(125,180,255)"},
          {1,"rgb(212,171,128)"},
          {2,"rgb(168,119,199)"},
          {3,"rgb(110,207,101)"},
          {4,"rgb(241,92,228)"},
          {5,"rgb(162,171,178)"},
          {6,"rgb(252,100,110)"},
          {7,"rgb(252,77,6)"},
          {8,"rgb(156,184,120)"},
          {9,"rgb(181,169,70)"},
          {10,"rgb(57,179,173)"},
          {11,"rgb(107,105,141)"},
        };
};

#endif // UTIL_H
