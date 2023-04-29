#ifndef MONTHBUTTON_H
#define MONTHBUTTON_H

#include "checkinconfig.h"
#include <QDate>
#include <QToolButton>


class monthButton : public QToolButton
{
public:
     monthButton(QWidget *parent = nullptr,bool m_iconState=false);
     QDate date;//记录当前的日期
     QDate currendDisplayDate; //保存当前页面的月份第一天
     void setMonthButtonClicked();
     void receiveBtnChecked(checkin_dateitem* dateItem);
private:
     void monthButton_clicked();
     bool iconState=false;


};

#endif // MONTHBUTTON_H
