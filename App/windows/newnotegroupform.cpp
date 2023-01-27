#include "newnotegroupform.h"
#include "ui_newnotegroupform.h"
#include<QPainterPath>

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
    this->setStyleSheet(QString("QLabel{color:rgb(110,111,111);} "
                       "QWidget#NewNoteGroupForm{background-color:rgb(255,255,255)}"
                       "QLineEdit{background-color:rgb(244,244,246);border:none}"
                        "QLabel{font-size:11px}"
                        "QToolButton {"
                                                  "    border: 1px;"
                                                  "    width: %1 ; min-width: %1; max-width: %1;"
                                                  "    height: %1 ; min-height: %1; max-height: %1;"
                                                  "    border-radius: %2;"
                                                  "    color: white;"
                                                  "    padding: 0;"
                                                  "    margin: 0;"
                                                  "}"
                                                  "QToolButton:checked {"
                                                  "    border: 2px solid green; "
                                                  "}").arg("16px").arg("8px"));



    ui->nameLineEdit->setText(NewNoteGroupTip);
    //InitColorPushBtn();
    InitRoundRadius();
    InitEvent();

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
    emit sendParentWindowData(ui->nameLineEdit->text());
    ui->nameLineEdit->setText(NewNoteGroupTip);
    this->setVisible(false);
}

void NewNoteGroupForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText(NewNoteGroupTip);
    this->setVisible(false);
}


