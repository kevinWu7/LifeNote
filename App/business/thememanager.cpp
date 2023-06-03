#include <mutex>
#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include <QRegularExpression>
#include "thememanager.h"
#include "theme.h"
#include "themeconfig.h""
#include "util.h"
#include "logger.h"

ThemeManager* ThemeManager::instance = nullptr;
#ifdef QT_NO_DEBUG
QString ThemeManager::ThemeId="light";
#else
QString ThemeManager::ThemeId="dark";
#endif
QString ThemeManager::PictureThemeId="none";;
double  ThemeManager::Transparency=1.0;
double  ThemeManager::LeftTransparency=1.0;
double  ThemeManager::RightTransparency=1.0;

std::once_flag ThemeManager::onceFlag;

ThemeManager::ThemeManager()
{
}

//根据background1来，获取background2 颜色
QString ThemeManager::getBackGround2(QString baseColor,int diff)
{
    // 解析RGB颜色值
    QString colorString = baseColor.mid(4, baseColor.length() - 5); // 去掉"rgb("和")"
    QStringList colorComponents = colorString.split(',');

    if (colorComponents.size() != 3) {
        // 非法的颜色值格式
        return baseColor;
    }

    // 提取颜色分量
    int red = colorComponents[0].toInt();
    int green = colorComponents[1].toInt();
    int blue = colorComponents[2].toInt();

    // 加上diff的值
    red += diff;
    green += diff;
    blue += diff;

    // 检查是否超出范围
    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);

    // 构建新的颜色字符串
    QString newColor = QString("rgb(%1,%2,%3)").arg(red).arg(green).arg(blue);
    return newColor;
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

void ThemeManager::switchTheme(QString _themeId,QString picture_ThemeId,bool isFirstInit)
{
    QString path;
    QString baseBackgroundColor;
    currentTheme.clear();

    QRegularExpression regex("\\d");
    // 在输入字符串中查找第一个数字出现的位置
    QRegularExpressionMatch match = regex.match(_themeId);
    // 获取左边的颜色部分
    QString themeLeftColor = _themeId.left( match.capturedStart());
    QRegularExpressionMatch pic_match = regex.match(picture_ThemeId);
    QString picIndex = picture_ThemeId.right(picture_ThemeId.length()- pic_match.capturedStart());
    if(_themeId=="dark")
    {
        path=QCoreApplication::applicationDirPath()+ "/qss/dark.qss";
        currentTheme=themeDark;
        baseBackgroundColor=currentTheme["BACKGROUND_COLOR1"];
    }
    else if(_themeId=="light")
    {
        path=QCoreApplication::applicationDirPath()+ "/qss/light.qss";
        currentTheme=themeLight;
        baseBackgroundColor=currentTheme["BACKGROUND_COLOR1"];
    }
    else  //纯色主题
    {
        int number=_themeId.right(1).toInt();
        baseBackgroundColor=diyThemeColor[_themeId];
        if(number<=diyThemeIndex[themeLeftColor])
        {
            path=QCoreApplication::applicationDirPath()+ "/qss/light.qss";
            currentTheme=themeLight;
            currentTheme["BACKGROUND_COLOR2"]= getBackGround2(baseBackgroundColor,-20);
            currentTheme["CONTROL_SELECTED"]= getBackGround2(baseBackgroundColor,-30);
            currentTheme["CONTROL_NOSELECTED"]= getBackGround2(baseBackgroundColor,-15);
            currentTheme["CONTROL_POOLTEXT"]= getBackGround2(baseBackgroundColor,-100);
            currentTheme["SINGLE_LINE_COLOR"]=util::generateRGBAString(currentTheme["CONTROL_SELECTED"],0.5);
            currentTheme["SCROLLBAR_HANDLE"]= getBackGround2(baseBackgroundColor,-50);
            currentTheme["SCROLLBAR_HOVER"]= getBackGround2(currentTheme["SCROLLBAR_HANDLE"],20);
        }
        else
        {
            path=QCoreApplication::applicationDirPath()+ "/qss/dark.qss";
            currentTheme=themeDark;
            currentTheme["BACKGROUND_COLOR2"]= getBackGround2(baseBackgroundColor,30);
            currentTheme["CONTROL_SELECTED"]= getBackGround2(baseBackgroundColor,70);
            currentTheme["CONTROL_NOSELECTED"]= getBackGround2(baseBackgroundColor,20);
            currentTheme["CONTROL_POOLTEXT"]= getBackGround2(baseBackgroundColor,100);
            currentTheme["SINGLE_LINE_COLOR"]=util::generateRGBAString(currentTheme["CONTROL_SELECTED"],0.5);
            currentTheme["SCROLLBAR_HANDLE"]= getBackGround2(baseBackgroundColor,50);
            currentTheme["SCROLLBAR_HOVER"]= getBackGround2(currentTheme["SCROLLBAR_HANDLE"],30);
        }
        currentTheme["BACKGROUND_COLOR1"]=baseBackgroundColor;
    }
    currentTheme["LEFT_BACK_COLOR1"]=util::generateRGBAString(baseBackgroundColor,LeftTransparency/100.0);
    currentTheme["RIGHT_BACK_COLOR1"]=util::generateRGBAString(baseBackgroundColor,RightTransparency/100.0);
    QString picPath =QString(":/imgs/res/images/pic%1.jpg").arg(picIndex);
    currentTheme["PICTURE_PATH"]=picPath;
    if(_themeId!= this->ThemeId)
    {
        themeConfig::getInstance().updateXml("ThemeId",_themeId);
        this->ThemeId=_themeId;
    }
    if(picture_ThemeId!=  this->PictureThemeId)
    {//picture_ThemeId 为空，也就是整个为none的情况需要补充
        themeConfig::getInstance().updateXml("PictureThemeId",picture_ThemeId);
        this->PictureThemeId=picture_ThemeId;
    }

    QFile f(path);
    if (!f.exists())
    {
        logger->log("Unable to set stylesheet, file not found");
        return;
    }
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    QString allstyle=ts.readAll();
    for(auto item : currentTheme)
    {
        allstyle= allstyle.replace(item.first,item.second);
    };
    qApp->setStyleSheet(allstyle);
    if(!isFirstInit)
    {
        ThemeManager::getInstance().triggerThemeGlobalEvent();
    }
}




