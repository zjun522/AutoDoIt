#ifndef CPOWEROFFACTION_H
#define CPOWEROFFACTION_H

#include "caction.h"

class CPowerOffAction : public CAction
{
public:
    enum EPowerOffMode {
        EPOM_PowerOff,
        EPOM_Hibernate,
        EPOM_Suspend,
    };

public:
    CPowerOffAction(EPowerOffMode powerOffMode = EPOM_PowerOff, ulong delayBefore = 0, ulong delayAfter = 0);
    ~CPowerOffAction() override;

    CPowerOffAction(const CPowerOffAction &a) = default;
    CPowerOffAction(CPowerOffAction &&a) noexcept = default;
    CPowerOffAction& operator=(const CPowerOffAction &a) = default;
    CPowerOffAction& operator=(CPowerOffAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    EPowerOffMode getPowerOffMode() const;

private:
    EPowerOffMode m_powerOffMode = EPOM_PowerOff;
};

#endif // CPOWEROFFACTION_H
