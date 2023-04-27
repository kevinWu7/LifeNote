#ifndef NODECONFIG_H
#define NODECONFIG_H

#include <iostream>
#include <vector>
#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QTreeWidgetItem>
#include <QDomElement>
#include "baseinfo.h"
#include "extraqtreewidgetitem.h"



class nodeconfig
{
public:
    nodeconfig();
    static void loadConfigXML(QTreeWidget *tree_widget);//启动时，读取文件加载node
    //currentNode is The node that is being operated
    //newNode is the Node in the Add  OperationType
    static void updateXml( OperationType type,QTreeWidgetItem *currenttNode,QTreeWidgetItem *newNode=nullptr);//节点发生改变时，重新生成配置文件
    static void updateXmlRenameNode(const QString& oldName,QTreeWidgetItem *currentNode);
    static void updateXmlAddTopLevelNode(ExtraQTreeWidgetItem *newNode,QTreeWidgetItem *collectNode);


};

#endif // NODECONFIG_H
