#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QWidget>
#include <QEvent>
#include <QTimer>
#include "roundedtooltiphelper.h"

#define ControlPositionSpacing 5

RoundedToolTip *RoundedToolTipHelper::m_toolTip = nullptr;
RoundedToolTipHelper *RoundedToolTipHelper::helper = nullptr;
QWidget* RoundedToolTipHelper::m_currentWidget = nullptr;
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
        if (!m_toolTip)
        {
            QString toolTipText = widget->toolTip();
            if (!toolTipText.isEmpty())
            {
                m_toolTip = new RoundedToolTip(toolTipText, widget);
            }
        }
    }
    helper->m_widgetPositions[widget] = position;
    widget->installEventFilter(helper);
}

void RoundedToolTipHelper::unInstallHelper(QWidget *widget)
{
    // 移除事件过滤器
    widget->removeEventFilter(helper);
    // 清理相关资源
    m_widgetPositions.erase(widget);
    if (widget == m_currentWidget)
    {
        m_currentWidget = nullptr;
        hideToolTip();
    }
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
    if(!m_toolTip)
    {
        m_toolTip=new RoundedToolTip(widget->toolTip(),widget);
    }

    if (m_toolTip)
    {
        m_toolTip->setToolTipText(widget->toolTip());
        m_toolTip->show();

        QPoint mousePos = QCursor::pos();
        QRect screenRect = QGuiApplication::screenAt(mousePos)->availableGeometry();
        ToolTipPosition position = m_widgetPositions[widget];
        QRect widgetRect = widget->rect();
        switch (position)
        {
        case MouseTop:
            mousePos -= QPoint(0, m_toolTip->height()+ControlPositionSpacing);
            break;
        case MouseBottom:
            mousePos += QPoint(0, ControlPositionSpacing*4); // 20 是一个偏移值，可以根据需要调整
            break;
        case MouseLeft:
            mousePos -= QPoint(m_toolTip->width()+ControlPositionSpacing, m_toolTip->height()/2);
            break;
        case MouseRight:
            mousePos += QPoint(ControlPositionSpacing*3, -m_toolTip->height()/2); // 20 是一个偏移值，可以根据需要调整
            break;
        case ControlTop:
            mousePos = widget->mapToGlobal(widgetRect.topLeft()-
                       QPoint(m_toolTip->width()/2-widgetRect.width()/2, m_toolTip->height()+ControlPositionSpacing));
            break;
        case ControlBottom:
            mousePos = widget->mapToGlobal(widgetRect.bottomLeft()-
                       QPoint(m_toolTip->width()/2-widgetRect.width()/2, -ControlPositionSpacing*2));
            break;
        case ControlLeft:
            mousePos = widget->mapToGlobal(widgetRect.topLeft() -
                      QPoint(m_toolTip->width()+ControlPositionSpacing,m_toolTip->height()/2-widgetRect.height()/2 ));
            break;
        case ControlRight:
            mousePos = widget->mapToGlobal(widgetRect.topRight()-
                       QPoint(-ControlPositionSpacing,m_toolTip->height()/2-widgetRect.height()/2 ));
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
   }
}

void RoundedToolTipHelper::hideToolTip()
{
    if (m_toolTip)
    {
        m_toolTip->hide();
    }
    delete m_toolTip;
    m_toolTip = nullptr;
}
