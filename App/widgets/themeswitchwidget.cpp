#include <QIntValidator>
#include "themeswitchwidget.h"
#include "ui_themeswitchwidget.h"
#include "theme.h"
#include "util.h"
#include "thememanager.h"
#include "themeconfig.h"
#include "logger.h"
#include <QLineEdit>

ThemeSwitchWidget::ThemeSwitchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemeSwitchWidget)
{
    ui->setupUi(this);
    InitSystemColorButtons();
    InitDiyColorButtons();
    QIntValidator *validator = new QIntValidator(30, 100, this);
    ui->transparencyEdit->setValidator(validator);
    QIntValidator *leftvalidator = new QIntValidator(0, 100, this);
    ui->leftTransparencyEdit->setValidator(leftvalidator);
    QIntValidator *rightvalidator = new QIntValidator(0, 100, this);
    ui->rightTransparencyEdit->setValidator(rightvalidator);

    ui->transparencyEdit->setText(QString::number(ThemeManager::getInstance().Transparency));
    ui->leftTransparencyEdit->setText(QString::number(ThemeManager::getInstance().LeftTransparency));
    ui->rightTransparencyEdit->setText(QString::number(ThemeManager::getInstance().RightTransparency));

    ui->transparencyHSlider->setValue(ThemeManager::getInstance().Transparency);
    ui->leftTransparencyHSlider->setValue(ThemeManager::getInstance().LeftTransparency);
    ui->rightTransparencyHSlider->setValue(ThemeManager::getInstance().RightTransparency);

    connect(ui->transparencyHSlider,&QSlider::sliderReleased,this,&ThemeSwitchWidget::transparencySliderMoved);
    connect(ui->leftTransparencyHSlider,&QSlider::sliderReleased,this,&ThemeSwitchWidget::leftTransparencySliderMoved);
    connect(ui->rightTransparencyHSlider,&QSlider::sliderReleased,this,&ThemeSwitchWidget::rightTransparencySliderMoved);

    connect(ui->transparencyEdit,&QLineEdit::textEdited,this,&ThemeSwitchWidget::transparencyEditEvent);
    connect(ui->leftTransparencyEdit,&QLineEdit::textEdited,this,&ThemeSwitchWidget::leftTransparencyEditEvent);
    connect(ui->rightTransparencyEdit,&QLineEdit::textEdited,this,&ThemeSwitchWidget::rightTransparencyEditEvent);

    ui->pictureBtn1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->pictureBtn1->setText("无背景");
    ui->pictureBtn1->setIcon(QIcon(":/icons/res/note/img.png"));
    ui->pictureBtn1->setIconSize(QSize(100,50));

    connect( ui->pictureBtn1,&QToolButton::clicked,this,&ThemeSwitchWidget::pictureBtnClicked);
    for(int index=2;index<ui->pictureWidget->children().count();index++)
    {
        QToolButton* picBtn= util::findWidget<QToolButton>(ui->pictureWidget,"pictureBtn"+QString::number(index));
        if(picBtn)
        {
            QString backgroundImage= QString(":/imgs/res/images/pic%1.jpg").arg(QString::number(index)); // 替换为你的背景图片路径
            // 设置样式表, 性能很卡，切换纯色主题时会卡死，故换成下面的icon方式
           /* QString style = QString("background-image:url(%1);").arg(backgroundImage);
            picBtn->setStyleSheet(style);*/
            QIcon btn_icon;
            btn_icon.addFile(backgroundImage);
            picBtn->setIcon(btn_icon);
            picBtn->setIconSize(QSize(150, 70));
            connect(picBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::pictureBtnClicked);
        }
    }
}

ThemeSwitchWidget::~ThemeSwitchWidget()
{
    delete ui;
}

void ThemeSwitchWidget::colorButtonClicked()
{
    QToolButton *button = qobject_cast<QToolButton*>(sender());
    //若为当前的按钮，重复点击的情况
    if(currentCheckedBtn==button)
    {
        currentCheckedBtn->setChecked(true);
        return;
    }
    if(button&&button->isChecked())
    {
        auto themeId=button->property("themeId").toString();
        auto color=button->property("color").toString();

        ThemeManager::getInstance().switchTheme(themeId,ThemeManager::getInstance().PictureThemeId);

        logger->log(QString(button->property("color").toString()));
        if(themeId.contains("Red"))
        {
            button->setIcon(QIcon(":/icons/res/checkin/tick.svg"));
        }
        else
        {
            button->setIcon(util::CreateColorSvgIcon(":/icons/res/checkin/tick.svg","rgb(224,133,75)"));
        }
        if(currentCheckedBtn&&currentCheckedBtn!=button)
        {
            currentCheckedBtn->setChecked(false);
            currentCheckedBtn->setIcon(QIcon(""));
        }
        currentCheckedBtn=button;
    }
}

