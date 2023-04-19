#include "checkinwidget.h"
#include "ui_checkinwidget.h"

checkinWidget::checkinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkinWidget)
{
    ui->setupUi(this);
    ui->addItemBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->addItemBtn->setIcon(QIcon(":/res/icons/addnew.png"));
    ui->addItemBtn->setToolTip("添加打卡项目");
    ui->addItemBtn->setStyleSheet("QToolButton:hover { background-color: rgb(218, 218, 218); }"
                                     "QToolButton {min-height: 20px; max-height: 20px; margin: 0px; border: none; }");
    ui->mainWidget->setStyleSheet("QWidget#mainWidget"
                                  "{background-color:#FFFFFF;"
                                  "border-radius:7px}");
    connect(ui->addItemBtn,&QToolButton::clicked,this,&checkinWidget::addItemBtn_clicked);

}


void checkinWidget::addItemBtn_clicked()
{
    HabbitItem *item =new HabbitItem;
    int index = ui->mainWidget->layout()->count()-1; // Replace with the desired index where you want to insert the item
    QBoxLayout *boxLayout = dynamic_cast<QBoxLayout*>(ui->mainWidget->layout());
    if (boxLayout)
    {
        boxLayout->insertWidget(index, dynamic_cast<QWidget*>(item));
    }
}

checkinWidget::~checkinWidget()
{
    delete ui;
}
