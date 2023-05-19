#include <QApplication>
#include "mainwindow.h"
#include "thememanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.setWindowTitle(WINDOW_TITLE);
    ThemeManager::getInstance().switchTheme("dark",true);
    window.show();
    return a.exec();
}
