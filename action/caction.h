#ifndef CACTION_H
#define CACTION_H

#include <QString>
#include <QDataStream>
#include "common/cappenv.hpp"

class CAction
{
public:
    CAction(ulong delayBefore = 0, ulong delayAfter = 0, QString icon = QLatin1String());
    virtual ~CAction() = default;

    CAction(const CAction &a) = default;
    CAction(CAction &&a) noexcept = default;
    CAction& operator=(const CAction &a) = default;
    CAction& operator=(CAction &&a) noexcept = default;

    virtual QString getActionName() const = 0;
    virtual bool doIt();
    virtual QDataStream &operator <<(QDataStream &stream);
    virtual QDataStream &operator >>(QDataStream &stream) const;

    ulong getDelayBefore() const;
    ulong getDelayAfter() const;
    QString getIcon() const;
    QString getErrorString() const;

protected:
    ulong m_delayBefore = 0;
    ulong m_delayAfter = 0;
    QString m_icon;
    QString m_errorString;
};

#endif // CACTION_H
