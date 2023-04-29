#include "monthbutton.h"
#include "calendarcentral.h"
#include "logger.h"

monthButton::monthButton(QWidget *parent ,bool m_iconState)
    : QToolButton(parent)
{
    connect(this,&QToolButton::clicked,this,&monthButton::monthButton_clicked);
    this->setIconSize(QSize(24,24));
    iconState=m_iconState;
    // 绑定成员函数到实例
    auto bindFunctionOfreceiveBtnChecked = std::bind(&monthButton::receiveBtnChecked, this, std::placeholders::_1);
    // 注册全局事件
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

void monthButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->date== date)
    {
        this->click();
    }
}

void monthButton::setMonthButtonClicked()
{
    if(date.month()!= currendDisplayDate.month())
    {
         this->setStyleSheet("background-color:rgba(72,114,251,0.6);color:white;");
    }
    else
    {
         this->setStyleSheet("background-color:rgb(72,114,251);color:white;");
    }
    iconState=true;
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
}
