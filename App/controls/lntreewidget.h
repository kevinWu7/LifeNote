#ifndef LNTREEWIDGET_H
#define LNTREEWIDGET_H
#include<QTreeWidget>
#include<QEvent>

class LNTreeWidget:public QTreeWidget
{
public:
     LNTreeWidget(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // LNTREEWIDGET_H
