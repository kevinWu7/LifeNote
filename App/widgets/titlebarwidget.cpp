#include "titlebarwidget.h"
#include <QLabel>
#include <QStyle>

TitleBarWidget::TitleBarWidget(QWidget *parent)
    :QWidget(parent)
{
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
    closeButton->setIconSize(QSize(14,14));

    maxButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    maxButton->setIcon(QIcon(":/icons/res/system/max.png"));
    maxButton->setIconSize(QSize(13,13));
    mainLayout->addWidget(closeButton);
    mainLayout->addWidget(minButton);
    mainLayout->addWidget(maxButton);
    mainLayout->addSpacerItem(spacer);
}
