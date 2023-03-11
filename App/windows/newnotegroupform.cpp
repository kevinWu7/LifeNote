#include "newnotegroupform.h"
#include "ui_newnotegroupform.h"
#include <QPainterPath>
#include "util.h"

#define  ROUND_RADIUS 5
#define  NewNoteGroupTip "未命名笔记本"

NewNoteGroupForm::NewNoteGroupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewNoteGroupForm)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setStyleSheet(QString("QLabel{color:rgb(110,111,111);font-size:11px} "
                                "QWidget#NewNoteGroupForm {background-color:rgb(255,255,255)}"
                                "QLineEdit {background-color:rgb(244,244,246);border:none}"
                                "QToolButton{"
                                "border: 1px;"
                                "width: %1 ; min-width: %1; max-width: %1;"
                                "height: %1 ; min-height: %1; max-height: %1;"
                                "border-radius: %2;"
                                "color: white;"
                                "padding: 0;"
                                "margin: 0;"
                                "}"
                                "QPushButton#okBtn{"
                                "background-color:rgb(235,181,67);"
                                "width: %3 ; min-width: %3; max-width: %3;"
                                "height: %4 ; min-height: %4; max-height: %4;"
                                "border-style: solid; "
                                "border-color:rgb(235,181,67); "
                                "border-width: 1px; "
                                "border-radius: 5px; "
                                "color:rgb(255,255,255)"
                                "}"
                                ).arg("16px").arg("8px").arg("85px").arg("18px"));
    ui->nameLineEdit->setText(NewNoteGroupTip);
    InitColorPushBtn();   
    InitRoundRadius();
    InitEvent();
}

void NewNoteGroupForm::InitColorPushBtn()
{
    for(int i=0;i<ui->colorLayout->count();i++)
    {
        QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->colorLayout->itemAt(i)->widget());
        QString color=QString::fromStdString(util::colorBtnMap.at(i));

        //动态设置颜色，从map中取值，并设置checked后的样式
        QString style=QString("QToolButton{background-color:%1;}"
                              "QToolButton:checked {border:2px solid %1;background-color:#FFFFFF;}").arg(color);
        toolBtn->setStyleSheet(style);
        connect(toolBtn,&QToolButton::clicked,this,&NewNoteGroupForm::onColorToolBtn_clicked);
    }
}

void NewNoteGroupForm::onColorToolBtn_clicked()
{
    QToolButton* button = dynamic_cast<QToolButton*>(QObject::sender());
    if(button->isChecked())
    {
        for(int i=0;i<ui->colorLayout->count();i++)
        {
            QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->colorLayout->itemAt(i)->widget());
            if(toolBtn!=button)
            {
                toolBtn->setChecked(false);
            }
            else
            {
                color_index=i;
            }
        }
    }
    else
    {
        color_index=0;
    }
}

void NewNoteGroupForm::InitRoundRadius()
{
    QPainterPath path;
    QRectF rect = QRectF(0,0,this->width(),this->height());
    path.addRoundedRect(rect,ROUND_RADIUS,ROUND_RADIUS);
    QPolygon polygon= path.toFillPolygon().toPolygon();
    QRegion region(polygon);
    setMask(region);
}
void NewNoteGroupForm::InitEvent()
{
    connect(ui->cancleBtn,&QPushButton::clicked,this,&NewNoteGroupForm::cancleBtn_clicked);
    connect(ui->okBtn,&QPushButton::clicked,this,&NewNoteGroupForm::okBtn_clicked);
}

NewNoteGroupForm::~NewNoteGroupForm()
{
    delete ui;
}

void NewNoteGroupForm::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();       // 获得部件当前位置
    this->mousePos = event->globalPos(); // 获得鼠标位置
    this->dPos = mousePos - windowPos;   // 移动后部件所在的位置
}

void NewNoteGroupForm::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void NewNoteGroupForm::okBtn_clicked()
{
    emit sendParentWindowData(ui->nameLineEdit->text(),color_index);
    ui->nameLineEdit->setText(NewNoteGroupTip);
    this->setVisible(false);
    //color_index=0;
}

void NewNoteGroupForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText(NewNoteGroupTip);
    this->setVisible(false);
}


