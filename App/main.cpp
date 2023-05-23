#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"
#include "theme.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //QApplication::setApplicationDisplayName(WINDOW_TITLE);
    MainWindow window;
    window.setWindowTitle(WINDOW_TITLE);
    ThemeManager::getInstance().switchTheme("dark",themeDark["BACKGROUND_COLOR1"],true);
    window.show();
    return a.exec();
}
