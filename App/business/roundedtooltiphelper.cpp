#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QWidget>
#include <QEvent>
#include <QTimer>
#include "roundedtooltiphelper.h"



RoundedToolTip *RoundedToolTipHelper::m_toolTip = nullptr;
RoundedToolTipHelper *RoundedToolTipHelper::helper = nullptr;
std::map<QWidget *, RoundedToolTipHelper::ToolTipPosition> RoundedToolTipHelper::m_widgetPositions;

RoundedToolTipHelper::RoundedToolTipHelper(QObject *parent)
    : QObject(parent)
{
     m_showToolTipTimer = new QTimer(this);
     m_showToolTipTimer->setSingleShot(true);
     m_showToolTipTimer->setInterval(700); // 设置延迟时间为700毫秒
     connect(m_showToolTipTimer, &QTimer::timeout, this, &RoundedToolTipHelper::onShowToolTipTimerTimeout);
}

void RoundedToolTipHelper::installHelper(QWidget *widget, ToolTipPosition position)
{
    if (!helper)
    {
        helper = new RoundedToolTipHelper();
    }
    helper->m_widgetPositions[widget] = position;
    widget->installEventFilter(helper);
}

bool RoundedToolTipHelper::eventFilter(QObject *watched, QEvent *event)
{
    if (QWidget *widget = qobject_cast<QWidget *>(watched))
    {
        if (event->type() == QEvent::Enter)
        {
            m_currentWidget = widget;
            m_showToolTipTimer->start(); // 启动定时器
        }
        else if (event->type() == QEvent::Leave)
        {
            m_showToolTipTimer->stop(); // 停止定时器
            hideToolTip();
            m_currentWidget = nullptr;
        }
        else if (event->type() == QEvent::ToolTip)
        {
            return true; // 拦截 QEvent::ToolTip 事件，阻止系统默认提示显示
        }
    }
    return QObject::eventFilter(watched, event);
}




void RoundedToolTipHelper::onShowToolTipTimerTimeout()
{
    if (m_currentWidget)
    {
        showToolTip(m_currentWidget);
    }
}
void RoundedToolTipHelper::showToolTip(QWidget *widget)
{
    if (!m_toolTip)
    {
        QString toolTipText = widget->toolTip();
        if (!toolTipText.isEmpty())
        {
            m_toolTip = new RoundedToolTip(toolTipText, widget);
        }
    }
    if (m_toolTip)
    {
        QPoint mousePos = QCursor::pos();
        QRect screenRect = QGuiApplication::screenAt(mousePos)->availableGeometry();
        ToolTipPosition position = m_widgetPositions[widget];

        switch (position)
        {
        case Top:
            mousePos -= QPoint(0, m_toolTip->height());
            break;
        case Bottom:
            mousePos += QPoint(0, 20); // 20 是一个偏移值，可以根据需要调整
            break;
        case Left:
            mousePos -= QPoint(m_toolTip->width(), 0);
            break;
        case Right:
            mousePos += QPoint(0, 0); // 20 是一个偏移值，可以根据需要调整
            break;
        }

        // 检查提示框是否超出屏幕边缘，如果是，则进行调整
        if (mousePos.x() < screenRect.left())
        {
            mousePos.setX(screenRect.left());
        }
        else if (mousePos.x() + m_toolTip->width() > screenRect.right())
        {
            mousePos.setX(screenRect.right() - m_toolTip->width());
        }

        if (mousePos.y() < screenRect.top())
        {
            mousePos.setY(screenRect.top());
        }
        else if (mousePos.y() + m_toolTip->height() > screenRect.bottom())
        {
            mousePos.setY(screenRect.bottom() - m_toolTip->height());
        }

        m_toolTip->move(mousePos);
        m_toolTip->show();
    }
}
void RoundedToolTipHelper::hideToolTip()
{
    if (m_toolTip)
    {
        m_toolTip->hide();
        delete m_toolTip;
        m_toolTip = nullptr;
    }
}