void ThemeSwitchWidget::transparencySliderMoved()
{
    auto window=util::getCurrentMainWindow();
    int value= ui->transparencyHSlider->value();
    window->setWindowOpacity(value/100.0);
    ui->transparencyEdit->setText(QString::number(value));
    ThemeManager::Transparency=value;
    themeConfig::getInstance().updateXml("Transparency",QString::number(value));
}

void ThemeSwitchWidget::leftTransparencySliderMoved()
{
    int value= ui->leftTransparencyHSlider->value();
    ui->leftTransparencyEdit->setText(QString::number(value));
    ThemeManager::LeftTransparency=value;
    themeConfig::getInstance().updateXml("LeftTransparency",QString::number(value));
    ThemeManager::getInstance().switchTheme(ThemeManager::ThemeId,ThemeManager::PictureThemeId,false);
}

void ThemeSwitchWidget::rightTransparencySliderMoved()
{
    int value= ui->rightTransparencyHSlider->value();
    ui->rightTransparencyEdit->setText(QString::number(value));
    ThemeManager::RightTransparency=value;
    themeConfig::getInstance().updateXml("RightTransparency",QString::number(value));
    ThemeManager::getInstance().switchTheme(ThemeManager::ThemeId,ThemeManager::PictureThemeId,false);
}

void ThemeSwitchWidget::transparencyEditEvent()
{
     logger->log(QString(ui->transparencyEdit->text()));
     int number=ui->transparencyEdit->text().toInt();
     if(number>=30&&number<=100)
     {
         ThemeManager::Transparency=number;
         ui->transparencyHSlider->setValue(number);
         auto window=util::getCurrentMainWindow();
         window->setWindowOpacity(number/100.0);
         themeConfig::getInstance().updateXml("Transparency",QString::number(number));
     }
}

void ThemeSwitchWidget::leftTransparencyEditEvent()
{
    int number=ui->leftTransparencyEdit->text().toInt();
    if(number>=0&&number<=100)
    {
        ThemeManager::LeftTransparency=number;
        ui->leftTransparencyHSlider->setValue(number);
        themeConfig::getInstance().updateXml("LeftTransparency",QString::number(number));
        ThemeManager::getInstance().switchTheme(ThemeManager::ThemeId,ThemeManager::PictureThemeId,false);
    }
}

void ThemeSwitchWidget::rightTransparencyEditEvent()
{
    int number=ui->rightTransparencyEdit->text().toInt();
    if(number>=0&&number<=100)
    {
        ThemeManager::RightTransparency=number;
        ui->rightTransparencyHSlider->setValue(number);
        themeConfig::getInstance().updateXml("RightTransparency",QString::number(number));
        ThemeManager::getInstance().switchTheme(ThemeManager::ThemeId,ThemeManager::PictureThemeId,false);
    }
}

void ThemeSwitchWidget::pictureBtnClicked()
{
   QToolButton *button = qobject_cast<QToolButton*>(sender());
   ThemeManager::getInstance().switchTheme(ThemeManager::getInstance().ThemeId,button->objectName().replace("tureBtn",""));
   for(int index=0;index<ui->pictureWidget->children().count();index++)
   {
       QToolButton* picBtn= qobject_cast<QToolButton*>(ui->pictureWidget->children().at(index));
       if(picBtn&&picBtn!=button)
       {
           picBtn->setChecked(false);
       }
   }
}

void ThemeSwitchWidget::noPictureBtnClicked()
{
    QToolButton *button = qobject_cast<QToolButton*>(sender());
    ThemeManager::getInstance().switchTheme(ThemeManager::getInstance().ThemeId,"none");
    for(int index=0;index<ui->pictureWidget->children().count();index++)
    {
        QToolButton* picBtn= qobject_cast<QToolButton*>(ui->pictureWidget->children().at(index));
        if(picBtn&&picBtn!=button)
        {
            picBtn->setChecked(false);
        }
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
                    button->setIcon(QIcon(":/icons/res/checkin/tick.svg"));
                }
                else
                {
                    button->setIcon(util::CreateColorSvgIcon(":/icons/res/checkin/tick.svg","rgb(224,133,75)"));
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
         ui->darkSystemBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/checkin/tick.svg","rgb(224,133,75)"));
         ui->darkSystemBtn->setChecked(true);
         currentCheckedBtn=ui->darkSystemBtn;
    }
    else if("light"==ThemeManager::getInstance().ThemeId)
    {
         ui->lightSystemBtn->setIcon(util::CreateColorSvgIcon(":/icons/res/checkin/tick.svg","rgb(224,133,75)"));
         ui->lightSystemBtn->setChecked(true);
         currentCheckedBtn=ui->lightSystemBtn;
    }
    connect(ui->darkSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
    connect(ui->lightSystemBtn,&QToolButton::clicked,this,&ThemeSwitchWidget::colorButtonClicked);
}


