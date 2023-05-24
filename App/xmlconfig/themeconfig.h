#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <mutex>
#include <QString>

#define THEME_CONFIG_PATH "/config/theme.xml"
class themeConfig
{
public:
    static themeConfig& getInstance();
    QString LoadThemeConfig();
    void updateXml(QString themeId);
private:
    themeConfig();
    themeConfig(const themeConfig&) = delete;
    themeConfig& operator=(const themeConfig&) = delete;

    static themeConfig* instance;
    static std::once_flag onceFlag;
};

#endif // THEMECONFIG_H


