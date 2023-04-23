#ifndef MONTHBUTTON_H
#define MONTHBUTTON_H

#include <QToolButton>
#include<QDate>

class monthButton : public QToolButton
{
public:
     monthButton(QWidget *parent = nullptr,bool m_iconState=false);
     QDate date;//记录当前的日期
private:
     void monthButton_clicked();
     bool iconState=false;

};

#endif // MONTHBUTTON_H
