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
    void InitCurrentDateAndTimer();
    QDate currentDate;
    HabitItem *currentHabit=nullptr;

public slots:
    void addItemBtn_clicked();
    void onRightMenuRequested(const QPoint &pos);
    void onMenuEdit();
    void onMenuDelete();
    void setSelectedHabit(HabitItem *habit);

private:
    Ui::checkinWidget *ui;
    void onReceiveNewHabitFormData(QString name, int iconIndex,int formMode);
    void initHabitRightMenu();
    HabitItem* addHabitItem(project_info *project);
    void onReceiveHabitMousePressed(HabitItem *habit);
    void timerOutTriggered();
    QAction * editHabitAction;
    QAction * deleteHabitAction;
    QMenu *rightHabitMenu;
    QTimer timer;
    themeChangedCallback bindFunctionOfreceiveThemeChanged;
    void receiveThemeChanged();
};

#endif // CHECKINWIDGET_H
