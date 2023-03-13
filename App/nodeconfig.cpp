#include "nodeconfig.h"
#include "extraqtreewidgetitem.h"
#include "util.h"
#include "logger.h"
#include <QCoreApplication>



#define CONFIG_PATH "/config/node.xml"

nodeconfig::nodeconfig()
{

}
struct node_info
{
    QString node_name;
    ExtraQTreeWidgetItem *widgetitem;
};

//XML struct is seem like qtreeWidget ,is a tree
//The node is added to the treewidget by loop the xml document
void nodeconfig::loadConfigXML(QTreeWidget *tree_widget)
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
                BaseInfo::NodeType isParent=BaseInfo::Child;
                node->widgetitem=new ExtraQTreeWidgetItem(isParent);
                foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                {
                    logger->log(attribute.name().toString());
                    logger->log(attribute.value().toString());
                    if(attribute.name().toString()==ATTRIBUTE_NOTETYPE)
                    {
                        isParent= attribute.value().toString()=="0"?BaseInfo::Child:BaseInfo::Parent;
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
//            if(vector_node.size()>0)
//            {
//                QString type= reader.text().toString();
//                auto lastNode=vector_node.at(vector_node.size()-1);
//                lastNode->widgetitem->nodeType= type=="0"?ExtraQTreeWidgetItem::NodeChild:ExtraQTreeWidgetItem::NodeParent;
//            }
            break;
        }
    }

    //是否是正常结束
    if (reader.error())
    {
        QString str="Error: "+reader.errorString()+"in file test.xml "
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
void nodeconfig::updateXml(BaseInfo::OperationType type,QTreeWidgetItem *currentNode,QTreeWidgetItem *newNode)
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
    if(type==BaseInfo::AddNode||type==BaseInfo::AddNodeGroup)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode parentDomElement=selectSingleNode(path,&doc);
        //Check whether the node name starts with a digit， xml nodename is invaild start with digit
        bool startWithDigit=util::isStartWidthDigit(newNode->text(0));
        QDomElement newDomElement=doc.createElement(startWithDigit?(START_FLAG+newNode->text(0)):newNode->text(0));
        if(startWithDigit)
        {
            newDomElement.setAttribute(ATTRIBUTE_STARTFLAG,"true");
        }
        parentDomElement.appendChild(newDomElement);
        newDomElement.setAttribute(ATTRIBUTE_NOTETYPE,type==BaseInfo::AddNode?0:1);
        if(type==BaseInfo::AddNodeGroup&&currentNode!=NULL)
        {
            newDomElement.setAttribute(ATTRIBUTE_COLORINDEX,dynamic_cast<ExtraQTreeWidgetItem*>(currentNode)->colorIndex);
        }
    }
    else if(type==BaseInfo::MoveNode)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode domElement=selectSingleNode(path,&doc);
        //remove the currentNode
        domElement.parentNode().removeChild(domElement);
        //recyledomNode  add the current
        auto recylePath=util::treeItemToNodePath(newNode);
        QDomNode recyleDomNode=selectSingleNode(recylePath,&doc);
        recyleDomNode.appendChild(domElement);
    }
    else if(type==BaseInfo::DeleteNode)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode domElement=selectSingleNode(path,&doc);
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

void nodeconfig::updateXmlAddTopLevelNode(ExtraQTreeWidgetItem *newNode,QTreeWidgetItem *collectNode)
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
    QDomNode parentDomElement=selectSingleNode(path,&doc);
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

void nodeconfig::updateXmlRenameNode(const QString& oldPath,QTreeWidgetItem *currentNode)
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
    QDomNode currentDomElement=selectSingleNode(oldPath,&doc);
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



//根据xml的路径，找出xml的相应节点QDomNode
//循环遍历xml节点，通过判断节点名是否和path的相应部分匹配，不断向下找
//为什么写这个方法，是因为原先只能通过在xml中设置属性，通过属性来判断，如下面这段代码，但是这样会导致xml内容臃肿，所以selectSingleNode能让xml看起来更简洁
/*
 QDomNodeList list = doc.elementsByTagName(path);
 for(int i=0;i<list.size();i++)
 {
    QDomElement e = list.at(i).toElement();
    if(e.attribute("path")==path)
    {
        QDomElement newDomElement=doc.createElement(newNode->text(0));
        list.at(i).appendChild(newDomElement);
        break;
    }
}*/
QDomNode nodeconfig::selectSingleNode(const QString& path,QDomDocument* doc) const
{
   // doc->documentElement()
    QStringList list=path.split(u'/');

    int i=0;
    auto rootElement=doc->documentElement();
    auto childNode=rootElement.firstChild();
    QString currentStr=list.at(0);

       while(!childNode.isNull())
       {
           if(childNode.toElement().tagName()==list.at(i)||
                 (childNode.toElement().tagName().startsWith(START_FLAG)&& //这个条件判断下若是有开头记号的node，则判断是否带有记号，且包含该字符串list.at(i)
                  childNode.toElement().tagName().contains(list.at(i))))
           {
               if(i==list.size()-1)//完全匹配上了
               {
                   return childNode;
               }
              if(childNode.hasChildNodes())
              {
                  QDomNodeList nodeList= childNode.childNodes();
                  childNode=nodeList.at(0);
                  i++;
                  currentStr=list.at(i);
              }
           }
           else
           {
              childNode=childNode.nextSibling();//将同级的下一个节点，赋给childNode
           }
       }
    return childNode;
}


