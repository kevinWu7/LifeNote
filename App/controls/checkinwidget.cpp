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
}

checkinWidget::~checkinWidget()
{
    delete ui;
}
