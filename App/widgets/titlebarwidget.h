#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMainWindow>
#include "thememanager.h"
class TitleBarWidget:public QWidget
{
public:
    TitleBarWidget(QWidget *parent = nullptr);
    QPoint m_dragPosition;
    QToolButton *iconButton;
    QLabel *titleLabel;
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
    void themeChangedUpdateUiStatus();
    themeChangedCallback bindThemeChangetCallback;
};

#endif // TITLEBARWIDGET_H
