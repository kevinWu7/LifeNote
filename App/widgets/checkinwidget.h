#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include "habbititem.h"

namespace Ui {
class checkinWidget;
}

class checkinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit checkinWidget(QWidget *parent = nullptr);
    ~checkinWidget();

public slots:
    void addItemBtn_clicked();

private:
    Ui::checkinWidget *ui;
};

#endif // CHECKINWIDGET_H
