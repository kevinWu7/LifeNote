#include <QScrollBar>
#include <QDrag>
#include <QCoreApplication>
#include <QFile>
#include <QtGlobal>
#include <QHeaderView>
#include <QMessageBox>
#include "logger.h"
#include "util.h"
#include "noteconfig.h"
#include "lntreewidget.h"


LNTreeWidget::LNTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragEnabled(true);


    setDragDropMode(QAbstractItemView::DragDrop);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    timer=new QTimer();
    QObject::connect(timer, &QTimer::timeout,  [this]() {
        if(this->underMouse())
        {
            qDebug() << "Timer tick";
        }
        else
        {
            qDebug() << "Timer tick out of mouse";
            this->verticalScrollBar()->setVisible(false);
            this->horizontalScrollBar()->setVisible(false);
            timer->stop();
        }
    });
    this->setColumnCount(1);
    this->setColumnWidth(0,40);

    this->setHeaderLabels(QStringList()<<"first");
    this->header()->setStretchLastSection(false);
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //隐藏标题栏
    this->header()->setVisible(false);

    //设置不同层次菜单的缩进
    this->setIndentation(9);
    //设置边框不可见
    this->setFrameStyle(QFrame::NoFrame);


    /*QPalette palette = this->palette();
    palette.setColor(QPalette::Active, QPalette::Highlight, QColor(186,214,251));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(200, 200, 200,70));
    palette.setColor(QPalette::HighlightedText, QColor(0,0,0));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(0, 0, 0));
    this->setPalette(palette);*/
}


void LNTreeWidget::enterEvent(QEnterEvent *event)
{
    // 处理整个控件的鼠标移入事件
    // 隐藏水平滚动条
    QTreeWidget::enterEvent(event);
}
void LNTreeWidget::leaveEvent(QEvent *event)
{
    // 处理整个控件的鼠标移入事件
    // 隐藏水平滚动条
    QTreeWidget::leaveEvent(event);
    timer->start(1000);  // Tick every 1 second
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

void LNTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QTreeWidget::dragLeaveEvent(event);
    if(coloredItem.size()>0)
    {
        coloredItem.at(0)->setBackground(0, QBrush(QColor(Qt::white)));
        coloredItem.clear();
    }
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
    ExtraQTreeWidgetItem* targetItem =dynamic_cast<ExtraQTreeWidgetItem*>(this->itemFromIndex(modelIndex));
    if(targetItem->nodeType==ParentNode)
    {
       if(coloredItem.size()==0)
       {
           coloredItem.push_back(targetItem);
           targetItem->setBackground(0, QBrush(QColor(219,220,223)));
       }
       else
       {
           if(coloredItem.at(0)!=targetItem)
           {
               coloredItem.at(0)->setBackground(0, QBrush(QColor(Qt::white)));
               coloredItem.clear();
               coloredItem.push_back(targetItem);
               targetItem->setBackground(0, QBrush(QColor(219,220,223)));
           }
       }
       this->expandItem(targetItem);
    }
    else
    {
        if(coloredItem.size()>0)
        {
            coloredItem.at(0)->setBackground(0, QBrush(QColor(Qt::white)));
            coloredItem.clear();
        }
    }
    // 如果item不为nullptr，则表示鼠标指针在一个item上
    if (targetItem)
    {
        //qDebug() << "Drag item:" << targetItem->text(0);
        event->acceptProposedAction();
    }
}

//鼠标松开事件
void LNTreeWidget::dropEvent(QDropEvent *event)
{
    if(coloredItem.size()>0)
    {
        coloredItem.at(0)->setBackground(0, QBrush(QColor(Qt::white)));
        coloredItem.clear();
    }
    QModelIndex modelIndex = this->indexAt(event->position().toPoint());
    if(!modelIndex.isValid())
    {
         qDebug() <<"dropEvent isValid";
         return;
    }
    ExtraQTreeWidgetItem* targetItem = dynamic_cast<ExtraQTreeWidgetItem*>(this->itemFromIndex(modelIndex));
    if(targetItem->nodeType==ChildNode)
    {
        return;
    }
    ExtraQTreeWidgetItem *draggedItem =dynamic_cast<ExtraQTreeWidgetItem*>(currentItem());
    if(util::isChildItem(draggedItem,targetItem))
    {
        QMessageBox::warning(this, tr("警告"),tr("\n无法将父节点移动到子节点下!"));
        return;
    }
    //若是子节点移到直接的父节点下，则无需进行文件拷贝
    //只有不是直接父子节点关系，再进行文件拷贝
    if(draggedItem->parent()!=targetItem)
    {
        auto fullPath= util::treeItemToFullFilePath(draggedItem); //如d:/sotrage/xxx.html
        auto targetItemPath=util::treeItemToFullFilePath(targetItem);


        //移动本地存储文件到回收站
        QString fileName = util::treeItemToFileName(draggedItem); //文件名称，如xxx.html
        bool moveResult=false;
        if(draggedItem->nodeType==ParentNode)
        {
            moveResult=util::cutDir(fullPath,targetItemPath,true);
        }
        else
        {
            auto tragetFile=QString("%1/%2").arg(targetItemPath,fileName);
            moveResult= QFile::rename(fullPath,tragetFile); //A路径移动到B路径
        }
        std::string str="move node and move file "+ std::string(moveResult ? "true": "false") ;
        logger->log(str);
        if(!moveResult)
        {
            QMessageBox::warning(this, tr("错误"),tr("\n文件移动失败,无法完成操作"));
            return;
        }
    }
    //delete doc(updateXml) must be ahead of the QTreeWidget'Node delete
    //because updateXml function is depend on the Node struct
    //delete directly if node is parentNode

    noteconfig::updateXml(MoveNode,draggedItem,targetItem);

    draggedItem->deleteType=1;
    if(draggedItem->parent()!=nullptr)
    {
        draggedItem->parent()->removeChild(draggedItem);//this line will trigger currentTreeItemChanged immediately
    }
    else
    {
        int topIndex=indexOfTopLevelItem(draggedItem);
        takeTopLevelItem(topIndex);
    }
    draggedItem->deleteType=0;

    targetItem->addChild(draggedItem);
    this->setCurrentItem(draggedItem);
}


void LNTreeWidget::wheelEvent(QWheelEvent* event)
{
     QTreeWidget::wheelEvent(event);
}
