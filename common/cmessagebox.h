#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H

#include <QMessageBox>

class CMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit CMessageBox(QString text, QWidget *parent = nullptr);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    int m_timerId = -1;
    int m_waitSecond = 20;
    QString m_text;
};

#endif // CMESSAGEBOX_H
