#ifndef CCONDITION_H
#define CCONDITION_H

#include <QString>
#include <QDataStream>
#include "common/cappenv.hpp"

class CCondition
{
public:
    explicit CCondition() = default;
    virtual ~CCondition() = default;

    CCondition(const CCondition &a) = default;
    CCondition(CCondition &&a) noexcept = default;
    CCondition& operator=(const CCondition &a) = default;
    CCondition& operator=(CCondition &&a) noexcept = default;

    virtual QString getConditionName() const = 0;
    virtual bool getStatus() const = 0;
    virtual QDataStream &operator <<(QDataStream &stream);
    virtual QDataStream &operator >>(QDataStream &stream) const;
};

#endif // CCONDITION_H
