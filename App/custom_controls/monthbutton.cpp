#include "monthbutton.h"
#include "calendarcentral.h"


monthButton::monthButton(QWidget *parent ,bool _ischecked)
    : QToolButton(parent)
{
    connect(this,&QToolButton::clicked,this,&monthButton::monthButton_clicked);
    this->setIconSize(QSize(24,24));
    isChecked=_ischecked;
    // 绑定成员函数到实例
    bindFunctionOfreceiveBtnChecked = std::bind(&monthButton::receiveBtnChecked, this, std::placeholders::_1);
    // 注册全局事件
    CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
    initBaseStyleSheet();
    updateColorStyle("rgb(239,239,239)","black","normal");

}

void monthButton::initBaseStyleSheet()
{
       baseStyleSheet = QString(
                                "QToolButton{"
                                "border: 0px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "font-size: 12px;"
                                "padding: 0;"
                                "margin: 0;}").arg("32px","16px");
};

void monthButton::updateColorStyle(QString background,QString textColor,QString font_weight)
{
      QString colorStyleSheet = QString("QToolButton"
                                       "{background-color:%1;"
                                       "color:%2;"
                                       "font-weight: %3;}")
                                       .arg(background,textColor,font_weight);
      setStyleSheet(baseStyleSheet + colorStyleSheet);
}

void monthButton::setDate(const QDate &date)
{
    currentDate=date;
}

const QDate &monthButton::getDate()
{
    return currentDate;
}

void monthButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->sender==1||dateItem->project_name!=project_name)
    {
        return;
    }
    if(dateItem->date== currentDate)
    {
        setMonthButtonClicked(dateItem->ischecked);
    }
}



void monthButton::monthButton_clicked()
{
    setMonthButtonClicked(!isChecked);

    checkin_dateitem *item =new checkin_dateitem;
    item->date=currentDate;
    item->tips="";
    item->ischecked=isChecked;
    item->sender=1;
    item->project_name=project_name;

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

//仅仅改变ui状态
void monthButton::setMonthButtonClicked(bool _ischeck)
{
    QDate _currentDate=QDate::currentDate();
    if(currentDate==_currentDate)
    {
        if(!_ischeck)
        {
            updateColorStyle("rgb(239,239,239)","orange","bold");
        }
        else
        {
            updateColorStyle("rgb(72,114,251)","orange","bold");
        }
    }
    else
    {
        if(!_ischeck)
        {
            if(currentDate.month()!= currendDisplayDate.month())
            {
                updateColorStyle("rgb(239,239,239)","rgb(158,158,158)","normal");
            }
            else
            {
                updateColorStyle("rgb(239,239,239)","black","normal");
            }
        }
        else
        {
            if(currentDate.month()!= currendDisplayDate.month())
            {
                updateColorStyle("rgba(72,114,251,0.6)","white","normal");
            }
            else
            {
                updateColorStyle("rgb(72,114,251)","white","normal");
            }
        }
    }
    isChecked=_ischeck;
}

monthButton::~monthButton()
{
    // 注销全局事件
    CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

