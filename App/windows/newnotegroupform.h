#ifndef NEWNOTEGROUPFORM_H
#define NEWNOTEGROUPFORM_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class NewNoteGroupForm;
}

class NewNoteGroupForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewNoteGroupForm(QWidget *parent = nullptr);
    ~NewNoteGroupForm();

    void InitEvent();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    //记录鼠标，窗口位置
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

    void cancleBtn_clicked();
    void okBtn_clicked();

signals:
    void sendParentWindowData(QString);

private:
    Ui::NewNoteGroupForm *ui;
};

#endif // NEWNOTEGROUPFORM_H
