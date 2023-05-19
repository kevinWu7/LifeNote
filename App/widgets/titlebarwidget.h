#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMainWindow>

class TitleBarWidget:public QWidget
{
public:
    TitleBarWidget(QWidget *parent = nullptr);
    QPoint m_dragPosition;
    QToolButton *minButton;
    QToolButton *maxButton;
    QToolButton *closeButton;
    QSpacerItem *spacer;
    QHBoxLayout *mainLayout;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
private:
    QMainWindow* mainWindow=nullptr;
};

#endif // TITLEBARWIDGET_H
