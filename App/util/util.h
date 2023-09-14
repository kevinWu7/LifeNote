#ifndef UTIL_H
#define UTIL_H

#include <QTreeWidgetItem>
#include <QDomElement>
#include <QMainWindow>
#include <QMenu>
#include <QGraphicsDropShadowEffect>


#ifdef QT_NO_DEBUG
    #ifdef Q_OS_MAC
        #define STORAGE_PATH  QString("/Users/wuchengcheng/Documents/Lifenote_dir")
    #elif defined(Q_OS_WIN)
        #define STORAGE_PATH QCoreApplication::applicationDirPath()
    #endif
#else
    #define STORAGE_PATH QCoreApplication::applicationDirPath()
#endif

class util
{

public:
    util();
    static QString treeItemToNodeDirPath(QTreeWidgetItem* treeItem);//返回path,如 笔记本/每日工作
    static QString treeItemToNodePath(QTreeWidgetItem* treeItem);//返回path,如 笔记本/每日工作
    static QString treeItemToFileName(QTreeWidgetItem* treeItem);//返回本地存储文件名，如工作.html
    static QString treeItemToFullFilePath(QTreeWidgetItem* treeItem);//返回本地存储路径，如D://笔记本/工作.html
    static QString NoRepeatNodeName( QList<QTreeWidgetItem*> &listNode,QString nodeName);
    static bool isStartWidthDigit(const QString& nodeName);
    static void SetDomAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
    static QIcon CreateColorSvgIcon(const QString &path,const QString &color,const QString& opacity="1");
    static bool copyDir(const QString &source, const QString &destination, bool override);
    static bool cutDir(const QString &source, const QString &destination, bool override);
    static bool isChildItem(QTreeWidgetItem* parentItem, QTreeWidgetItem* childItem);
    static std::map<int,std::string> colorBtnMap;
    static std::map<int,std::string> iconMap;
    static std::vector<QString> fontVector;
    static QDomNode selectSingleNode(const QString& path,QDomDocument* doc);
    static std::vector<QDate> getThisWeek();
    static QMainWindow* getQMainWindowByWidget(QWidget* widget);
    static QMainWindow* getCurrentMainWindow();
    static QString getPlatFormName();
    static QString generateRGBAString(const QString& colorString, float alpha);
    static QColor generateRGBAColor(const QString& colorString, float alpha);
    static void ChangeQMenuStyle( QMenu& menu);
    template<typename T>
    static T *findWidget(QWidget *parent, const QString &objectName)
    {
        if (!parent)
        {
            return nullptr;
        }

        T *childWidget = parent->findChild<T *>(objectName);
        if (childWidget)
        {
            //qDebug() << "Found widget with object name:" << objectName;
        } else
        {
            qDebug() << "Widget with object name" << objectName << "not found";
        }
        return childWidget;
    }
    template<typename T>
   static T* findParentWidget(QObject* object)
    {
        QObject* parent = object->parent();
        while (parent)
        {
            if (T* typedParent = qobject_cast<T*>(parent))
                return typedParent;

            parent = parent->parent();
        }

        return nullptr;
    }

private:

};

#endif // UTIL_H
