#ifndef CPROCESSCONDITION_H
#define CPROCESSCONDITION_H

#include "ccondition.h"
#include <QString>

class CProcessCondition : public CCondition
{
public:
    CProcessCondition();
    explicit CProcessCondition(QString processName, bool running = false);
    ~CProcessCondition() override;

    CProcessCondition(const CProcessCondition &a) = default;
    CProcessCondition(CProcessCondition &&a) noexcept = default;
    CProcessCondition& operator=(const CProcessCondition &a) = default;
    CProcessCondition& operator=(CProcessCondition &&a) noexcept = default;

    QString getConditionName() const override;
    bool getStatus() const override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    QString getProcessName() const;
    bool getRunning() const;

private:
    QString m_processName;
    bool m_running = false;
};

#endif // CPROCESSCONDITION_H
