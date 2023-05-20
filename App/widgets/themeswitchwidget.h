#ifndef THEMESWITCHWIDGET_H
#define THEMESWITCHWIDGET_H

#include <QWidget>

namespace Ui {
class ThemeSwitchWidget;
}

class ThemeSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeSwitchWidget(QWidget *parent = nullptr);
    ~ThemeSwitchWidget();

private:
    Ui::ThemeSwitchWidget *ui;
    void InitDiyColorButtons();
};

#endif // THEMESWITCHWIDGET_H
