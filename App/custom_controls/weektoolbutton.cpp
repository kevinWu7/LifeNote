#include <QWidget>
#include "weektoolbutton.h"
#include "calendarcentral.h"


WeekToolButton::WeekToolButton(QWidget *parent,bool m_iconState)
      : QToolButton(parent)
{
     connect(this,&QToolButton::clicked,this,&WeekToolButton::WeekButton_clicked);
     this->setIconSize(QSize(24,24));
     iconState=m_iconState;
     // 绑定成员函数到实例
     auto bindFunctionOfreceiveBtnChecked = std::bind(&WeekToolButton::receiveBtnChecked, this, std::placeholders::_1);
     // 注册全局事件
     CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}



void WeekToolButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->sender==0 ||dateItem->project_name!=project_name)
    {
        return;
    }
    if(dateItem->date== date)
    {
       setWeekButtonClicked(dateItem->ischecked);
    }
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
    item->sender=0;
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

//仅仅改变ui状态
void WeekToolButton::setWeekButtonClicked(bool ischecked)
{
    if(ischecked)
    {
         this->setIcon(QIcon(":/icons/res/checkin/tick.png"));
    }
    else
    {
         this->setIcon(QIcon());
    }
    iconState=ischecked;
}


