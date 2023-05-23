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
   InitSystemColorButtons();
    InitDiyColorButtons();
}

ThemeSwitchWidget::~ThemeSwitchWidget()
{
    delete ui;
}

void ThemeSwitchWidget::colorButtonClicked()
{
    QToolButton *button = qobject_cast<QToolButton*>(sender());
    if(button&&button->isChecked())
    {
        auto themeId=button->property("themeId").toString();
        auto color=button->property("color").toString();
        ThemeManager::getInstance().switchTheme(themeId,color);
        logger->log(QString(button->property("color").toString()));
        button->setIcon(QIcon(":/icons/res/checkin/tick.png"));
        if(currentCheckedBtn&&currentCheckedBtn!=button)
        {
            currentCheckedBtn->setChecked(false);
            currentCheckedBtn->setIcon(QIcon(""));
        }
        currentCheckedBtn=button;
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
            button->setText("");
            button->setIconSize(QSize(12,12));
            connect(button,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
        }
    }
}

void ThemeSwitchWidget::InitSystemColorButtons()
{
    ui->darkSystemBtn->setStyleSheet(QString("width:%1;min-width: %1;"
                                             "max-width: %1;height: %2;min-height: %2;"
                                             "max-height: %2;border-radius: 8px;"
                                             "background-color:%3").arg("70px","35px",themeDark["BACKGROUND_COLOR1"]));
    ui->lightSystemBtn->setStyleSheet(QString("width:%1;min-width: %1;"
                                              "max-width: %1;height: %2;min-height: %2;"
                                              "max-height: %2;border-radius: 8px;"
                                              "background-color:%3").arg("70px","35px",themeLight["BACKGROUND_COLOR1"]));
    ui->darkSystemBtn->setProperty("themeId","dark");
    ui->darkSystemBtn->setProperty("color",themeDark["BACKGROUND_COLOR1"]);
    ui->lightSystemBtn->setProperty("themeId","light");
    ui->lightSystemBtn->setProperty("color",themeLight["BACKGROUND_COLOR1"]);
    connect(ui->darkSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
    connect(ui->lightSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
}


