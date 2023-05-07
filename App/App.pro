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
    $$PWD/util

SOURCES += \
    business/calendarcentral.cpp \
    custom_controls/monthbutton.cpp \
    custom_controls/extraqtreewidgetitem.cpp \
    custom_controls/lntreewidget.cpp \
    custom_controls/lntextedit.cpp \
    custom_controls/weektoolbutton.cpp \
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
    xmlconfig/noteconfig.cpp \
    xmlconfig/checkinconfig.cpp

HEADERS += \
    business/calendarcentral.h \
    custom_controls/monthbutton.h \
    custom_controls/extraqtreewidgetitem.h \
    custom_controls/lntreewidget.h \
    custom_controls/lntextedit.h \
    custom_controls/weektoolbutton.h \  \
    widgets/habititem.h \
    xmlconfig/noteconfig.h \
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
    xmlconfig/noteconfig.h \
    xmlconfig/checkinconfig.h

FORMS += \
    widgets/habititem.ui \
    widgets/recorditem.ui \
    widgets/calendarcontrol.ui \
    widgets/checkinwidget.ui \
    widgets/texteditcontainer.ui \
    windows/newhabitform.ui \
    windows/mainwindow.ui \
    windows/newnotegroupform.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
# 配置file_copies
CONFIG += file_copies

# 创建 transfer 自定义变量
# 配置需要复制的文件或目录（支持通配符）
transfer.files = $$PWD/config # $$PWD 表示工程源代码所在目录

# 配置需要复制的目标目录，$$OUT_PWD 含义为程序输出目录
win32 {
    transfer.path = $$OUT_PWD/debug
}
macx {
    transfer.path = $$OUT_PWD/App.app/Contents/MacOS
}

# 配置 COPIES
COPIES += transfer



# 配置COPIES
COPIES += transfer

DISTFILES += \
    todoList.txt

