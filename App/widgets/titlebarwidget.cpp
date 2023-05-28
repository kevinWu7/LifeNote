#include "titlebarwidget.h"
#include <QLabel>
#include <QStyle>
#include "util.h"
#include "baseinfo.h"
#include "theme.h"



TitleBarWidget::TitleBarWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setProperty("platform",util::getPlatFormName());
    mainLayout= new QHBoxLayout(this);
    mainLayout->setContentsMargins(7,0,7,0);
    mainLayout->setSpacing(5);
    iconButton = new QToolButton();
    titleLabel = new QLabel();
    minButton = new QToolButton();
    maxButton = new QToolButton();
    closeButton = new QToolButton();

    iconButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    iconButton->setIcon(QIcon(":/icons/res/system/logo.png"));
    iconButton->setIconSize(QSize(13,13));
    titleLabel->setText(WINDOW_TITLE);

    QString color= currentTheme["CONTROL_TEXT"];
    QSpacerItem *spacer=new QSpacerItem(100, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    minButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    minButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_min.svg",color));
    minButton->setIconSize(QSize(13,13));


    closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    closeButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_close.svg",color));
    closeButton->setIconSize(QSize(13,13));

    maxButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    maxButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_max.svg",color));
    maxButton->setIconSize(QSize(13,13));

    mainLayout->addWidget(iconButton);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addWidget(minButton);
    mainLayout->addWidget(maxButton);
    mainLayout->addWidget(closeButton);

    mainWindow=util::getQMainWindowByWidget(this);
    bindThemeChangetCallback=std::bind(&TitleBarWidget::themeChangedUpdateUiStatus,this);
    ThemeManager::getInstance().registerThemeGlobalEvent(bindThemeChangetCallback);
}

void TitleBarWidget::themeChangedUpdateUiStatus()
{
    QString color= currentTheme["CONTROL_TEXT"];
    minButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_min.svg",color));
    closeButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_close.svg",color));
    maxButton->setIcon(util::CreateColorSvgIcon(":/icons/res/system/win_max.svg",color));
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
    const QPoint topleft = mapToGlobal(QPoint(0, 0));
    //这里不够精准
    if(event->globalPosition().toPoint().x()<topleft.x()+iconButton->width()+titleLabel->width()
        ||event->globalPosition().toPoint().x()>topleft.x()+ width() -maxButton->width()-minButton->width()-closeButton->width())
    {
        return;
    }
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
        setCursor(Qt::ArrowCursor);
    }
}
