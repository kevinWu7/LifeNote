#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>
#include <QTimer>
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
    void InitCurrentDate();
    QDate currentDate;

public slots:
    void addItemBtn_clicked();

private:
    Ui::checkinWidget *ui;
    void onReceiveNewHabitFormData(QString name, int iconIndex);

    HabbitItem* addHabitItem(project_info *project);
    void onReceiveHabitMousePressed(HabbitItem *habit);
    void timerOutTriggered();
    QTimer timer;
};

#endif // CHECKINWIDGET_H
