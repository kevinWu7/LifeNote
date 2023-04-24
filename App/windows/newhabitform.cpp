#include "newhabitform.h"
#include "ui_newhabitform.h"
#include "util.h"
#include <QPainterPath>

#define DefaultDisplayTip "输入项目名"
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
    }
}

void NewHabitForm::okBtn_clicked()
{
    emit sendSelectDataToParent(ui->nameLineEdit->text(),2);
    ui->nameLineEdit->setText(DefaultDisplayTip);
    this->setVisible(false);
}

void NewHabitForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText(DefaultDisplayTip);
    this->setVisible(false);
}


NewHabitForm::~NewHabitForm()
{
    delete ui;
}


