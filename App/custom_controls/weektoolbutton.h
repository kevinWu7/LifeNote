#ifndef WEEKTOOLBUTTON_H
#define WEEKTOOLBUTTON_H

#include <QToolButton>

class WeekToolButton : public QToolButton
{
public:
    WeekToolButton(QWidget *parent = nullptr,bool m_iconState=false);
    void WeekButton_clicked();
private :
    bool iconState=false;
};

#endif // WEEKTOOLBUTTON_H
