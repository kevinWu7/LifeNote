#ifndef LNTEXTEDIT_H
#define LNTEXTEDIT_H


#include <QTextEdit>
#include <QTimer>

class LNTextEdit : public QTextEdit
{
public:
    LNTextEdit(QWidget *parent);
    void leaveEvent(QEvent *event) override;
    bool isAutoHide=true;
private:
    QTimer *timer=nullptr;
};

#endif // LNTEXTEDIT_H
