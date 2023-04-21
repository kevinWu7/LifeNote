#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <QWidget>

namespace Ui {
class CalendarControl;
}

class CalendarControl : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarControl(QWidget *parent = nullptr);
    ~CalendarControl();

private:
    Ui::CalendarControl *ui;
};

#endif // CALENDARCONTROL_H
