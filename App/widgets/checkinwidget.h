#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>
#include <QTimer>
#include "habititem.h"
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
    HabbitItem *currentHabit=nullptr;

public slots:
    void addItemBtn_clicked();
    void onRightMenuRequested(const QPoint &pos);
    void onMenuEdit();
    void onMenuDelete();

private:
    Ui::checkinWidget *ui;
    void onReceiveNewHabitFormData(QString name, int iconIndex,int formMode);
    void initHabitRightMenu();
    HabbitItem* addHabitItem(project_info *project);
    void onReceiveHabitMousePressed(HabbitItem *habit);
    void timerOutTriggered();
    QAction * editHabitAction;
    QAction * deleteHabitAction;
    QMenu *rightHabitMenu;
    QTimer timer;
};

#endif // CHECKINWIDGET_H
