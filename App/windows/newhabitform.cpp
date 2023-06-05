#include <QPainterPath>
#include "util.h"
#include "newhabitform.h"
#include "ui_newhabitform.h"


#define DefaultDisplayTip "未命名项目"
#define  ROUND_RADIUS 5

NewHabitForm::NewHabitForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewHabitForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);

    initIConBtn();
    InitRoundRadius();
    connect(ui->okBtn,&QPushButton::clicked,this,&NewHabitForm::okBtn_clicked);
    connect(ui->cancleBtn,&QPushButton::clicked,this,&NewHabitForm::cancleBtn_clicked);
    ui->nameLineEdit->setText(DefaultDisplayTip);
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

void NewHabitForm::setEditMode(QString name, int iconIndex)
{
    formMode=1;
    ui->nameLabel->setText("请输入新的项目名");
    ui->nameLineEdit->setText(name);
    QToolButton* toolBtn= dynamic_cast<QToolButton*>(ui->iconWidget->layout()->itemAt(iconIndex)->widget());
    if(!toolBtn->isChecked())
    {
        toolBtn->click();
    }
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
            }
        }
    }
}

void NewHabitForm::okBtn_clicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
       ui->warningLabel->setText("项目名称为空，请输入项目名称!");
       ui->warningLabel->setVisible(true);
       return;
    }
    else
    {
        ui->warningLabel->setText("");
        ui->warningLabel->setVisible(false);
    }
    emit sendSelectDataToParent(ui->nameLineEdit->text(),iconIndex,formMode);
    ui->nameLineEdit->setText(DefaultDisplayTip);
    this->setVisible(false);
    formMode=0;
}

void NewHabitForm::cancleBtn_clicked()
{
    ui->nameLineEdit->setText(DefaultDisplayTip);
    ui->warningLabel->setVisible(false);
    this->setVisible(false);
    formMode=0;
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


