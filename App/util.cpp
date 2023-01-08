#include "util.h"
#include "baseinfo.h"
#include <QApplication>
util::util()
{

}

//获取节点的路径xml记录的path,如 笔记本/每日工作/无标题.html ，返回 笔记本/每日工作/无标题
QString util::treeItemToNodePath(QTreeWidgetItem* treeItem)
{
    QString fullPath= treeItem->text(0);

    while (treeItem->parent() != NULL)
    {
        fullPath= treeItem->parent()->text(0) + "/" + fullPath;
        treeItem = treeItem->parent();
    }
    return fullPath;
}

//获取节点的物理路径， 如笔记本/每日工作/无标题.html， 返回笔记本/每日工作
QString util::treeItemToNodeDirPath(QTreeWidgetItem *treeItem)
{
    auto path=treeItemToFullFilePath(treeItem);
    int first = path.lastIndexOf ("/");
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
QString util::NoRepeatNodeName(QTreeWidgetItem* parentNode,QString nodeName)
{
    int count=parentNode->childCount();
    int index=0;
    while(true)
    {
        bool isEqual=false;
        QString Name=nodeName+(index==0?"":QString::number(index));
        for(int i=0;i<count;i++)
        {
            auto child= parentNode->child(i);
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
//

