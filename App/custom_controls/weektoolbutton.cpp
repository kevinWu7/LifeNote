#include <QWidget>
#include "weektoolbutton.h"
#include "calendarcentral.h"
#include "roundedtooltiphelper.h"


WeekToolButton::WeekToolButton(QWidget *parent,bool _ischecked)
      : QToolButton(parent)
{
     connect(this,&QToolButton::clicked,this,&WeekToolButton::WeekButton_clicked);
     initBaseStyleSheet();
     updateSizeStyle(16);
     isChecked=_ischecked;
     // 绑定成员函数到实例
     bindFunctionOfreceiveBtnChecked = std::bind(&WeekToolButton::receiveBtnChecked, this, std::placeholders::_1);
     // 注册全局事件
     CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

void WeekToolButton::initBaseStyleSheet()
{
       baseStyleSheet = QString("QToolButton:hover {border:none; background-color: rgb(210, 210, 215); }"
                                "QToolButton{"
                                "border: none;"
                                "background-color: rgb(230,230,235);"
                                "padding: 0;"
                                "margin: 0;}");
};

void WeekToolButton::updateSizeStyle(int size)
{
      QString sizeStyleSheet = QString("QToolButton{"
                                       "width: %1px; min-width: %1px; max-width: %1px;"
                                       "height: %1px; min-height: %1px; max-height: %1px;"
                                       "border-radius: %2px;}")
                                       .arg(QString::number(size),QString::number(size/2));
      setStyleSheet(baseStyleSheet + sizeStyleSheet);
      this->setIconSize(QSize(size,size));
}

void WeekToolButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->sender==0 ||dateItem->project_name!=project_name)
    {
        return;
    }
    if(dateItem->date== currentDate)
    {
       setWeekButtonClicked(dateItem->ischecked);
    }
}

void WeekToolButton::WeekButton_clicked()
{
    setWeekButtonClicked(!isChecked);
    checkin_dateitem *item =new checkin_dateitem;
    item->date=currentDate;
    item->tips="";
    item->ischecked=isChecked;
    item->project_name=project_name;
    item->sender=0;
    if(isChecked)
    {
        CheckinConfig::getInstance().updateDetailXml(CheckinAction,item);
    }
    else
    {
        CheckinConfig::getInstance().updateDetailXml(CheckOutAction,item);
    }
    CalendarCentral::getInstance().triggerGlobalEvent(item);
}

void WeekToolButton::setDate(const QDate &_date)
{
    currentDate=_date;
    bool isToday=_date==QDate::currentDate();
    if(isToday)
    {
        this->updateSizeStyle(20);
    }
    else
    {
        this->updateSizeStyle(16);
    }
}

const QDate& WeekToolButton::getDate()
{
    return currentDate;
}


//仅仅改变ui状态
void WeekToolButton::setWeekButtonClicked(bool _ischecked)
{
    if(_ischecked)
    {
         this->setIcon(QIcon(":/icons/res/checkin/tick.png"));
    }
    else
    {
         this->setIcon(QIcon());
    }
    isChecked=_ischecked;
}

WeekToolButton::~WeekToolButton()
{
    CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
    RoundedToolTipHelper::installHelper(this);
}


