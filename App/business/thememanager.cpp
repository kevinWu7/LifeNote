#include <mutex>
#include "thememanager.h"


ThemeManager* ThemeManager::instance = nullptr;

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
