#include "habbititem.h"
#include "ui_habbititem.h"

HabbitItem::HabbitItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabbitItem)
{
    ui->setupUi(this);
    this->setStyleSheet(QString("QToolButton:hover { background-color: rgb(224, 224, 224); }"
                                "QToolButton{"
                                "border: 1px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "background-color: rgb(239,239,239);"
                                "padding: 0;"
                                "margin: 0;"
                                "}").arg("16px","8px"));
}

HabbitItem::~HabbitItem()
{
    delete ui;
}
