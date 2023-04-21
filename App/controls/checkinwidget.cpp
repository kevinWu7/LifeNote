#include "checkinwidget.h"
#include "ui_checkinwidget.h"

checkinWidget::checkinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkinWidget)
{
    ui->setupUi(this);
    ui->addItemBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->addItemBtn->setIcon(QIcon(":/icons/res/note/addnew.png"));
    ui->addItemBtn->setToolTip("添加打卡项目");
    ui->addItemBtn->setIconSize(QSize(22,22));

    ui->iconBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->iconBtn->setIcon(QIcon(":/icons/res/checkin/sun.png"));
    ui->iconBtn->setIconSize(QSize(27,27));
    ui->dateTimeLabel->setFont(QFont("Arial", 16, QFont::Normal));
    ui->leftTitleStatus->setSpacing(2);

    ui->titleBarControl->setStyleSheet("QToolButton:hover { background-color: rgb(218, 218, 218); }"
                                     "QToolButton{width:32px;min-width:32px;max-width:32px;height:32px; min-height:32px; max-height:32px;"
                                       "margin:0px; border:none;}");
    ui->leftNavigateWidget->setStyleSheet("QWidget#leftNavigateWidget"
                                  "{background-color:#FFFFFF;"
                                  "border-radius:7px}");
    connect(ui->addItemBtn,&QToolButton::clicked,this,&checkinWidget::addItemBtn_clicked);

}


void checkinWidget::addItemBtn_clicked()
{
    HabbitItem *item =new HabbitItem;
    int index = ui->leftNavigateWidget->layout()->count()-1; // Replace with the desired index where you want to insert the item
    QBoxLayout *boxLayout = dynamic_cast<QBoxLayout*>(ui->leftNavigateWidget->layout());
    if (boxLayout)
    {
        boxLayout->insertWidget(index, dynamic_cast<QWidget*>(item));
    }
}

checkinWidget::~checkinWidget()
{
    delete ui;
}
