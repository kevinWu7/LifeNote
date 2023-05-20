#include "themeswitchwidget.h"
#include "ui_themeswitchwidget.h"
#include "theme.h"
#include "thememanager.h"
#include "util.h"

ThemeSwitchWidget::ThemeSwitchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemeSwitchWidget)
{
    ui->setupUi(this);
    InitDiyColorButtons();
}

ThemeSwitchWidget::~ThemeSwitchWidget()
{
    delete ui;
}

void ThemeSwitchWidget::InitDiyColorButtons()
{
    for(int i=0;i<ui->diyColorWidget->layout()->count();i++)
    {
        //QToolButton* button=dynamic_cast<QToolButton*>(ui->diyColorWidget->layout()->itemAt(i)->widget());
        QToolButton* button = util::findWidget<QToolButton>(ui->diyColorWidget,"btnRed"+QString::number(i+1));
        if(button)
        {
            int mapIndex=0;
            for(auto colorKry :diyThemeColor)
            {
                if(mapIndex==i)
                {
                    button->setStyleSheet(QString("background-color:%1").arg(QString(colorKry.second)));
                    break;
                }
                mapIndex++;
            }
        }
    }
}
