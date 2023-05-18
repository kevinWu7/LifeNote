#include <mutex>
#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include "thememanager.h"
#include "theme.h"


ThemeManager* ThemeManager::instance = nullptr;
QString ThemeManager::ThemeId="dark";
std::once_flag ThemeManager::onceFlag;

ThemeManager::ThemeManager()
{
}

ThemeManager& ThemeManager::getInstance()
{
    std::call_once(onceFlag, []()
    {
        instance = new ThemeManager();
    });
    return *instance;
}

void ThemeManager::registerThemeGlobalEvent(themeChangedCallback callback)
{
    callbackList.push_back(callback);
}


void ThemeManager::unregisterThemeGlobalEvent(themeChangedCallback callback)
{
    auto it = std::find_if(callbackList.begin(), callbackList.end(),
        [callback](const themeChangedCallback& cb) { return &cb == &callback; });

    if (it != callbackList.end())
    {
        callbackList.erase(it);
    }
}


void ThemeManager::triggerThemeGlobalEvent()
{
    for (themeChangedCallback callback:callbackList)
    {
        callback();
    }
}

QMainWindow* ThemeManager::getCurrentMainWindow()
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

void ThemeManager::switchTheme(QString _themeId,bool isFirstInit)
{
    QString path;
    QString titleColor;
    std::map<QString,QString> themeMap;
    if(_themeId=="dark")
    {
        path=QCoreApplication::applicationDirPath()+ "/qss/dark.qss";
        titleColor=DARK_BACKGROUND;
        themeMap=themeDark;
    }
    else if(_themeId=="light")
    {
        path=QCoreApplication::applicationDirPath()+ "/qss/light.qss";
        titleColor=LIGHT_BACKGROUND;
        themeMap=themeLight;
    }
    else if(_themeId=="blue")
    {
        path=QCoreApplication::applicationDirPath()+ "/qss/dark.qss";
        titleColor=BLUE_BACKGROUND;
        themeMap=themeBlue;
    }
    QFile f(path);
    if (!f.exists())
    {
        qDebug() << "Unable to set stylesheet, file not found";
    }
    else
    {
        this->ThemeId=_themeId;
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);

        QString allstyle=ts.readAll();
        for(auto item : themeMap)
        {
            allstyle= allstyle.replace(item.first,item.second);
        };
        qApp->setStyleSheet(allstyle);
        auto mainWindow=getCurrentMainWindow();
        if(mainWindow)
        {
            Cocoa::changeTitleBarColor(getCurrentMainWindow()->winId(), titleColor);
        }
        if(!isFirstInit)
        {

            ThemeManager::getInstance().triggerThemeGlobalEvent();
        }
    }
}
