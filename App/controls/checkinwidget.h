#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>

namespace Ui {
class checkinWidget;
}

class checkinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit checkinWidget(QWidget *parent = nullptr);
    ~checkinWidget();

private:
    Ui::checkinWidget *ui;
};

#endif // CHECKINWIDGET_H
