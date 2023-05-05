#include "monthbutton.h"
#include "calendarcentral.h"


monthButton::monthButton(QWidget *parent ,bool m_iconState)
    : QToolButton(parent)
{
    connect(this,&QToolButton::clicked,this,&monthButton::monthButton_clicked);
    this->setIconSize(QSize(24,24));
    iconState=m_iconState;
    // 绑定成员函数到实例
    bindFunctionOfreceiveBtnChecked = std::bind(&monthButton::receiveBtnChecked, this, std::placeholders::_1);
    // 注册全局事件
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

void monthButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->sender==1||dateItem->project_name!=project_name)
    {
        return;
    }
    if(dateItem->date== date)
    {
        setMonthButtonClicked(dateItem->ischecked);
    }
}



void monthButton::monthButton_clicked()
{
    monthButton *button = dynamic_cast<monthButton*>(sender());
    QDate currentDate=QDate::currentDate();
    if(date==currentDate)
    {
        if(iconState)
        {
            button->setStyleSheet("background-color:rgb(239,239,239);color:orange;font-weight: bold;");
        }
        else
        {
            button->setStyleSheet("background-color:rgb(72,114,251);color:orange;font-weight: bold;");
        }
    }
    else
    {
        if(iconState)
        {
            if(date.month()!= currendDisplayDate.month())
            {
                button->setStyleSheet("background-color:rgb(239,239,239);color:rgb(158,158,158);");
            }
            else
            {
                button->setStyleSheet("background-color:rgb(239,239,239);color:black;");
            }
        }
        else
        {
           if(date.month()!= currendDisplayDate.month())
           {
                button->setStyleSheet("background-color:rgba(72,114,251,0.6);color:white;");
           }
           else
           {
                button->setStyleSheet("background-color:rgb(72,114,251);color:white;");
           }
        }
    }
    iconState=!iconState;

    checkin_dateitem *item =new checkin_dateitem;
    item->date=date;
    item->tips="";
    item->ischecked=iconState;
    item->sender=1;
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

//仅仅改变ui状态
void monthButton::setMonthButtonClicked(bool ischeck)
{
    if(!ischeck)
    {
        if(date.month()!= currendDisplayDate.month())
        {
            this->setStyleSheet("background-color:rgb(239,239,239);color:rgb(158,158,158);");
        }
        else
        {
            this->setStyleSheet("background-color:rgb(239,239,239);color:black;");
        }
    }
    else
    {
       if(date.month()!= currendDisplayDate.month())
       {
            this->setStyleSheet("background-color:rgba(72,114,251,0.6);color:white;");
       }
       else
       {
            this->setStyleSheet("background-color:rgb(72,114,251);color:white;");
       }
    }
    iconState=ischeck;
}

monthButton::~monthButton()
{
    // 注销全局事件
    CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

