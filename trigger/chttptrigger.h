#ifndef CHTTPTRIGGER_H
#define CHTTPTRIGGER_H

#include "common/csslserver.h"
#include "common/data.h"
#include "ctrigger.h"

class CHTTPTrigger : public CTrigger
{
    Q_OBJECT
public:
    explicit CHTTPTrigger(QObject *parent = nullptr);
    CHTTPTrigger(const QString &remoteHost, const QString &command,
                ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                std::shared_ptr<CAction> action = nullptr,
                std::shared_ptr<CCondition> condition = nullptr,
                QString taskName = "",
                QObject *parent = nullptr);
    ~CHTTPTrigger() override;
    Q_DISABLE_COPY_MOVE(CHTTPTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    SNetCmd getNetCmd() const;

private:
    static CSslServer m_server;
    static QMultiMap<CHTTPTrigger*, SNetCmd> m_triggerMap;
    SNetCmd m_netCmd;

private slots:
    static void readData(const QByteArray &array, QString hostName);
};

#endif // CHTTPTRIGGER_H
