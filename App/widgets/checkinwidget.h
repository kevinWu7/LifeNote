#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>
#include "baseinfo.h"
#include "habbititem.h"
#include "newhabitform.h"
#include "checkinconfig.h"


namespace Ui {
class checkinWidget;
}

class checkinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit checkinWidget(QWidget *parent = nullptr);
    ~checkinWidget();
    NewHabitForm *habitForm=nullptr;


public slots:
    void addItemBtn_clicked();

private:
    Ui::checkinWidget *ui;
    void onReceiveNewHabitFormData(QString name, int iconIndex);

    HabbitItem* addHabitItem(project_info *project);
    void onReceiveHabitMousePressed(HabbitItem *habit);
};

#endif // CHECKINWIDGET_H
