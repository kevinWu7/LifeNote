#include "monthbutton.h"
#include "calendarcentral.h"


monthButton::monthButton(QWidget *parent ,bool _ischecked)
    : QToolButton(parent)
{
    connect(this,&QToolButton::clicked,this,&monthButton::monthButton_clicked);
    this->setIconSize(QSize(24,24));
    isChecked=_ischecked;
    // 绑定成员函数到实例
    //bindFunctionOfreceiveBtnChecked = std::bind(&monthButton::receiveBtnChecked, this, std::placeholders::_1);
    //CalendarCentral::getInstance().registerGlobalEvent(bindFunctionOfreceiveBtnChecked);
    initBaseStyleSheet();
    updateColorStyle("rgb(239,239,239)","rgb(158,158,158)","normal","12");
}

void monthButton::initBaseStyleSheet()
{
       baseStyleSheet = QString("QToolButton{"
                                "border: none;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "padding: 0;"
                                "margin: 0;}").arg("36px","18px");
};

void monthButton::updateColorStyle(QString background,QString textColor,QString font_weight,QString font_size)
{
      QString colorStyleSheet = QString("QToolButton"
                                       "{background-color:%1;"
                                       "color:%2;"
                                       "font-weight: %3;"
                                       "font-size:%4}")
                                       .arg(background,textColor,font_weight,font_size);
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

/*void monthButton::receiveBtnChecked(checkin_dateitem* dateItem)
{
    if(dateItem->sender==senderBtn::monthBtn||dateItem->project_name!=project_name)
    {
        return;
    }
    if(dateItem->date== currentDate)
    {
        setMonthButtonClicked(dateItem->ischecked);
    }
}*/



void monthButton::monthButton_clicked()
{
    isChecked=!isChecked;
    checkin_dateitem *item =new checkin_dateitem;
    item->date=currentDate;
    item->tips="";
    item->ischecked=isChecked;
    item->sender=senderBtn::monthBtn;
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

//仅仅改变ui状态. is_auto_checked指示是否为满足规则，自动补上的
void monthButton::setMonthButtonClicked(bool _ischeck,bool is_auto_checked)
{
    QDate _currentDate=QDate::currentDate();
    QString background=_ischeck?"rgb(72,114,251)":"rgb(239,239,239)";
    background=is_auto_checked?"rgba(72,114,251,0.3)" :background;
    if(currentDate==_currentDate)
    {
        if(!_ischeck)
        {
            updateColorStyle(background,"orange","bold","13px");
        }
        else
        {
            updateColorStyle(background,"orange","bold","13px");
        }
    }
    else
    {
        if(currentDate.month()!= currendDisplayDate.month())// 不是当月的
        {
            if(_ischeck||is_auto_checked)
            {
                updateColorStyle(background,"rgba(255,255,255,0.7)","normal","9px");
            }
            else
            {
                updateColorStyle(background,"rgb(148,148,148)","normal","9px");
            }
        }
        else
        {
            if(_ischeck||is_auto_checked)
            {
                updateColorStyle(background,"white","normal","13px");
            }
            else
            {
                updateColorStyle(background,"black","normal","13px");
            }
        }
    }
    isChecked=_ischeck;
}

monthButton::~monthButton()
{
    // 注销全局事件
    //CalendarCentral::getInstance().unregisterGlobalEvent(bindFunctionOfreceiveBtnChecked);
}

