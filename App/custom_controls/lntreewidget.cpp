#include <QScrollBar>
#include <QDrag>
#include <QCoreApplication>
#include <QFile>
#include <QtGlobal>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include "util.h"
#include "noteconfig.h"
#include "lntreewidget.h"
#include "theme.h"
#include "logger.h"



LNTreeWidget::LNTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // 设置自定义委托

    this->setItemDelegate(&delegate);
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

    //绑定themechanged事件
    bFuncThemeChangeCallback=std::bind(&LNTreeWidget::themeChanged,this);
    ThemeManager::getInstance().registerThemeGlobalEvent(bFuncThemeChangeCallback);

    QPalette palette = this->palette();
    auto activeHighLight=util::generateRGBAColor(currentTheme["CONTROL_SELECTED"],1.0);
    auto inActiveHighLight=util::generateRGBAColor(currentTheme["CONTROL_NOSELECTED"],1.0);
    palette.setColor(QPalette::Active, QPalette::Highlight, activeHighLight);//item选中时背景色
    palette.setColor(QPalette::Inactive, QPalette::Highlight,inActiveHighLight);//item选中但失焦时背景色
    //此处可以修改文本颜色，但是系统默认的已经ok了
    //palette.setColor(QPalette::HighlightedText, QColor(255,255,0)); //item选中时文字色
    //palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(160, 0, 0));//item选中但失焦时文字色
    this->setPalette(palette);
}

void LNTreeWidget::themeChanged()
{
    QPalette palette = this->palette();
    auto activeHighLight=util::generateRGBAColor(currentTheme["CONTROL_SELECTED"],1.0);
    auto inActiveHighLight=util::generateRGBAColor(currentTheme["CONTROL_NOSELECTED"],1.0);
    palette.setColor(QPalette::Active, QPalette::Highlight, activeHighLight);//item选中时背景色
    palette.setColor(QPalette::Inactive, QPalette::Highlight,inActiveHighLight);//item选中但失焦时背景色
    this->setPalette(palette);
}

LNTreeWidget::~LNTreeWidget()
{
    if(bFuncThemeChangeCallback)
    {
        ThemeManager::getInstance().unregisterThemeGlobalEvent(bFuncThemeChangeCallback);
    }
}

void LNTreeWidget::enterEvent(QEnterEvent *event)
{
    // 处理整个控件的鼠标移入事件
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
    QTreeWidget::mousePressEvent(event);
}


void LNTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    if(targetItem)
    {
        targetItem->setBackground(0, QBrush( QColor(255, 0, 0,0)));
        targetItem->setData(0, Qt::UserRole, CustomDelegate::NoLine);
        // 重新绘制控件
        viewport()->update();
    }
    QTreeWidget::dragLeaveEvent(event);
}

//拖动中不断触发的事件
void LNTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    // 获取被拖拽的QTreeWidgetItem
    QPoint cur_point=event->position().toPoint();
    QModelIndex modelIndex = this->indexAt(cur_point);
    QRect itemRect = visualRect(modelIndex);  // 获取当前节点的矩形区域
    int topY = itemRect.top();  // 当前节点的上边框 Y 值
    int bottomY = itemRect.bottom();  // 当前节点的下边框 Y 值

    if(!modelIndex.isValid())
    {
        return;
    }

    targetItem =dynamic_cast<ExtraQTreeWidgetItem*>(this->itemFromIndex(modelIndex));

    ExtraQTreeWidgetItem *draggedItem =dynamic_cast<ExtraQTreeWidgetItem*>(currentItem());

    if(targetItem->nodeType==ParentNode)
    {
        //这是因为拖拽刚开始，targetItem就是draggedItem，所以这里加个判断区分
        if(targetItem!=draggedItem)
        {
           targetItem->setBackground(0,QBrush(util::generateRGBAColor(currentTheme["CONTROL_SELECTED"],0.8)));
        }

        this->expandItem(targetItem);
    }
    // 调用递归函数开始遍历，将背景色全部设为透明，并将上次的横线残留删除
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = topLevelItem(i);
        traverseTreeWidgetItems(item);
    }
    //设置横线的标记
    if(targetItem->parent()!=nullptr)
    {
        if(targetItem->nodeType==ChildNode)
        {
            targetItem->setData(0, Qt::UserRole, CustomDelegate::TopLine);
            //当拖动到最后一个节点时,将userRole设置为2，目的是画一条下面的线，使用户可以移动到任意位置
            if(targetItem->parent()->indexOfChild(targetItem)==targetItem->parent()->childCount()-1)
            {
                if(cur_point.y()<=bottomY&&cur_point.y()>(bottomY-topY)/2.0+topY)//靠下方1/2时，显示下方横线
                {
                    targetItem->setData(0, Qt::UserRole, CustomDelegate::BottomLine);
                }
            }
        }
        else if(targetItem->nodeType==ParentNode)
        {
            if(cur_point.y()<=bottomY&&cur_point.y()>(bottomY-topY)/4.0+topY) //在靠下方3/4的位置属于添加到父节点
            {
                targetItem->setData(0, Qt::UserRole, CustomDelegate::NoLine);
            }
            else if(cur_point.y()<=(bottomY-topY)/4.0+topY)//在靠上方1/4的位置属于插入到上方
            {
                targetItem->setData(0, Qt::UserRole, CustomDelegate::TopLine);
                targetItem->setBackground(0,QBrush(QColor(255,0,0,0)));
            }
            //这段逻辑添加后会有点复杂，先注释了。
            //目前的缺陷为:当parentNode在最下方时，无法将其他节点拖动到这个parentNode的下方
            /*//最后一个节点时，需将userRole设置为2，目的是画一条下面的线，使用户可以移动到任意位置
            if(targetItem->parent()->indexOfChild(targetItem)==targetItem->parent()->childCount()-1)
            {
                if(cur_point.y()<=bottomY&&cur_point.y()>(bottomY-topY)/4.0*3+topY)//靠下方1/4时，显示下方横线
                {
                    targetItem->setData(0, Qt::UserRole, CustomDelegate::BottomLine);
                }
            }*/
        }
        if(targetItem==draggedItem) //在自身节点移动时，取消横线
        {
             targetItem->setData(0, Qt::UserRole, CustomDelegate::NoLine);
        }
    }
    // 重新绘制控件
    viewport()->update();
    // 如果item不为nullptr，则表示鼠标指针在一个item上
    if (targetItem)
    {
        event->acceptProposedAction();
    }
}

