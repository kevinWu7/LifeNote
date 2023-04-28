#ifndef WEEKTOOLBUTTON_H
#define WEEKTOOLBUTTON_H

#include <QDate>
#include <QToolButton>


class WeekToolButton : public QToolButton
{
    Q_OBJECT
public:
    WeekToolButton(QWidget *parent = nullptr,bool m_iconState=false);
    void WeekButton_clicked();
    void setWeekButtonClicked();
    QDate date;
signals:
    void OnWeekButtonClicked(QDate date,bool ischecked);
private :
    bool iconState=false;

};

#endif // WEEKTOOLBUTTON_H
