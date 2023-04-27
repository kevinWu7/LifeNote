#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QToolButton>
#include "baseinfo.h"
#include "habbititem.h"
#include "newhabitform.h"


struct project_info
{
    QString project_name;
    QToolButton * button;
    QString iconIndex;
    QString type;
};

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
    std::vector<project_info*> LoadCheckinConfig();
    void updateXml(HabitOperationType type,project_info * project);
    void addHabitItem(project_info *project);
    void onReceiveHabitMousePressed(HabbitItem *habit,QWidget *weekWidget);
};

#endif // CHECKINWIDGET_H
