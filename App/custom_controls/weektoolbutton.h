#ifndef WEEKTOOLBUTTON_H
#define WEEKTOOLBUTTON_H

#include <QDate>
#include <QToolButton>
#include "checkinconfig.h"


class WeekToolButton : public QToolButton
{
    Q_OBJECT
public:
    WeekToolButton(QWidget *parent = nullptr,bool m_iconState=false);
    void WeekButton_clicked();
    QDate date;
    QString project_name;
    void setWeekButtonClicked(bool ischecked);
signals:
    //void OnWeekButtonClicked(QDate date,bool ischecked);
private :
    bool iconState=false;
    void receiveBtnChecked(checkin_dateitem* dateItem);

};

#endif // WEEKTOOLBUTTON_H
