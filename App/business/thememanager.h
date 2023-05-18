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
  void changeTitleBarTextColor(WId winId, QString color);
}
#endif

typedef std::function<void()> themeChangedCallback;
class ThemeManager
{
public:
    static ThemeManager& getInstance();
    static QString ThemeId;
    // 注册全局事件
    void registerThemeGlobalEvent(themeChangedCallback callback);
    void unregisterThemeGlobalEvent(themeChangedCallback callback);
    void triggerThemeGlobalEvent();
    void switchTheme(QString themeId,bool isFirstIni=false);
    QMainWindow* getCurrentMainWindow();
    std::vector<themeChangedCallback> callbackList;
private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    static ThemeManager* instance;
    static std::once_flag onceFlag;
};

#endif // THEMEMANAGER_H

