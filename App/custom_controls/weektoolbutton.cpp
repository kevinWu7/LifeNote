#include <QWidget>
#include "weektoolbutton.h"
#include "checkinconfig.h"
#include "logger.h"


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
    emit OnWeekButtonClicked(this->date,iconState);

}

/*void WeekToolButton::OnWeekButtonClicked(QDate date)
{
    // 处理信号触发事件的逻辑
    logger->log(QString("triggerOnWeekButtonClicked"));
}
*/
