#ifndef LNTREEWIDGET_H
#define LNTREEWIDGET_H
#include <QTreeWidget>
#include <QEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QTimer>
#include "extraqtreewidgetitem.h"

class LNTreeWidget:public QTreeWidget
{
public:
     LNTreeWidget(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    //TODO:新增一个coloredItem全局变量，目的是为了实时更新拖动到目标item时
    //目标item能够改变背景色为灰色，并且在移除目标item时，能够重新变回白色
    //但是暂时无法在dragEnterEvent中实现该效果(dragEnterEvent中无法获取目标item，是由TreeWidget重写该方法，所以只会进一次)
    //只能在dragMoveEvent中配合这个变量来实现此效果，不断的将目标item添加到vector和将其移除vector这种较为搓的方式来实现
    std::vector<ExtraQTreeWidgetItem*> coloredItem;
    QTimer *timer=nullptr;
};

#endif // LNTREEWIDGET_H
