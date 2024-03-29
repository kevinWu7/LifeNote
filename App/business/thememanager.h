#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H
#include <mutex>
#include <vector>
#include <QString>
#include <QMainWindow>

#ifdef Q_OS_MAC
namespace Cocoa
{
  void changeTitleBarColor(WId winId,QString color);
  void changeTitleBarHeight(WId winId, double height);
  void setWindowTitleColor(WId winId, const char *color);
  void setWindowTitleMerged(WId winId);
}
#endif

typedef std::function<void()> themeChangedCallback;
class ThemeManager
{
public:
    static ThemeManager& getInstance();
    static QString ThemeId;
    static QString PictureThemeId;
    static bool isDarkTheme; //是否为深色系皮肤,目前仅用于选中节点后，文字高亮
    static double Transparency;
    static double LeftTransparency;
    static double RightTransparency;

    // 注册全局事件
    void registerThemeGlobalEvent(themeChangedCallback callback);
    void unregisterThemeGlobalEvent(themeChangedCallback callback);
    void triggerThemeGlobalEvent();
    void switchTheme(QString themeId,QString picture_ThemeId,bool isFirstIni=false);
    QMainWindow* getCurrentMainWindow();
    std::vector<themeChangedCallback> callbackList;
private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    QString getBackGround2(QString baseColor,int diff);

    static ThemeManager* instance;
    static std::once_flag onceFlag;
};

#endif // THEMEMANAGER_H

