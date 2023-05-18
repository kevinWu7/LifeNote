#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>

class TitleBarWidget:public QWidget
{
public:
    TitleBarWidget(QWidget *parent = nullptr);

    QToolButton *minButton;
    QToolButton *maxButton;
    QToolButton *closeButton;
    QSpacerItem *spacer;
    QHBoxLayout *mainLayout;
   // (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

};

#endif // TITLEBARWIDGET_H
