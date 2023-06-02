#ifndef LNSLIDER_H
#define LNSLIDER_H

#include <QSlider>

class LNSlider : public QSlider
{
public:
    LNSlider(QWidget* parent);

public:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // LNSLIDER_H
