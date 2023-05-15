#include <QApplication>
#include "mainwindow.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(WINDOW_TITLE);

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
        qApp->setStyleSheet(ts.readAll());
    }
    w.show();
    return a.exec();
}
