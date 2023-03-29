#include "lntextedit.h"
#include <QScrollBar>

LNTextEdit::LNTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
     setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     timer=new QTimer();
     connect(timer,&QTimer::timeout,this, [this](){
         if(!this->underMouse())
         {
             qDebug() << "Timer tick out of mouse";
             this->verticalScrollBar()->setVisible(false);
             timer->stop();
         }
     });
     this->setStyleSheet("border:0px; background-color:#FFFFFF");
     QScrollBar* verticalScrollBar=this->verticalScrollBar();
     verticalScrollBar->setStyleSheet("QScrollBar:vertical"  //滚动条的背景
                              "{"
                              "width:6px;"
                              "background:rgba(0,0,0,0%);"
                              "margin:0px,0px,0px,0px;"
                              "padding-top:0px;"
                              "padding-bottom:0px;"
                              "}"
                              "QScrollBar::handle:vertical" //滚动条本身
                              "{"
                              "width:6px;"
                              "background:rgba(0,0,0,20%);"
                              " border-radius:3px;"
                              "min-height:20;"
                              "}"
                              "QScrollBar::handle:vertical:hover"//滚动条本身，鼠标选中的样式
                              "{"
                              "width:6px;"
                              "background:rgba(0,0,0,35%);"
                              " border-radius:3px;"
                              "min-height:20;"
                              "}"
                              "QScrollBar::add-line:vertical" //上箭头，高度设置为0，隐藏掉
                              "{height:0px;subcontrol-position:bottom;}"
                              "QScrollBar::sub-line:vertical"//下拉箭头，高度设置为0，隐藏掉
                              "{height:0px;subcontrol-position:top;}"
                              );
}


void LNTextEdit::leaveEvent(QEvent *event)
{
    // 处理整个控件的鼠标移入事件
    // 隐藏水平滚动条
    QTextEdit::leaveEvent(event);
    timer->start(1000);  // Tick every 1 second
}
