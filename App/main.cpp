#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"
#include "themeconfig.h"
#include "external/export_api_sniper_sdk.h"
#include "firstinit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool isInit=firstinit::getIsFirstInit();
   // if(!isInit)
    {
       //此句代码会阻塞，直到获取发送get_all_File后，返回消息。
       connect_to_server_async_export("127.0.0.1","18889",&MainWindow::connected_success);
     // firstinit::updateInitNode(true);
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

