#ifndef CHECKINWIDGET_H
#define CHECKINWIDGET_H

#include <QWidget>
#include "newhabitform.h"
#include <QToolButton>

struct project_info
{
    QString project_name;
    QToolButton * button;
    QString iconIndex;
};

namespace Ui {
class checkinWidget;
}

class checkinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit checkinWidget(QWidget *parent = nullptr);
    ~checkinWidget();
    NewHabitForm *habitForm=nullptr;


public slots:
    void addItemBtn_clicked();

private:
    Ui::checkinWidget *ui;
    void onReceiveNewHabitFormData(QString name, int iconIndex);
    std::vector<project_info*> LoadCheckinConfig();
};

#endif // CHECKINWIDGET_H
