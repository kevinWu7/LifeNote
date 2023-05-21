#include "themeswitchwidget.h"
#include "ui_themeswitchwidget.h"
#include "theme.h"
#include "util.h"
#include "thememanager.h"
#include "logger.h"

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

void ThemeSwitchWidget::colorButtonClicked()
{
    QToolButton *button = qobject_cast<QToolButton*>(sender());
    if(button)
    {
        auto themeId=button->property("themeId").toString();
        auto color=button->property("color").toString();
        ThemeManager::getInstance().switchTheme(themeId,color);
        logger->log(QString(button->property("color").toString()));
    }

}

void ThemeSwitchWidget::InitDiyColorButtons()
{
    for(auto colorKry :diyThemeColor)
    {
        QToolButton* button = util::findWidget<QToolButton>(ui->diyColorWidget,"btn"+colorKry.first);
        if(button)
        {
            button->setStyleSheet(QString("background-color:%1").arg(QString(colorKry.second)));
            button->setProperty("themeId",colorKry.first);
            button->setProperty("color",colorKry.second);
            connect(button,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
        }
    }
}
