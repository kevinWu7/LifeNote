#ifndef ROUNDEDTOOLTIPHELPER_H
#define ROUNDEDTOOLTIPHELPER_H

#include <map>
#include <QObject>
#include "roundedtooltip.h"

class RoundedToolTipHelper : public QObject
{
    Q_OBJECT



public:

    enum ToolTipPosition {
         MouseTop,
         MouseBottom,
         MouseLeft,
         MouseRight,
         ControlTop,
         ControlBottom,
         ControlLeft,
         ControlRight,
     };

    explicit RoundedToolTipHelper(QObject *parent = nullptr);
    static void installHelper(QWidget *widget ,ToolTipPosition position = MouseBottom);
    static void unInstallHelper(QWidget *widget);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onShowToolTipTimerTimeout();
private:
    static void showToolTip(QWidget *widget);
    static void hideToolTip();
    static std::map<QWidget *, ToolTipPosition> m_widgetPositions;
    static RoundedToolTip *m_toolTip;
    QTimer *m_showToolTipTimer;
    static QWidget *m_currentWidget;
    static RoundedToolTipHelper *helper;
};

#endif // ROUNDEDTOOLTIPHELPER_H
