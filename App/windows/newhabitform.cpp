#include <QPainterPath>
#include "util.h"
#include "newhabitform.h"
#include "ui_newhabitform.h"
#include "logger.h""


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

    initRuleWidget();
    connect(ui->periodComboBox,&QComboBox::currentIndexChanged,this,&NewHabitForm::periodComboCurIndexChanged);
    connect(ui->timesComboBox,&QComboBox::currentIndexChanged,this,&NewHabitForm::periodComboCurIndexChanged);

}


void NewHabitForm::periodComboCurIndexChanged()
{
    auto index=ui->periodComboBox->currentIndex();
    logger->log(QString::number(index));
    if(index==0)//天
    {
        ui->timesComboBox->clear();
        ui->timesComboBox->addItem("1");
    }
    else if(index==1)//周
    {
        ui->timesComboBox->clear();
        for(int i=1;i<8;i++)
        {
           ui->timesComboBox->addItem(QString::number(i));
        }
    }
    else if(index==2)//月
    {
        ui->timesComboBox->clear();
        for(int i=1;i<11;i++)
        {
           ui->timesComboBox->addItem(QString::number(i));
        }
    }
}


void NewHabitForm::initRuleWidget()
{

    ui->periodComboBox->addItem("天");
    ui->periodComboBox->addItem("周");
    ui->periodComboBox->addItem("月");
    ui->timesComboBox->addItem("1");
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


