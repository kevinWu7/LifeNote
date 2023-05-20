#include "theme.h"



std::map<QString,QString> themeDark={
    {"BACKGROUND_COLOR1","rgb(43,44,45)"},
    {"BACKGROUND_COLOR2", "rgb(63,62,58)"},
    {"CONTROL_SELECTED" , "rgb(72,72,72)"},
    {"CONTROL_NOSELECTED", "rgb(62,62,62)"},
    {"CONTROL_TEXT","rgb(255,255,255)"},
    {"CONTROL_POOLTEXT","rgb(160,160,160)"}, //浅文字
    {"SINGLE_LINE_COLOR" ,"rgb(72,72,72)"}
};

std::map<QString,QString> themeBlue={
    {"BACKGROUND_COLOR1","rgb(83,107,174)"},
    {"BACKGROUND_COLOR2", "rgb(70,90,146)"},
    {"CONTROL_SELECTED" , "rgb(131,148,196)"},
    {"CONTROL_NOSELECTED", "rgb(101,122,182)"},
    {"CONTROL_TEXT",   "rgb(255,255,255)"},
    {"CONTROL_POOLTEXT","rgb(160,160,160)"}, //浅文字
    {"SINGLE_LINE_COLOR" ,"rgb(72,72,72)"}
};

std::map<QString,QString> themeLight={
    {"BACKGROUND_COLOR1","rgb(255,255,255)"},
    {"BACKGROUND_COLOR2", "rgb(236,236,236)"},
    {"CONTROL_SELECTED" , "rgb(213, 230, 253)"},
    {"CONTROL_NOSELECTED", "rgb(232,232,232)"},
    {"CONTROL_TEXT", "rgb(0, 0, 0)"},
    {"CONTROL_POOLTEXT","rgb(180,180,180)"}, //浅文字
    {"SINGLE_LINE_COLOR","rgb(239, 239, 239)"}
};


std::unordered_map<std::string, QString> diyThemeColor = {
    {"RED1_BACKGROUND", "rgb(248,211,210)"},
    {"RED2_BACKGROUND", "rgb(243,167,165)"},
    {"RED3_BACKGROUND", "rgb(238,125,122)"},
    {"RED4_BACKGROUND", "rgb(236,98,93)"},
    {"RED5_BACKGROUND", "rgb(235,79,71)"},
    {"RED6_BACKGROUND", "rgb(189,62,56)"},
    {"RED7_BACKGROUND", "rgb(117,49,46)"},
    {"RED8_BACKGROUND", "rgb(71,35,34)"},
    {"GREEN1_BACKGROUND", "rgb(203,241,213)"},
    {"GREEN2_BACKGROUND", "rgb(168,240,185)"},
    {"GREEN3_BACKGROUND", "rgb(130,226,150)"},
    {"GREEN4_BACKGROUND", "rgb(106,213,118)"},
    {"GREEN5_BACKGROUND", "rgb(95,201,100)"},
    {"GREEN6_BACKGROUND", "rgb(69,151,74)"},
    {"GREEN7_BACKGROUND", "rgb(61,125,69)"},
    {"GREEN8_BACKGROUND", "rgb(45,75,51)"}
};
