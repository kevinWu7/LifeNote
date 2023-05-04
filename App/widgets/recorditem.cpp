#include "recorditem.h"
#include "ui_recorditem.h"

RecordItem::RecordItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordItem)
{
    ui->setupUi(this);
    ui->numberLabel->setStyleSheet(QString(""));
    ui->numberLabel->setFont(QFont("Arial", 25, QFont::Normal));
    ui->textLabel->setFont(QFont("Arial", 12, QFont::Normal));
    ui->textLabel->setStyleSheet("color:rgb(160,160,160)");
}

RecordItem::~RecordItem()
{
    delete ui;
}
