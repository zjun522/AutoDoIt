#ifndef CIDLETRIGGER_H
#define CIDLETRIGGER_H

#include "ctrigger.h"
#include <QTimer>

class CIdleTrigger : public CTrigger
{
    Q_OBJECT
public:
    CIdleTrigger(quint64 idleSeconds = 300000, int duration = 10000,
                 ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                 std::shared_ptr<CAction> action = nullptr,
                 std::shared_ptr<CCondition> condition = nullptr,
                 QObject *parent = nullptr);
    ~CIdleTrigger() override;
    Q_DISABLE_COPY_MOVE(CIdleTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    quint64 getIdleSeconds() const;

private slots:
    void scanIdle();

private:
    QTimer m_timer;
    int m_duration = 10*1000;
    quint64 m_idleSeconds = 300*1000;
};

#endif // CIDLETRIGGER_H
