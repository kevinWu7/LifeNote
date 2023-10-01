#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"
#include "themeconfig.h"
#include "external/sniper_sdk_api.h"
#include "firstinit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool isInit=firstinit::getIsFirstInit();
    if(!isInit)
    {
        start_client_export();
        firstinit::updateInitNode(true);
    }
    auto themeData=themeConfig::getInstance().LoadThemeConfig();
    ThemeManager::getInstance().ThemeId=themeData.themeId;
    ThemeManager::getInstance().PictureThemeId=themeData.picture_themeId;
    ThemeManager::getInstance().Transparency=themeData.transparency;
    ThemeManager::getInstance().LeftTransparency=themeData.leftTransparency;
    ThemeManager::getInstance().RightTransparency=themeData.rightTransparency;
    ThemeManager::getInstance().switchTheme(ThemeManager::getInstance().ThemeId,ThemeManager::getInstance().PictureThemeId,true);
    MainWindow window;
    window.setWindowTitle("");
    window.setWindowOpacity(ThemeManager::getInstance().Transparency/100.0);
    window.show();
    return a.exec();
}

