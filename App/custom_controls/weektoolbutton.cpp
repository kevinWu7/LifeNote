#include "weektoolbutton.h"
#include<QWidget>


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
}
