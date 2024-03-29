QT       += core gui svg xml help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
ICON = logo.icns

INCLUDEPATH += \
    $$PWD/custom_controls \
    $$PWD/widgets \
    $$PWD/windows \
    $$PWD/base \
    $$PWD/xmlconfig \
    $$PWD/business \
    $$PWD/util \
    $$PWD/ui \
    $$PWD/external

SOURCES += \
    business/calendarcentral.cpp \
    business/roundedtooltiphelper.cpp \
    business/thememanager.cpp \
    custom_controls/monthbutton.cpp \
    custom_controls/extraqtreewidgetitem.cpp \
    custom_controls/lntreewidget.cpp \
    custom_controls/lntextedit.cpp \
    custom_controls/weektoolbutton.cpp \
    custom_controls/roundedtooltip.cpp \
    custom_controls/lnslider.cpp \
    ui/theme.cpp \
    widgets/themeswitchwidget.cpp \
    widgets/titlebarwidget.cpp \
    widgets/habititem.cpp \
    widgets/recorditem.cpp \
    widgets/calendarcontrol.cpp \
    widgets/checkinwidget.cpp \
    widgets/texteditcontainer.cpp \
    windows/newnotegroupform.cpp \
    windows/mainwindow.cpp \
    windows/newhabitform.cpp \
    main.cpp \
    util/logger.cpp \
    util/util.cpp \
    xmlconfig/firstinit.cpp \
    xmlconfig/noteconfig.cpp \
    xmlconfig/themeconfig.cpp \
    xmlconfig/checkinconfig.cpp

HEADERS += \
    business/calendarcentral.h \
    business/roundedtooltiphelper.h \
    business/thememanager.h \
    custom_controls/monthbutton.h \
    custom_controls/extraqtreewidgetitem.h \
    custom_controls/lntreewidget.h \
    custom_controls/lntextedit.h \
    custom_controls/weektoolbutton.h \  \
    custom_controls/roundedtooltip.h \
    custom_controls/lnslider.h \
    ui/theme.h \
    widgets/themeswitchwidget.h \
    widgets/titlebarwidget.h \
    widgets/habititem.h \
    widgets/recorditem.h \
    widgets/calendarcontrol.h \
    widgets/checkinwidget.h \  
    widgets/texteditcontainer.h \
    windows/mainwindow.h \
    windows/newnotegroupform.h \
    windows/newhabitform.h \
    base/baseinfo.h \
    util/util.h \
    util/logger.h \
    xmlconfig/firstinit.h \
    xmlconfig/noteconfig.h \
    xmlconfig/themeconfig.h \
    xmlconfig/checkinconfig.h \
    external/export_api_sniper_sdk.h

FORMS += \
    widgets/themeswitchwidget.ui \
    widgets/habititem.ui \
    widgets/recorditem.ui \
    widgets/calendarcontrol.ui \
    widgets/checkinwidget.ui \
    widgets/texteditcontainer.ui \
    windows/newhabitform.ui \
    windows/mainwindow.ui \
    windows/newnotegroupform.ui

macx {
LIBS +=   -framework AppKit  # 链接 Foundation 和 AppKit 框架
OBJECTIVE_SOURCES += \#引入object-C的类源文件
    titlebarcontroller.mm
    LIBS += -L$$PWD/libs/macx -lSniperSDK
}
win32 {
    LIBS += -L$$PWD/libs/windows -lSniperSDK
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# 启用文件拷贝
CONFIG += file_copies

win32 {
    # 在Windows下设置运行时路径
    CONFIG(release, debug|release):{
        OUTPUT_COMPILE_PATH= $$OUT_PWD/release
        QMAKE_RPATHDIR += $$OUT_PWD/release/libs
    } else {
        OUTPUT_COMPILE_PATH= $$OUT_PWD/debug
        QMAKE_RPATHDIR += $$OUT_PWD/debug/libs
    }
}
macx {
    # 设置运行时路径
    QMAKE_LFLAGS += -Wl,-rpath,@executable_path/../Frameworks
}

# 配置需要复制的文件或目录（支持通配符）
configtransfer.files = $$PWD/config # $$PWD 表示工程源代码所在目录
# 创建 qssTransfer 自定义变量
qssTransfer.files = $$PWD/qss # 这里设置你的 qss 文件夹路径

# 配置需要复制的目标目录，$$OUT_PWD 含义为程序输出目录
win32 {
     configtransfer.path =$$OUTPUT_COMPILE_PATH
     qssTransfer.path = $$OUTPUT_COMPILE_PATH
     libsTransfer.files = $$PWD/libs/windows
     libsTransfer.path =$$OUTPUT_COMPILE_PATH
}
macx {
     configtransfer.path = $$OUT_PWD/App.app/Contents/MacOS
     qssTransfer.path = $$OUT_PWD/App.app/Contents/MacOS
     libsTransfer.files = $$PWD/libs/macx/*.dylib
     libsTransfer.path = $$OUT_PWD/App.app/Contents/Frameworks
}



# 配置 COPIES
COPIES += configtransfer
COPIES += qssTransfer # 添加 qss 文件夹到拷贝列表
COPIES += libsTransfer




DISTFILES += \
    projectStruct.txt \
    qss/dark.qss \
    qss/light.qss \
    todoList.txt

#配置win图标
win32 {
    RC_ICONS +=logo_win.ico
}


