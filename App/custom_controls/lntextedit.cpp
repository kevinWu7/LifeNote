#include <QScrollBar>
#include "lntextedit.h"


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
}


void LNTextEdit::leaveEvent(QEvent *event)
{
    // 隐藏水平滚动条
    QTextEdit::leaveEvent(event);
    if(isAutoHide)
    {
       timer->start(1000);  // Tick every 1 second
    }
}


