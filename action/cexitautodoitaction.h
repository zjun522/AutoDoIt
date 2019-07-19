#ifndef CEXITAUTODOITACTION_H
#define CEXITAUTODOITACTION_H

#include "caction.h"

class CExitAutoDoItAction : public CAction
{
public:
    CExitAutoDoItAction(ulong delayBefore = 0, ulong delayAfter = 0);
    ~CExitAutoDoItAction() override;

    CExitAutoDoItAction(const CExitAutoDoItAction &a) = default;
    CExitAutoDoItAction(CExitAutoDoItAction &&a) noexcept = default;
    CExitAutoDoItAction& operator=(const CExitAutoDoItAction &a) = default;
    CExitAutoDoItAction& operator=(CExitAutoDoItAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
};

#endif // CEXITAUTODOITACTION_H
