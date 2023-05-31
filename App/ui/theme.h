#ifndef THEME_H
#define THEME_H

#include <map>
#include <QString>

extern std::map<QString, QString> currentTheme;

extern std::map<QString, QString> themeDark;
extern std::map<QString, QString> themeLight;
extern std::map<QString, QString> diyThemeColor;
extern std::map<QString,int> diyThemeIndex; //各个色系的分界点汇总

#endif // THEME_H
