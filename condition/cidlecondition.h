#ifndef CIDLECONDITION_H
#define CIDLECONDITION_H

#include "ccondition.h"
#include <QString>

class CIdleCondition : public CCondition
{
public:
    explicit CIdleCondition(quint64 idleSeconds = 300000);
    ~CIdleCondition() override;

    CIdleCondition(const CIdleCondition &a) = default;
    CIdleCondition(CIdleCondition &&a) noexcept = default;
    CIdleCondition& operator=(const CIdleCondition &a) = default;
    CIdleCondition& operator=(CIdleCondition &&a) noexcept = default;

    QString getConditionName() const override;
    bool getStatus() const override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    quint64 getIdleSeconds() const;

private:
    quint64 m_idleSeconds = 300000;
};

#endif // CIDLECONDITION_H
