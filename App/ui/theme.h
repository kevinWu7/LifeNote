#ifndef THEME_H
#define THEME_H

#include <map>
#include <QString>

#define DARK_BACKGROUND "rgb(43,44,45)"
#define LIGHT_BACKGROUND "rgb(255,255,255)"
#define BLUE_BACKGROUND "rgb(83,107,174)"

extern std::map<QString, QString> themeDark;
extern std::map<QString, QString> themeLight;
extern std::map<QString, QString> themeBlue;


#endif // THEME_H
