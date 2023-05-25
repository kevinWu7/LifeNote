#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <mutex>
#include <QString>

#define THEME_CONFIG_PATH "/config/theme.xml"

struct themeData
{
    QString themeId;
    double transparency;
};

class themeConfig
{
public:
    static themeConfig& getInstance();
    themeData LoadThemeConfig();
    void updateXml(const QString& fieldType,const QString& value);
private:
    themeConfig();
    themeConfig(const themeConfig&) = delete;
    themeConfig& operator=(const themeConfig&) = delete;

    static themeConfig* instance;
    static std::once_flag onceFlag;
};

#endif // THEMECONFIG_H


