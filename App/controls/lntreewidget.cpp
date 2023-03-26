#include "lntreewidget.h"
#include "logger.h"
#include <QScrollBar>
#include <QDrag>
#include "ExtraQTreeWidgetItem.h"

LNTreeWidget::LNTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}


void LNTreeWidget::enterEvent(QEnterEvent *event)
{
    // 处理整个控件的鼠标移入事件
    // 隐藏水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QTreeWidget::enterEvent(event);
}
void LNTreeWidget::leaveEvent(QEvent *event)
{
    // 处理整个控件的鼠标移入事件
    // 隐藏水平滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QTreeWidget::leaveEvent(event);
}

void LNTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 获取拖动项
        QTreeWidgetItem* item = currentItem();
        if (item)
        {
            // 创建拖动数据对象
            /** mimeData = new QMimeData();
            QByteArray encodedData;
            QDataStream stream(&encodedData, QIODevice::WriteOnly);
            stream << item->text(0);
            mimeData->setData("text/plain", encodedData);

            // 创建拖动对象
            QDrag* drag = new QDrag(this);
            drag->setMimeData(mimeData);
            //drag->setPixmap(QPixmap(":/images/leaf.png"));
            drag->setHotSpot(QPoint(0, 0));

            // 启动拖动操作
            drag->exec();*/
        }
    }
    QTreeWidget::mousePressEvent(event);
}

void LNTreeWidget::startDrag1(Qt::DropActions supportedActions)
{
    // Get the currently selected item
    QTreeWidgetItem *item = currentItem();

    // Create a custom pixmap for the drag image
    QPixmap pixmap(":/res/icons/underline.png");
    QPixmap scaledPixmap = pixmap.scaled(QSize(32, 32), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // Create a drag object and set the pixmap as the drag image
    QDrag *drag = new QDrag(this);
    drag->setPixmap(scaledPixmap);

    // Set the MIME type for the drag
    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/x-qabstractitemmodeldatalist", QByteArray());
    drag->setMimeData(mimeData);

    // Start the drag
    drag->exec(Qt::MoveAction);
}


//拖动中不断触发的事件
void LNTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    // 获取被拖拽的QTreeWidgetItem
    QModelIndex modelIndex = this->indexAt(event->position().toPoint());
    if(!modelIndex.isValid())
    {
        qDebug() <<"isValid";
        return;
    }
    QTreeWidgetItem* item = this->itemFromIndex(modelIndex);
    if(item->childCount()>0)
    {
        this->expandItem(item);
    }
    // 如果item不为nullptr，则表示鼠标指针在一个item上
    if (item)
    {
        qDebug() << "Drag item:" << item->text(0);
        event->acceptProposedAction();

    }
}

//鼠标松开事件
void LNTreeWidget::dropEvent(QDropEvent *event)
{
    QModelIndex modelIndex = this->indexAt(event->position().toPoint());
    if(!modelIndex.isValid())
    {
         qDebug() <<"dropEvent isValid";
         return;
    }
    ExtraQTreeWidgetItem* targetItem = dynamic_cast<ExtraQTreeWidgetItem*>(this->itemFromIndex(modelIndex));
    if(targetItem->nodeType==BaseInfo::Child)
    {

        return;
    }
    ExtraQTreeWidgetItem *draggedItem =dynamic_cast<ExtraQTreeWidgetItem*>(currentItem());

    draggedItem->deleteType=1;
    draggedItem->parent()->removeChild(draggedItem);//this line will trigger currentTreeItemChanged immediately
    draggedItem->deleteType=0;

    targetItem->addChild(draggedItem);
    this->setCurrentItem(draggedItem);

}


