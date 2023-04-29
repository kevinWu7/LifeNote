#include <QWidget>
#include "weektoolbutton.h"
#include "calendarcentral.h"


WeekToolButton::WeekToolButton(QWidget *parent,bool m_iconState)
      : QToolButton(parent)
{
     connect(this,&QToolButton::clicked,this,&WeekToolButton::WeekButton_clicked);
     this->setIconSize(QSize(24,24));
     iconState=m_iconState;
}




void WeekToolButton::WeekButton_clicked()
{
    if(iconState)
    {
        this->setIcon(QIcon());
    }
    else
    {
        this->setIcon(QIcon(":/icons/res/checkin/tick.png"));
    }
    iconState=!iconState;
    checkin_dateitem *item =new checkin_dateitem;
    item->date=date;
    item->tips="";
    item->ischecked=iconState;
    item->project_name=project_name;
    if(iconState)
    {
        CheckinConfig::getInstance().updateDetailXml(CheckinAction,item);
    }
    else
    {
        CheckinConfig::getInstance().updateDetailXml(CheckOutAction,item);
    }
    CalendarCentral::getInstance().triggerGlobalEvent(item);
}

void WeekToolButton::setWeekButtonClicked()
{
    this->setIcon(QIcon(":/icons/res/checkin/tick.png"));
    iconState=true;
}


