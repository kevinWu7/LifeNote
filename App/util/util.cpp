#include <QApplication>
#include <QFile>
#include <QSvgRenderer>
#include <QPainter>
#include <QDir>
#include "baseinfo.h"
#include "logger.h"
#include "util.h"
#include "extraqtreewidgetitem.h"


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


//根据xml的路径，找出xml的相应节点QDomNode
//循环遍历xml节点，通过判断节点名是否和path的相应部分匹配，不断向下找
//为什么写这个方法，是因为原先只能通过在xml中设置属性，通过属性来判断，如下面这段代码，但是这样会导致xml内容臃肿，所以selectSingleNode能让xml看起来更简洁
/*勿删
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
QDomNode util::selectSingleNode(const QString& path,QDomDocument* doc)
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

std::vector<QDate> util::getThisWeek()
{
    QDate currentDate = QDate::currentDate();
    // 获取本周的时间点
    std::vector<QDate> thisWeek;
    int dayOfWeek = currentDate.dayOfWeek();
    QDate startOfWeek = currentDate.addDays(-dayOfWeek + 1); // 周一
    for (int i = 0; i < 7; ++i) {
        QDate date = startOfWeek.addDays(i);
        thisWeek.push_back(date);
    }
    return thisWeek;
}

QMainWindow* util::getCurrentMainWindow()
{
    QApplication* app = qApp;
    QMainWindow* mainWindow = nullptr;

    // 判断应用程序对象类型
    if (app)
    {
        // 获取主窗体指针
        const auto topLevelWidgets = app->topLevelWidgets();
        for (QWidget* widget : topLevelWidgets)
        {
            mainWindow = qobject_cast<QMainWindow*>(widget);
            if (mainWindow)
            {
                break;
            }
        }
    }
    return mainWindow;
}

QMainWindow* util::getQMainWindowByWidget(QWidget* widget)
{
    QMainWindow* mainWindow = nullptr;
    while (widget != nullptr)
    {
        mainWindow = qobject_cast<QMainWindow*>(widget);
        if (mainWindow != nullptr)
        {
            // 找到了 MainWindow
            break;
        }
        widget = widget->parentWidget();
    }
    return mainWindow;
}

QString util::generateRGBAString(const QString& colorString, float alpha)
{
    // 解析RGB颜色值
    QString rgbString = colorString.mid(4, colorString.length() - 5); // 去掉"rgb("和")"
    QStringList colorComponents = rgbString.split(',');

    if (colorComponents.size() != 3) {
        // 非法的颜色值格式
        return colorString;
    }

    // 提取颜色分量
    int red = colorComponents[0].toInt();
    int green = colorComponents[1].toInt();
    int blue = colorComponents[2].toInt();

    // 确保透明度在0到1之间
    alpha = qBound(0.0f, alpha, 1.0f);

    // 构建RGBA字符串
    QString rgbaString = QString("rgba(%1,%2,%3,%4)").arg(red).arg(green).arg(blue).arg(alpha);
    return rgbaString;
}


QString util::getPlatFormName()
{
#ifdef Q_OS_MAC
    return "macos";
#elif Q_OS_WIN
    return "windows";
#else
    return "unknown";
#endif
}


