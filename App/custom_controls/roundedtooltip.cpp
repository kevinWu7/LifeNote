#include <QVBoxLayout>
#include "roundedtooltip.h"


RoundedToolTip::RoundedToolTip(const QString &text, QWidget *parent)
    : QWidget(parent) {
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *layout = new QVBoxLayout(this);
    m_label = new QLabel();
    m_label->setText(text);
    m_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_label);
    QString styleSheet = QStringLiteral(
                                        "QLabel {"
                                        "border:none;"
                                        "border-radius: 5px;"
                                        "padding-left: 5px;" // 内边距
                                        "padding-right: 5px;" // 内边距
                                        "padding-top: 3px;" // 内边距
                                        "padding-bottom: 4px;" // 内边距
                                        "background-color: rgb(102,102,102);"
                                        "font:12px;"
                                        "color: white;"
                                        "}");
    m_label->setStyleSheet(styleSheet);
    this->layout()->setContentsMargins(0,0,0,0);
}

void RoundedToolTip::setToolTipText(QString tooltip)
{
    m_label->setText(tooltip);
}


