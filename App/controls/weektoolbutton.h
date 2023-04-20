#ifndef WEEKTOOLBUTTON_H
#define WEEKTOOLBUTTON_H

#include <QToolButton>
#include "logger.h"

class WeekToolButton : public QToolButton
{
public:
    WeekToolButton(QWidget *parent = nullptr);
    void CheckinButton_clicked();
};

#endif // WEEKTOOLBUTTON_H
