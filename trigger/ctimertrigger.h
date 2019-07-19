#ifndef CTIMERTRIGGER_H
#define CTIMERTRIGGER_H

#include <QTime>
#include <QTimer>
#include "ctrigger.h"

//时间触发器，如：30秒后做...
class CTimerTrigger : public CTrigger
{
    Q_OBJECT
public:
    enum ETimerMode {
        ETM_Timer,
        ETM_CountDown,
    };
public:
    explicit CTimerTrigger(QObject *parent = nullptr);
    explicit CTimerTrigger(QTime time,
                           ETimerMode timerMode = ETimerMode::ETM_Timer,
                           ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                           std::shared_ptr<CAction> action = nullptr,
                           std::shared_ptr<CCondition> condition = nullptr,
                           QObject *parent = nullptr);
    ~CTimerTrigger() override;
    Q_DISABLE_COPY_MOVE(CTimerTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QString getTipMsg() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    ETimerMode getTimerMode() const;
    QTime getTime() const;

//public slots:
//    virtual void triggered() override;

private:
    QTime m_time;
    QTimer m_timer;
    ETimerMode m_timerMode = ETimerMode::ETM_Timer;
};

#endif // CTIMERTRIGGER_H
