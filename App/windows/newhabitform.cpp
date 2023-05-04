#include <QPainterPath>
#include "util.h"
#include "logger.h"
#include "newhabitform.h"
#include "ui_newhabitform.h"
#include "baseinfo.h"


#define DefaultDisplayTip "未命名项目"
#define  ROUND_RADIUS 5

NewHabitForm::NewHabitForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewHabitForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    ui->iconWidget->setStyleSheet(QString("QToolButton{border:none;"
                                  "border: none;"
                                  "width: %1 ; min-width: %1; max-width: %1;"
                                  "height: %1 ; min-height: %1; max-height: %1;"
                                  "border-radius: %2;"
                                  "padding: 0;"
                                  "margin: 0;}"
                                  "QToolButton:checked{background-color:rgb(218, 218, 218)}"
                                  "QToolButton:hover{background-color:rgb(218, 218, 218)}"
                                   ).arg("30px","15px")
                               );
    this->setStyleSheet(QString("QLabel{color:rgb(110,111,111);font-size:11px} "
                                "QWidget#NewHabitForm {background-color:rgb(255,255,255)}"
                                "QLineEdit {background-color:rgb(244,244,246);border:none}"
                                "QPushButton#okBtn{"
                                "background-color:rgb(235,181,67);"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %2 ; min-height: %2; max-height: %2;"
                                "border-style: solid; "
                                "border-color:rgb(235,181,67); "
                                "border-width: 1px; "
                                "border-radius: 5px; "
                                "color:rgb(255,255,255)"
                                "}"
                                ).arg("60px","18px"));
    ui->bottomLine->setStyleSheet(QString("QFrame{border-top: 1px solid %1; border-bottom: none;}").arg(LINE_COLOR));
    ui->topLine->setStyleSheet(QString("QFrame{border-top: 1px solid %1; border-bottom: none;}").arg(LINE_COLOR));
    initIConBtn();
    InitRoundRadius();
    connect(ui->okBtn,&QPushButton::clicked,this,&NewHabitForm::okBtn_clicked);
    connect(ui->cancleBtn,&QPushButton::clicked,this,&NewHabitForm::cancleBtn_clicked);
}


void NewHabitForm::InitRoundRadius()
{
    QPainterPath path;
    QRectF rect = QRectF(0,0,this->width(),this->height());
    path.addRoundedRect(rect,ROUND_RADIUS,ROUND_RADIUS);
    QPolygon polygon= path.toFillPolygon().toPolygon();
    QRegion region(polygon);
    setMask(region);
}
void NewHabitForm::initIConBtn()
{
    for(int i=0;i<ui->iconWidget->layout()->count();i++)
    {
        QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->iconWidget->layout()->itemAt(i)->widget());
        QString icon=QString::fromStdString(util::iconMap.at(i));
        toolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolBtn->setIcon(QIcon(QString(":/icons/res/checkin/%1").arg(icon)));
        toolBtn->setIconSize(QSize(20,20));
        connect(toolBtn,&QToolButton::clicked,this,&NewHabitForm::iconBtn_clicked);
    }
}

void NewHabitForm::iconBtn_clicked()
{
    auto btn= dynamic_cast<QToolButton*>(sender());
    if(btn->isChecked())
    {
        for(int i=0;i<ui->iconWidget->layout()->count();i++)
        {
            QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->iconWidget->layout()->itemAt(i)->widget());
            if(btn!=toolBtn)
            {
                toolBtn->setChecked(false);
            }
            else
            {
                iconIndex=i;
                logger->log(QString("index: %1").arg(i));
            }
        }
    }
}

void NewHabitForm::okBtn_clicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
       ui->warningLabel->setText("项目名称为空，请输入项目名称!");
       ui->warningLabel->setStyleSheet("color:red");
       return;
    }
    else
    {
        ui->warningLabel->setText("");
        ui->warningLabel->setStyleSheet("color:transparent");
    }
    emit sendSelectDataToParent(ui->nameLineEdit->text(),iconIndex);
    ui->nameLineEdit->setText("");
    this->setVisible(false);
}

void NewHabitForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText("");
    this->setVisible(false);
}
void NewHabitForm::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();       // 获得部件当前位置
    this->mousePos = event->globalPosition(); // 获得鼠标位置
    this->dPos = mousePos - windowPos;   // 移动后部件所在的位置
}

void NewHabitForm::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPosition().x() - this->dPos.x(),
               event->globalPosition().y() - this->dPos.y());
}

NewHabitForm::~NewHabitForm()
{
    delete ui;
}


