#include "lnslider.h"
#include <QWheelEvent>
#include "logger.h"
#include "util.h"
#include <QScrollArea>
#include <QScrollBar>

LNSlider::LNSlider(QWidget *parent):
    QSlider(parent)
{
    installEventFilter(this);
}

//此方法为了禁止slider的滚轮事件，由于slider位于一个大的scroarea中
//滚轮事件会导致无法顺利的滚动整个页面
//但是单纯的禁用会导致滚到slider的区域内，滚轮没有任何反应了，所以需要获取滚轮的偏差值，并找到父scrollArea并手动为其赋值
bool LNSlider::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Wheel) // 检查是否为滚轮事件
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

        // 在这里根据滚轮事件滚动方向，处理滚动QScrollArea的逻辑
        QScrollArea* scrollArea = util::findParentWidget<QScrollArea>(this->parent());
        if (scrollArea)
        {
            int delta = wheelEvent->angleDelta().y();
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value() - delta);
        }
        return true; // 表示事件已被处理
    }
    return QSlider::eventFilter(watched, event);
}
