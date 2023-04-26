#include "habbititem.h"
#include "ui_habbititem.h"
#include "util.h"
#include "logger.h"
#include <QMouseEvent>

HabbitItem::HabbitItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HabbitItem)
{
    ui->setupUi(this);
    ui->weekWidget->setStyleSheet(QString("QToolButton:hover {border: 0px; background-color: rgb(210, 210, 215); }"
                                "QToolButton{"
                                "border: 0px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "background-color: rgb(230,230,235);"
                                "padding: 0;"
                                "margin: 0;}"
                              ).arg("16px","8px"));
    ui->imgBtn->setStyleSheet(QString("QToolButton#imgBtn{"
                           "background-color: transparent;"
                           "width: %1 ; min-width: %1; max-width: %1;"
                           "height: %1 ; min-height: %1; max-height: %1;}"
                          ).arg("22px"));
    //this->setStyleSheet("background-color:red");
    ui->imgBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->imgBtn->setIconSize(QSize(22,22));
    ui->nameLabel->setFont(QFont("Arial", 14, QFont::Normal));

    //connect(this, &HabbitItem::mousePressEvent, this, &HabbitItem::onMousePress);
}

void HabbitItem::mousePressEvent(QMouseEvent *event)
{
    // 判断鼠标事件类型
    if (event->type() == QMouseEvent::MouseButtonPress) {
        // 判断鼠标按钮类型
        if (event->button() == Qt::LeftButton) {
            // 处理左键点击事件
            // 获取鼠标点击位置的坐标

            emit triggerMousePressEvent(this,ui->weekWidget);

           // this->setStyleSheet("QWidget#HabbitItem{background-color:rgb(234,240,255)}");
            QPoint pos = event->pos();
            // ...
        } else if (event->button() == Qt::RightButton) {
            // 处理右键点击事件
            // 获取鼠标点击位置的坐标
            QPoint pos = event->pos();
            // ...
        }
    }
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
