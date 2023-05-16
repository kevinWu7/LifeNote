#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H
#include <mutex>
#include <vector>
#include <QString>

typedef std::function<void()> themeChangedCallback;
class ThemeManager
{
public:
    static ThemeManager& getInstance();
    // 注册全局事件
    void registerThemeGlobalEvent(themeChangedCallback callback);
    void unregisterThemeGlobalEvent(themeChangedCallback callback);
    void triggerThemeGlobalEvent();
    std::vector<themeChangedCallback> callbackList ;
private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    static ThemeManager* instance;
    static std::once_flag onceFlag;
};

#endif // THEMEMANAGER_H

