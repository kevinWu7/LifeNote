#include "theme.h"

std::map<QString,QString> currentTheme={

};

//注意！！！！
//由于qss为replace 替换，所以不能出现key存在包含关系，否则会发现意想不到的错误
//如一个key为：BACKGROUND_COLOR1，另一个key为：LEFT_BACKGROUND_COLOR1
//这样会出现LEFT_BACKGROUND_COLOR1被替换为了LEFT_rgb(43,44,45)
std::map<QString,QString> themeDark={
    {"BACKGROUND_COLOR1","rgb(0,0,0)"},
    {"LEFT_BACK_COLOR1","rgb(0,0,0)"},
    {"RIGHT_BACK_COLOR1","rgb(0,0,0)"},
    {"BACKGROUND_COLOR2", "rgb(63,62,58)"},
    {"CONTROL_SELECTED" , "rgba(255,255,255,0.3)"},
    {"CONTROL_NOSELECTED", "rgba(255,255,255,0.1)"},
    {"CONTROL_TEXT","rgb(255,255,255)"},
    {"CONTROL_WARNINGTEXT","rgb(255,255,255)"}, //警告色
    {"CONTROL_POOLTEXT","rgb(160,160,160)"}, //浅文字
    {"SINGLE_LINE_COLOR" ,"rgb(72,72,72)"},
    {"SCROLLBAR_HANDLE" ,"rgba(86,86,86,80%)"},
    {"SCROLLBAR_HOVER" ,"rgb(86,86,86)"},
    {"PICTURE_PATH","none"}
};

std::map<QString,QString> themeLight={
    {"BACKGROUND_COLOR1","rgb(255,255,255)"},
    {"LEFT_BACK_COLOR1","rgb(255,255,255)"},
    {"RIGHT_BACK_COLOR1","rgb(255,255,255)"},
    {"BACKGROUND_COLOR2", "rgb(236,236,236)"},
    {"CONTROL_SELECTED" , "rgba(0,0,0,0.15)"},
    {"CONTROL_NOSELECTED", "rgba(80,80,80,0.1)"},
    {"CONTROL_TEXT", "rgb(0, 0, 0)"},
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
    {"Red", 3},
    {"Green", 4},
    {"TintBlue", 4},
    {"Blue", 2},
    {"Cyan", 4},
    {"Yellow", 4},
    {"TintYellow", 4},
    {"Pink",2},
    {"Purple", 3},
    {"Gray", 1},
};

std::map<QString, QString> diyThemeColor = {
    {"Red1", "rgb(248,211,210)"},
    {"Red2", "rgb(240,180,185)"},
    {"Red3", "rgb(228,160,160)"},
    {"Red4", "rgb(208,110,83)"},
    {"Red5", "rgb(170,81,78)"},
    {"Red6", "rgb(117,49,46)"},
    {"Red7", "rgb(71,35,34)"},
    {"Green1", "rgb(203,241,213)"},
    {"Green2", "rgb(196,212,151)"},
    {"Green3", "rgb(168,240,185)"},
    {"Green4", "rgb(155,202,104)"},
    {"Green5", "rgb(81,96,34)"},
    {"Green6", "rgb(61,125,69)"},
    {"Green7", "rgb(45,75,51)"},
    {"TintBlue1", "rgb(215,235,253)"},
    {"TintBlue2", "rgb(172,216,251)"},
    {"TintBlue3", "rgb(136,196,250)"},
    {"TintBlue4", "rgb(120,177,249)"},
    {"TintBlue5", "rgb(61,128,198)"},
    {"TintBlue6", "rgb(43,85,124)"},
    {"TintBlue7", "rgb(40,58,74)"},
    {"Blue1", "rgb(192,200,236)"},
    {"Blue2", "rgb(155,160,233)"},
    {"Blue3", "rgb(112,121,231)"},
    {"Blue4", "rgb(83,94,230)"},
    {"Blue5", "rgb(45,60,161)"},
    {"Blue6", "rgb(45,50,122)"},
    {"Blue7", "rgb(34,37,73)"},
    {"Cyan1", "rgb(202,238,241)"},
    {"Cyan2", "rgb(166,235,240)"},
    {"Cyan3", "rgb(128,219,227)"},
    {"Cyan4", "rgb(106,205,214)"},
    {"Cyan5", "rgb(80,165,187)"},
    {"Cyan6", "rgb(53,108,113)"},
    {"Cyan7", "rgb(45,73,75)"},
    {"Yellow1", "rgb(254,245,208)"},
    {"Yellow2", "rgb(253,237,163)"},
    {"Yellow3", "rgb(208,190,82)"},
    {"Yellow4", "rgb(198,167,55)"},
    {"Yellow5", "rgb(178,142,60)"},
    {"Yellow6", "rgb(125,108,42)"},
    {"Yellow7", "rgb(76,67,38)"},
    {"TintYellow1", "rgb(236,220,199)"},
    {"TintYellow2", "rgb(214,189,157)"},
    {"TintYellow3", "rgb(212,176,127)"},
    {"TintYellow4", "rgb(212,162,100)"},
    {"TintYellow5", "rgb(165,111,51)"},
    {"TintYellow6", "rgb(113,76,35)"},
    {"TintYellow7", "rgb(67,52,33)"},
    {"Pink1", "rgb(213,179,205)"},
    {"Pink2", "rgb(206,156,192)"},
    {"Pink3", "rgb(200,120,180)"},
    {"Pink4", "rgb(180,90,155)"},
    {"Pink5", "rgb(150,50,120)"},
    {"Pink6", "rgb(102,43,84)"},
    {"Pink7", "rgb(60,30,52)"},
    {"Purple1", "rgb(219,198,243)"},
    {"Purple2", "rgb(202,172,238)"},
    {"Purple3", "rgb(192,153,230)"},
    {"Purple4", "rgb(164,118,225)"},
    {"Purple5", "rgb(111,62,170)"},
    {"Purple6", "rgb(86,58,122)"},
    {"Purple7", "rgb(52,33,73)"},
    {"Gray1", "rgb(207,208,209)"},
    {"Gray2", "rgb(156,156,158)"},
    {"Gray3", "rgb(110,110,114)"},
    {"Gray4", "rgb(85,87,92)"},
    {"Gray5", "rgb(64,65,70)"},
    {"Gray6", "rgb(58,59,64)"},
    {"Gray7", "rgb(42,43,46)"},
};
