#ifndef NEWHABITFORM_H
#define NEWHABITFORM_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class NewHabitForm;
}

class NewHabitForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewHabitForm(QWidget *parent = nullptr);
    ~NewHabitForm();
    void InitRoundRadius();

signals:
    void sendSelectDataToParent(QString name ,int iconIndex);

public slots:
    void okBtn_clicked();
    void cancleBtn_clicked();
    void iconBtn_clicked();
private:
    Ui::NewHabitForm *ui;
    void initIConBtn();
    int iconIndex=0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    //记录鼠标，窗口位置
    QPointF windowPos;
    QPointF mousePos;
    QPointF dPos;
};

#endif // NEWHABITFORM_H
