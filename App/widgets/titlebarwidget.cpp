#include "titlebarwidget.h"
#include <QLabel>
#include <QStyle>
#include "util.h"


TitleBarWidget::TitleBarWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setProperty("platform",util::getPlatFormName());
    mainLayout= new QHBoxLayout(this);
    mainLayout->setContentsMargins(7,0,0,0);
    mainLayout->setSpacing(2);
    minButton = new QToolButton();
    maxButton = new QToolButton();
    closeButton = new QToolButton();

    QSpacerItem *spacer=new QSpacerItem(100, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    minButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    minButton->setIcon(QIcon(":/icons/res/system/min.png"));
    minButton->setIconSize(QSize(13,13));

    closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    closeButton->setIcon(QIcon(":/icons/res/system/close.png"));
    closeButton->setIconSize(QSize(13,13));

    maxButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    maxButton->setIcon(QIcon(":/icons/res/system/max.png"));
    maxButton->setIconSize(QSize(13,13));
    mainLayout->addSpacerItem(spacer);
    mainLayout->addWidget(minButton);
    mainLayout->addWidget(maxButton);
    mainLayout->addWidget(closeButton);

    mainWindow=util::getQMainWindowByWidget(this);
}


void TitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 记录鼠标按下时的窗口位置
        m_dragPosition =  event->globalPosition().toPoint() -mainWindow->frameGeometry().topLeft();
        event->accept();
    }
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //移动窗口到当前鼠标位置
        mainWindow->move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void TitleBarWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 鼠标释放时，停止拖动
        event->accept();
    }
}
