#ifndef CSTARTSTOPTASKACTION_H
#define CSTARTSTOPTASKACTION_H

#include "caction.h"

class CStartStopTaskAction : public CAction
{
public:
    CStartStopTaskAction(bool start = true, ulong delayBefore = 0, ulong delayAfter = 0);
    ~CStartStopTaskAction() override;

    CStartStopTaskAction(const CStartStopTaskAction &a) = default;
    CStartStopTaskAction(CStartStopTaskAction &&a) noexcept = default;
    CStartStopTaskAction& operator=(const CStartStopTaskAction &a) = default;
    CStartStopTaskAction& operator=(CStartStopTaskAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    bool getStart() const;

private:
    bool m_start = true;
};

#endif // CSTARTSTOPTASKACTION_H
