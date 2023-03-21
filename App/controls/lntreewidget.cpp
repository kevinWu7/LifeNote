#include "lntreewidget.h"
#include "logger.h"
#include <QScrollBar>>


void LNTreeWidget::enterEvent(QEnterEvent *event)
{
    // 处理整个控件的鼠标移入事件
    logger->log(QString("移入了控件"));
    // 隐藏水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QTreeWidget::enterEvent(event);
}
void LNTreeWidget::leaveEvent(QEvent *event)
{
    // 处理整个控件的鼠标移入事件
    logger->log(QString("移入了控件"));
    // 隐藏水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QTreeWidget::leaveEvent(event);
}


LNTreeWidget::LNTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    // 构造函数
}

