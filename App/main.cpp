#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"
#include "theme.h"
#include "themeconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //QApplication::setApplicationDisplayName(WINDOW_TITLE);
    auto themeData=themeConfig::getInstance().LoadThemeConfig();
    ThemeManager::getInstance().ThemeId=themeData.themeId;
    ThemeManager::getInstance().Transparency=themeData.transparency;
    MainWindow window;
    window.setWindowTitle(WINDOW_TITLE);
    window.setWindowOpacity(ThemeManager::getInstance().Transparency/100.0);
    ThemeManager::getInstance().switchTheme(ThemeManager::getInstance().ThemeId,
                  diyThemeColor[ThemeManager::getInstance().ThemeId],true);
    window.show();
    return a.exec();
}
