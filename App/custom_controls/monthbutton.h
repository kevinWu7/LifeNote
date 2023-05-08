#ifndef MONTHBUTTON_H
#define MONTHBUTTON_H

#include "checkinconfig.h"
#include <QDate>
#include <QToolButton>


class monthButton : public QToolButton
{
public:
     monthButton(QWidget *parent = nullptr,bool _ischecked=false);
     void setDate(const QDate& date);
     const QDate& getDate();
     QDate currendDisplayDate; //保存当前页面的月份第一天
     void receiveBtnChecked(checkin_dateitem* dateItem);
     void setMonthButtonClicked(bool _ischeck);
     bool isChecked=false;
     QString project_name;
     ~monthButton();
private:
     void monthButton_clicked();
     std::function<void(checkin_dateitem *)> bindFunctionOfreceiveBtnChecked;
     QDate currentDate;//记录当前按钮保存的日期
     QString baseStyleSheet;
     void initBaseStyleSheet();
     void updateColorStyle(QString background,QString textColor,QString font_weight);
};

#endif // MONTHBUTTON_H
