#ifndef CGROUPTRIGGER_H
#define CGROUPTRIGGER_H

#include "ctrigger.h"
#include <QVector>

class CGroupTrigger : public CTrigger
{
    Q_OBJECT
public:
    CGroupTrigger(ERepeatMode repeatMode = ERepeatMode::EPM_ONCE, std::shared_ptr<CAction> action = nullptr, std::shared_ptr<CCondition> condition = nullptr, QObject *parent = nullptr);
    ~CGroupTrigger() override;
    Q_DISABLE_COPY_MOVE(CGroupTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QString getTipMsg() override;
    bool getIsWatch() const override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    void addTrigger(CTrigger *trig);

    QVector<CTrigger *> getTriggers() const;

private:
    QVector<CTrigger *> m_triggers;
};

#endif // CGROUPTRIGGER_H
