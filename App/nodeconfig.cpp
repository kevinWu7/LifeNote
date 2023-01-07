#include "nodeconfig.h"
#include "extraqtreewidgetitem.h"

nodeconfig::nodeconfig()
{

}
struct node_info
{
    QString node_name;
    ExtraQTreeWidgetItem *widgetitem;
};

//currentNode is The node that is being operated
//newNode is the Node in the Add  OperationType and UPDATE
void nodeconfig::updateXml(BaseInfo::OperationType type,QTreeWidgetItem *currentNode,QTreeWidgetItem *newNode)
{
    QFile file("../../../config/node.xml");

    if(!file.open(QIODevice::ReadOnly))
    {
        std::cout<<"open local xml failed";
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        std::cout<<"set doc content form file failed";
        file.close();
        return;
    }
    file.close();
    if(type==BaseInfo::AddNode||type==BaseInfo::AddNodeGroup)
    {
        auto path=util::treeItemToNodePath(currentNode);
        QDomNode parentDomElement=selectSingleNode(path,&doc);
        QDomElement newDomElement=doc.createElement(newNode->text(0));
        parentDomElement.appendChild(newDomElement);
        newDomElement.setAttribute("nodetype",type==BaseInfo::AddNode?0:1);
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

//XML struct is seem like qtreeWidget ,is a tree
//The node is added to the treewidget by loop the xml document
void nodeconfig::readNodeConfigXML(QTreeWidget *tree_widget)
{
    //设置输入文件
    QFile inputfile("../../../config/node.xml");
    qDebug()<<QDir::currentPath();
    if(!inputfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Open node.xml file failed";
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
                foreach (const QXmlStreamAttribute & attribute, reader.attributes())
                {
                    qDebug()<<attribute.name();
                    qDebug()<<attribute.value();
                    if(attribute.name().toString()=="nodetype")
                    {
                        isParent= attribute.value().toString()=="0"?BaseInfo::Child:BaseInfo::Parent;
                        break;
                    }
                }
                node->widgetitem=new ExtraQTreeWidgetItem(isParent);
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
        qDebug() << "Error: " << reader.errorString() << "in file test.xml at line " << reader.lineNumber() << ", column " << reader.columnNumber();
        return ;
    }

    //关闭文件
    inputfile.close();
    return;
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
           if(childNode.toElement().tagName()==list.at(i))
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


