#ifndef CSTARTSTOPPROCESSACTION_H
#define CSTARTSTOPPROCESSACTION_H

#include "caction.h"
#include <QString>
#include <QStringList>

class CStartStopProcessAction : public CAction
{
public:
    CStartStopProcessAction() = default;
    CStartStopProcessAction(QString program, QStringList arguments, bool start = true, ulong delayBefore = 0, ulong delayAfter = 0);
    ~CStartStopProcessAction() override;

    CStartStopProcessAction(const CStartStopProcessAction &a) = default;
    CStartStopProcessAction(CStartStopProcessAction &&a) noexcept = default;
    CStartStopProcessAction& operator=(const CStartStopProcessAction &a) = default;
    CStartStopProcessAction& operator=(CStartStopProcessAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    QString getProcessName() const;
    bool getStart() const;

private:
    QString m_program;
    QStringList m_arguments;
    bool m_start = true;
};

#endif // CSTARTSTOPPROCESSACTION_H
