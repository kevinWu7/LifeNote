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
        if(themeId.contains("Red"))
        {
            button->setIcon(QIcon(":/icons/res/checkin/tick.png"));
        }
        else
        {
            button->setIcon(QIcon(":/icons/res/checkin/selected.png"));
        }
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
            if(colorKry.first==ThemeManager::getInstance().ThemeId)
            {
                if(ThemeManager::getInstance().ThemeId.contains("Red"))
                {
                    button->setIcon(QIcon(":/icons/res/checkin/tick.png"));
                }
                else
                {
                    button->setIcon(QIcon(":/icons/res/checkin/selected.png"));
                }
                button->setChecked(true);
                currentCheckedBtn=button;
            }
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
    if("dark"==ThemeManager::getInstance().ThemeId)
    {
         ui->darkSystemBtn->setIcon(QIcon(":/icons/res/checkin/selected.png"));
         ui->darkSystemBtn->setChecked(true);
         currentCheckedBtn=ui->darkSystemBtn;
    }
    else if("light"==ThemeManager::getInstance().ThemeId)
    {
         ui->lightSystemBtn->setIcon(QIcon(":/icons/res/checkin/selected.png"));
         ui->lightSystemBtn->setChecked(true);
         currentCheckedBtn=ui->lightSystemBtn;
    }
    connect(ui->darkSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
    connect(ui->lightSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
}


