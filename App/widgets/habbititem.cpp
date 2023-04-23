#include "habbititem.h"
#include "ui_habbititem.h"

HabbitItem::HabbitItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabbitItem)
{
    ui->setupUi(this);
    this->setStyleSheet(QString("QToolButton:hover {border: 0px; background-color: rgb(224, 224, 224); }"
                                "QToolButton{"
                                "border: 0px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "background-color: rgb(239,239,239);"
                                "padding: 0;"
                                "margin: 0;}"
                                "QToolButton#imgBtn{"
                                "background-color: transparent;"
                                "width: %3 ; min-width: %3; max-width: %3;"
                                "height: %3 ; min-height: %3; max-height: %3;"
                                "}"
                              ).arg("16px","8px","22px"));


    ui->imgBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->imgBtn->setIcon(QIcon(":/icons/res/checkin/run.png"));
    ui->imgBtn->setIconSize(QSize(22,22));

    ui->labelText->setFont(QFont("Arial", 14, QFont::Normal));
}



HabbitItem::~HabbitItem()
{
    delete ui;
}
