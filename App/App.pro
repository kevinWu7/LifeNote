QT       += core gui svg xml help


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
ICON = logo.icns
INCLUDEPATH += \
    $$PWD/controls \
    $$PWD/windows \
    $$PWD/base \
    $$PWD/util

SOURCES += \
    controls/extraqtreewidgetitem.cpp \
    main.cpp \
    util/logger.cpp \
    windows/newnotegroupform.cpp \
    windows/mainwindow.cpp \
    nodeconfig.cpp \
    controls/svgtextlabel.cpp \
    util/util.cpp

HEADERS += \
    base/baseinfo.h \
    controls/extraqtreewidgetitem.h \
    util/logger.h \
    windows/newnotegroupform.h \
    windows/mainwindow.h \
    nodeconfig.h \
    controls/svgtextlabel.h \
    util/util.h

FORMS += \
    windows/newnotegroupform.ui \
    windows/mainwindow.ui \
    controls/svgtextlabel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
# 配置file_copies
CONFIG += file_copies

# 创建transfer自定义变量
# 配置需要复制的文件或目录(支持通配符)
transfer.files = $$PWD/config #$$PWD表示工程源代码所在目录
# 配置需要复制的目标目录, $$OUT_PWD含义为程序输出目录
transfer.path = $$OUT_PWD/App.app/Contents/MacOS

# 配置COPIES
COPIES += transfer

DISTFILES += \
    todoList.txt

