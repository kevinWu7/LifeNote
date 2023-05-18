#include <QApplication>
#include "mainwindow.h"
#include "theme.h"
#include "ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle(WINDOW_TITLE);
     ThemeManager::getInstance().switchTheme("dark",true);
    w.show();
    return a.exec();
}
