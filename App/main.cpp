#include <QApplication>
#include "mainwindow.h"
#include "util.h"
#include "theme.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(QCoreApplication::applicationDirPath()+ "/qss/dark.qss");
    if (!f.exists())
    {
        qDebug() << "Unable to set stylesheet, file not found";
    }
    else
    {
        util::isThemeDark=true;
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        QString allstyle=ts.readAll();
        for(auto item : themeBlue)
        {
            allstyle= allstyle.replace(item.first,item.second);
        };
        qApp->setStyleSheet(allstyle);
    }
    MainWindow w;
    w.setWindowTitle(WINDOW_TITLE);

    w.show();
    return a.exec();
}
