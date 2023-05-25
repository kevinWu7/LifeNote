#ifndef THEMESWITCHWIDGET_H
#define THEMESWITCHWIDGET_H

#include <QWidget>
#include <QToolBUtton>

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
    void transparencySliderMoved(int value);
    void transparencyEditEvent();
private:
    Ui::ThemeSwitchWidget *ui;
    void InitDiyColorButtons();
    void InitSystemColorButtons();
    QToolButton* currentCheckedBtn=nullptr;
};

#endif // THEMESWITCHWIDGET_H
