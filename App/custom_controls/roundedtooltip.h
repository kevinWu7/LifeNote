#ifndef ROUNDEDTOOLTIP_H
#define ROUNDEDTOOLTIP_H

#include <QWidget>
#include <QLabel>

class RoundedToolTip : public QWidget {
    Q_OBJECT

public:
    explicit RoundedToolTip(const QString &text, QWidget *parent = nullptr);

private:
    QLabel *m_label;
};

#endif // ROUNDEDTOOLTIP_H
