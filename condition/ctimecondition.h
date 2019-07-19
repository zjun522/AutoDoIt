#ifndef CTIMECONDITION_H
#define CTIMECONDITION_H

#include <QDateTime>
#include "ccondition.h"

//时间条件，如：在几点之前
class CTimeCondition : public CCondition
{
public:
    enum ETimeType{
        ETT_BEFORE,
        ETT_AFTER,
    };
public:
    CTimeCondition();
    CTimeCondition(QTime time, ETimeType timeType = ETimeType::ETT_AFTER);
    ~CTimeCondition() override;

    CTimeCondition(const CTimeCondition &a) = default;
    CTimeCondition(CTimeCondition &&a) noexcept = default;
    CTimeCondition& operator=(const CTimeCondition &a) = default;
    CTimeCondition& operator=(CTimeCondition &&a) noexcept = default;

    QString getConditionName() const override;
    bool getStatus() const override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    ETimeType getTimeType() const;

    QTime getTime() const;

private:
    QTime m_time;
    ETimeType m_timeType = ETimeType::ETT_AFTER;
};

#endif // CTIMECONDITION_H
