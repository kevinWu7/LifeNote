#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"
#include "theme.h"
#include "themeconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //QApplication::setApplicationDisplayName(WINDOW_TITLE);
    ThemeManager::getInstance().ThemeId=themeConfig::getInstance().LoadThemeConfig();
    MainWindow window;
    window.setWindowTitle(WINDOW_TITLE);

    ThemeManager::getInstance().switchTheme(ThemeManager::getInstance().ThemeId,
                  diyThemeColor[ThemeManager::getInstance().ThemeId],true);
    window.show();
    return a.exec();
}
