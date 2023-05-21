#ifndef THEME_H
#define THEME_H

#include <map>
#include <QString>
#include <unordered_map>

#define DARK_BACKGROUND "rgb(43,44,45)"
#define LIGHT_BACKGROUND "rgb(255,255,255)"
#define BLUE_BACKGROUND "rgb(83,107,174)"

#define RED1_BACKGROUND "rgb(248,211,210)";
#define RED2_BACKGROUND "rgb(243,167,165)";
#define RED3_BACKGROUND "rgb(238,125,122)";
#define RED4_BACKGROUND "rgb(236,98,93)";
#define RED5_BACKGROUND "rgb(235,79,71)";
#define RED6_BACKGROUND "rgb(189,62,56)";
#define RED7_BACKGROUND "rgb(117,49,46)";
#define RED8_BACKGROUND "rgb(71,35,34)";

#define GREEN1_BACKGROUND "rgb(203,241,213)";
#define GREEN2_BACKGROUND "rgb(168,240,185)";
#define GREEN3_BACKGROUND "rgb(130,226,150)";
#define GREEN4_BACKGROUND "rgb(106,213,118)";
#define GREEN5_BACKGROUND "rgb(95,201,100)";
#define GREEN6_BACKGROUND "rgb(69,151,74)";
#define GREEN7_BACKGROUND "rgb(61,125,69)";
#define GREEN8_BACKGROUND "rgb(45,75,51)";


extern std::map<QString, QString> themeDark;
extern std::map<QString, QString> themeLight;
extern std::map<QString, QString> themeBlue;
extern std::map<QString, QString> diyThemeColor;


#endif // THEME_H
