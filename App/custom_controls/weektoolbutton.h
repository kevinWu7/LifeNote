#ifndef WEEKTOOLBUTTON_H
#define WEEKTOOLBUTTON_H

#include <QDate>
#include <QToolButton>
#include "checkinconfig.h"


class WeekToolButton : public QToolButton
{
    Q_OBJECT
public:
    WeekToolButton(QWidget *parent = nullptr,bool _ischecked=false);
    void WeekButton_clicked();
    void setDate(const QDate& _date);
    const QDate& getDate();
    QString project_name;
    void setWeekButtonClicked(bool _ischecked,bool is_auto_checked=false);
    ~WeekToolButton();

signals:
    //void OnWeekButtonClicked(QDate date,bool ischecked);
private :
    bool isChecked=false;
    QDate currentDate;
    void initBaseStyleSheet();
    void updateSizeStyle(int size);
    QString baseStyleSheet;
};

#endif // WEEKTOOLBUTTON_H
