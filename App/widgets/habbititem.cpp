#include "habbititem.h"
#include "ui_habbititem.h"
#include "util.h"
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
    ui->imgBtn->setIconSize(QSize(22,22));
    ui->nameLabel->setFont(QFont("Arial", 14, QFont::Normal));
}

void HabbitItem::setIconIndex(int iconIndex)
{
    auto icon=QString::fromStdString(util::iconMap[iconIndex]);
    ui->imgBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
}

void HabbitItem::setProjectName(QString name)
{
    ui->nameLabel->setText(name);
}

HabbitItem::~HabbitItem()
{
    delete ui;
}
