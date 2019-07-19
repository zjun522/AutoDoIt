#ifndef CMESSAGEACTION_H
#define CMESSAGEACTION_H

#include "caction.h"

class CMessageAction : public CAction
{
public:
    enum EMsgMode {
        ETM_Message,
        ETM_Notify,
    };

public:
    CMessageAction() = default;
    CMessageAction(QString message, EMsgMode msgMode, ulong delayBefore = 0, ulong delayAfter = 0);
    ~CMessageAction() override;

    CMessageAction(const CMessageAction &a) = default;
    CMessageAction(CMessageAction &&a) noexcept = default;
    CMessageAction& operator=(const CMessageAction &a) = default;
    CMessageAction& operator=(CMessageAction &&a) noexcept = default;

    QString getActionName() const override;
    bool doIt() override;
    QDataStream &operator <<(QDataStream &stream) override;
    QDataStream &operator >>(QDataStream &stream) const override;

    EMsgMode getMsgMode() const;
    QString getMessage() const;

private:
    QString m_message;
    EMsgMode m_msgMode = ETM_Notify;
};

#endif // CMESSAGEACTION_H
