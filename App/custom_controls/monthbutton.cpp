#include "monthbutton.h"

monthButton::monthButton(QWidget *parent ,bool m_iconState)
    : QToolButton(parent)
{
    connect(this,&QToolButton::clicked,this,&monthButton::monthButton_clicked);
    this->setIconSize(QSize(24,24));
    iconState=m_iconState;
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
