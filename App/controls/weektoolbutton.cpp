#include "weektoolbutton.h"
#include<QWidget>


WeekToolButton::WeekToolButton(QWidget *parent)
      : QToolButton(parent)
{
     connect(this,&QToolButton::clicked,this,&WeekToolButton::CheckinButton_clicked);
}


void WeekToolButton::CheckinButton_clicked()
{
     logger->log(QString("clikced"));//todo
}
