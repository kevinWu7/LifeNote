#include <QPainterPath>
#include "util.h"
#include "newnotegroupform.h"
#include "ui_newnotegroupform.h"


#define DEFAULT_NAME "未命名笔记本"
#define  ROUND_RADIUS 5

NewNoteGroupForm::NewNoteGroupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewNoteGroupForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);

    InitColorPushBtn();
    InitRoundRadius();
    InitEvent();
    ui->nameLineEdit->setText(DEFAULT_NAME);
}

void NewNoteGroupForm::InitColorPushBtn()
{
    for(int i=0;i<ui->colorLayout->count();i++)
    {
        QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->colorLayout->itemAt(i)->widget());
        QString color=QString::fromStdString(util::colorBtnMap.at(i));
        //动态设置颜色，从map中取值，并设置checked后的样式
        QString style=QString("QToolButton{background-color:%1;}"
                              "QToolButton:checked {border:2px solid %1;background-color:transparent;}").arg(color);
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
    this->mousePos = event->globalPosition(); // 获得鼠标位置
    this->dPos = mousePos - windowPos;   // 移动后部件所在的位置
}

void NewNoteGroupForm::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPosition().x() - this->dPos.x(),
               event->globalPosition().y() - this->dPos.y());
}

void NewNoteGroupForm::okBtn_clicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
       ui->warningLabel->setText("名称为空，请输入名称!");
       ui->warningLabel->setVisible(true);
       return;
    }
    else
    {
        ui->warningLabel->setText("");
        ui->warningLabel->setVisible(false);
    }
    emit sendParentWindowData(ui->nameLineEdit->text(),color_index);
    ui->nameLineEdit->setText(DEFAULT_NAME);
    this->setVisible(false);
}

void NewNoteGroupForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText(DEFAULT_NAME);
    ui->warningLabel->setVisible(false);
    this->setVisible(false);
}


