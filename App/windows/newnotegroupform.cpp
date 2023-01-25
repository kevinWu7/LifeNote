#include "newnotegroupform.h"
#include "ui_newnotegroupform.h"

NewNoteGroupForm::NewNoteGroupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewNoteGroupForm)
{
    ui->setupUi(this);
    InitEvent();
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
    sendParentWindowData(ui->nameLineEdit->text());
}

void NewNoteGroupForm::cancleBtn_clicked()
{
    this->setVisible(false);
}


