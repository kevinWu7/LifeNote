#ifndef THEMESWITCHWIDGET_H
#define THEMESWITCHWIDGET_H

#include <QWidget>
#include <QToolButton>

namespace Ui {
class ThemeSwitchWidget;
}

class ThemeSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeSwitchWidget(QWidget *parent = nullptr);
    ~ThemeSwitchWidget();

public slots:
    void colorButtonClicked();
    void transparencySliderMoved();
    void leftTransparencySliderMoved();
    void rightTransparencySliderMoved();
    void transparencyEditEvent();
    void leftTransparencyEditEvent();
    void rightTransparencyEditEvent();
    void pictureBtnClicked();

private:
    Ui::ThemeSwitchWidget *ui;
    void InitDiyColorButtons();
    void InitSystemColorButtons();
    QToolButton* currentCheckedBtn=nullptr;

};

#endif // THEMESWITCHWIDGET_H
