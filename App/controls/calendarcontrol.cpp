#include "calendarcontrol.h"
#include "ui_calendarcontrol.h"

CalendarControl::CalendarControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarControl)
{
    ui->setupUi(this);
}

CalendarControl::~CalendarControl()
{
    delete ui;
}
