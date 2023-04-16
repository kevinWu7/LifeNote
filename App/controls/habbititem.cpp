#include "habbititem.h"
#include "ui_habbititem.h"

HabbitItem::HabbitItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabbitItem)
{
    ui->setupUi(this);
}

HabbitItem::~HabbitItem()
{
    delete ui;
}
