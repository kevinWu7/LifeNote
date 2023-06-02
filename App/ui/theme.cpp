#include "theme.h"

std::map<QString,QString> currentTheme={

};

std::map<QString,QString> themeDark={
    {"BACKGROUND_COLOR1","rgb(43,44,45)"},
    {"LEFT_BACKGROUND_COLOR1","rgb(43,44,45)"},
    {"RIGHT_BACKGROUND_COLOR1","rgb(43,44,45)"},
    {"BACKGROUND_COLOR2", "rgb(63,62,58)"},
    {"CONTROL_SELECTED" , "rgb(72,72,72)"},
    {"CONTROL_NOSELECTED", "rgb(62,62,62)"},
    {"CONTROL_TEXT","rgb(225,225,225)"},
    {"CONTROL_WARNINGTEXT","rgb(255,255,255)"}, //警告色
    {"CONTROL_POOLTEXT","rgb(160,160,160)"}, //浅文字
    {"SINGLE_LINE_COLOR" ,"rgb(72,72,72)"},
    {"SCROLLBAR_HANDLE" ,"rgba(86,86,86,80%)"},
    {"SCROLLBAR_HOVER" ,"rgb(86,86,86)"},
    {"PICTURE_PATH","none"}
};

std::map<QString,QString> themeLight={
    {"BACKGROUND_COLOR1","rgb(255,255,255)"},
    {"LEFT_BACKGROUND_COLOR1","rgb(255,255,255)"},
    {"RIGHT_BACKGROUND_COLOR1","rgb(255,255,255)"},
    {"BACKGROUND_COLOR2", "rgb(236,236,236)"},
    {"CONTROL_SELECTED" , "rgb(213, 230, 253)"},
    {"CONTROL_NOSELECTED", "rgb(232,232,232)"},
    {"CONTROL_TEXT", "rgb(30, 30, 30)"},
    {"CONTROL_WARNINGTEXT","rgb(0,0,0)"}, //警告色
    {"CONTROL_POOLTEXT","rgb(180,180,180)"}, //浅文字
    {"SINGLE_LINE_COLOR","rgb(239, 239, 239)"},
    {"SCROLLBAR_HANDLE" ,"rgba(0,0,0,25%)"},
    {"SCROLLBAR_HOVER" ,"rgba(0,0,0,15%)"},
    {"PICTURE_PATH","none"}
};

//还需要一个定制化的CONTROL_SELECTED的控制
//以及各个主题色的定制化需求

//控制哪个主题开始，浅深切换
std::map<QString, int> diyThemeIndex= {
    {"Red", 2},
    {"Green", 4},
    {"TintBlue", 4},
    {"Blue", 2},
    {"Cyan", 6},
    {"Yellow", 4},
    {"TintYellow", 4},
    {"Pink",3},
    {"Purple", 4},
    {"Gray", 2},
};

std::map<QString, QString> diyThemeColor = {
    {"Red1", "rgb(248,211,210)"},
    {"Red2", "rgb(243,167,165)"},
    {"Red3", "rgb(228,150,150)"},
    {"Red4", "rgb(218,120,93)"},
    {"Red5", "rgb(189,80,56)"},
    {"Red6", "rgb(170,81,78)"},
    {"Red7", "rgb(117,49,46)"},
    {"Red8", "rgb(71,35,34)"},
    {"Green1", "rgb(203,241,213)"},
    {"Green2", "rgb(196,212,151)"},
    {"Green3", "rgb(168,240,185)"},
    {"Green4", "rgb(130,226,150)"},
    {"Green5", "rgb(155,182,84)"},
    {"Green6", "rgb(81,96,34)"},
    {"Green7", "rgb(61,125,69)"},
    {"Green8", "rgb(45,75,51)"},
    {"TintBlue1", "rgb(215,235,253)"},
    {"TintBlue2", "rgb(172,216,251)"},
    {"TintBlue3", "rgb(136,196,250)"},
    {"TintBlue4", "rgb(120,177,249)"},
    {"TintBlue5", "rgb(76,158,248)"},
    {"TintBlue6", "rgb(61,128,198)"},
    {"TintBlue7", "rgb(43,85,124)"},
    {"TintBlue8", "rgb(40,58,74)"},
    {"Blue1", "rgb(192,200,236)"},
    {"Blue2", "rgb(155,160,233)"},
    {"Blue3", "rgb(112,121,231)"},
    {"Blue4", "rgb(83,94,230)"},
    {"Blue5", "rgb(61,72,230)"},
    {"Blue6", "rgb(48,57,181)"},
    {"Blue7", "rgb(45,50,122)"},
    {"Blue8", "rgb(34,37,73)"},
    {"Cyan1", "rgb(202,238,241)"},
    {"Cyan2", "rgb(166,235,240)"},
    {"Cyan3", "rgb(128,219,227)"},
    {"Cyan4", "rgb(106,205,214)"},
    {"Cyan5", "rgb(90,195,207)"},
    {"Cyan6", "rgb(76,166,176)"},
    {"Cyan7", "rgb(53,108,113)"},
    {"Cyan8", "rgb(45,73,75)"},
    {"Yellow1", "rgb(254,245,208)"},
    {"Yellow2", "rgb(253,237,163)"},
    {"Yellow3", "rgb(251,227,122)"},
    {"Yellow4", "rgb(198,167,55)"},
    {"Yellow5", "rgb(203,163,73)"},
    {"Yellow6", "rgb(178,142,60)"},
    {"Yellow7", "rgb(125,108,42)"},
    {"Yellow8", "rgb(76,67,38)"},
    {"TintYellow1", "rgb(236,220,199)"},
    {"TintYellow2", "rgb(214,189,157)"},
    {"TintYellow3", "rgb(212,176,127)"},
    {"TintYellow4", "rgb(212,162,100)"},
    {"TintYellow5", "rgb(207,141,65)"},
    {"TintYellow6", "rgb(165,111,51)"},
    {"TintYellow7", "rgb(113,76,35)"},
    {"TintYellow8", "rgb(67,52,33)"},
    {"Pink1", "rgb(213,179,205)"},
    {"Pink2", "rgb(196,146,182)"},
    {"Pink3", "rgb(200,120,180)"},
    {"Pink4", "rgb(180,90,155)"},
    {"Pink5", "rgb(190,65,150)"},
    {"Pink6", "rgb(150,50,120)"},
    {"Pink7", "rgb(102,43,84)"},
    {"Pink8", "rgb(60,30,52)"},
    {"Purple1", "rgb(219,198,243)"},
    {"Purple2", "rgb(200,170,236)"},
    {"Purple3", "rgb(182,143,230)"},
    {"Purple4", "rgb(164,118,225)"},
    {"Purple5", "rgb(141,81,219)"},
    {"Purple6", "rgb(111,62,170)"},
    {"Purple7", "rgb(86,58,122)"},
    {"Purple8", "rgb(52,33,73)"},
    {"Gray1", "rgb(207,208,209)"},
    {"Gray2", "rgb(156,156,158)"},
    {"Gray3", "rgb(110,110,114)"},
    {"Gray4", "rgb(85,87,92)"},
    {"Gray5", "rgb(64,65,70)"},
    {"Gray6", "rgb(58,59,64)"},
    {"Gray7", "rgb(42,43,46)"},
    {"Gray8", "rgb(37,38,42)"},
};
