#ifndef CGROUPCONDITION_H
#define CGROUPCONDITION_H

#include "ccondition.h"
#include <memory>

class CGroupCondition : public CCondition
{
public:
    enum EAlgType{
        EAT_AND,
        EAT_OR
    };

public:
    CGroupCondition();
    CGroupCondition(std::shared_ptr<CCondition> cdt1,
                    std::shared_ptr<CCondition> cdt2 = nullptr,
                    EAlgType algtype = EAlgType::EAT_AND);
    ~CGroupCondition() override;

    CGroupCondition(const CGroupCondition &a) = default;
    CGroupCondition(CGroupCondition &&a) noexcept = default;
    CGroupCondition& operator=(const CGroupCondition &a) = default;
    CGroupCondition& operator=(CGroupCondition &&a) noexcept = default;

    QString getConditionName() const override;
    bool getStatus() const override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    std::shared_ptr<CCondition> getCdt1() const;
    std::shared_ptr<CCondition> getCdt2() const;

    EAlgType getAlg() const;

private:
    std::shared_ptr<CCondition> m_cdt1{ nullptr };
    std::shared_ptr<CCondition> m_cdt2{ nullptr };
    EAlgType m_alg = EAlgType::EAT_AND;
};

#endif // CGROUPCONDITION_H