//鼠标松开事件
void LNTreeWidget::dropEvent(QDropEvent *event)
{
    QModelIndex modelIndex = this->indexAt(event->position().toPoint());
    ExtraQTreeWidgetItem *draggedItem =dynamic_cast<ExtraQTreeWidgetItem*>(currentItem());
    if(!modelIndex.isValid()||draggedItem==targetItem)
    {
        return;
    }

    if(util::isChildItem(draggedItem,targetItem))
    {
        QMessageBox::warning(this, tr("警告"),tr("\n无法将父节点移动到子节点下!"));
        return;
    }
    QVariant data = targetItem->data(0,Qt::UserRole);
    auto targetParentItem=(targetItem->nodeType==ChildNode||data.toInt()==CustomDelegate::TopLine)
            ?targetItem->parent():targetItem;
    //若在同个父节点之间移动位置，则无需拷贝文件
    if(draggedItem->parent()!=targetParentItem)
    {
        auto fullPath= util::treeItemToFullFilePath(draggedItem); //如d:/sotrage/xxx.html
        auto targetItemPath=util::treeItemToFullFilePath(targetParentItem);
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
        if(!moveResult)
        {
            if(targetItem)
            {
                targetItem->setBackground(0, QBrush( QColor(255, 0, 0,0)));
                targetItem->setData(0, Qt::UserRole, CustomDelegate::NoLine);
                // 重新绘制控件
                viewport()->update();
            }
            QMessageBox::warning(this, tr("错误"),tr("\n文件移动失败\n请检查是否有同名节点"));
            return;
        }
    }
    //当目标node为子节点时，说明需要插入到该节点的上方
    if(targetItem->nodeType==ChildNode||data.toInt()!=CustomDelegate::NoLine)
    {
        //delete doc(updateXml) must be ahead of the QTreeWidget'Node delete
        //because updateXml function is depend on the Node struct
        //delete directly if node is parentNode

        if (data.toInt()==CustomDelegate::TopLine)
        {
            noteconfig::updateXml(InsertNode,draggedItem,targetItem);
        }
        else if(data.toInt()==CustomDelegate::BottomLine)
        {
            noteconfig::updateXml(MoveNode,draggedItem,targetItem->parent());
        }

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
        int insertIndex=targetItem->parent()->indexOfChild(targetItem);
        if (data.toInt()==CustomDelegate::TopLine) //1指示移动到该节点的上方
        {
            targetItem->parent()->insertChild(insertIndex,draggedItem);
        }
        else if(data.toInt()==CustomDelegate::BottomLine)  //2指示移动到该节点的下方
        {
            targetItem->parent()->addChild(draggedItem);
        }
    }
    else  //当目标node为父节点时，说明需要加入到该父节点的最下方
    {
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
    }
    this->setCurrentItem(draggedItem);
    if(targetItem)
    {
        targetItem->setBackground(0, QBrush( QColor(255, 0, 0,0)));
        targetItem->setData(0, Qt::UserRole, CustomDelegate::NoLine);
        // 重新绘制控件
        viewport()->update();
    }
}


void LNTreeWidget::wheelEvent(QWheelEvent* event)
{
    QTreeWidget::wheelEvent(event);
}

void LNTreeWidget::traverseTreeWidgetItems(QTreeWidgetItem *item)
{
    if (!item)
        return;
    if(item!=currentItem()&&item!=targetItem)
    {
       item->setBackground(0, QBrush( QColor(255, 0, 0,0)));
    }
    item->setData(0, Qt::UserRole, CustomDelegate::NoLine);
    // 遍历子节点
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *childItem = item->child(i);
        traverseTreeWidgetItems(childItem);
    }
}



