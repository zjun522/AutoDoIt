#include "cmessagebox.h"
#include <QTimerEvent>
#include <QPushButton>

CMessageBox::CMessageBox(QString text, QWidget *parent) :
    QMessageBox(parent),
    m_text(std::move(text))
{
    this->setIcon(QMessageBox::Information);
    this->addButton(QMessageBox::Ok)->setText(tr(u8"立即"));
    this->addButton(QMessageBox::Cancel)->setText(tr(u8"取消"));
    this->setText(tr(u8"%0秒后计算机%1！").arg(m_waitSecond).arg(m_text));
    m_timerId = startTimer(1000);
}

void CMessageBox::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId){
        this->setText(tr(u8"%0秒后计算机%1！").arg(--m_waitSecond).arg(m_text));
        if(m_waitSecond == 0)
        {
            this->setResult(QMessageBox::Ok);
            this->hide();
        }
    }
}
