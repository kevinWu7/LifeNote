#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <QWidget>
#include <vector>
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
    void initDate();
    void initMonthBtnEvent();
   void monthBtn_clicked();
    std::vector<QDate> vector_date;
    std::vector<QDate> currentMonth_date;
};

#endif // CALENDARCONTROL_H
