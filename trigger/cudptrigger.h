#ifndef CUDPTRIGGER_H
#define CUDPTRIGGER_H

#include "common/data.h"
#include "ctrigger.h"
#include <QUdpSocket>

class CUDPTrigger : public CTrigger
{
    Q_OBJECT

public:
    explicit CUDPTrigger(QObject *parent = nullptr);
    CUDPTrigger(const QString &remoteHost, const QString &command,
                ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                std::shared_ptr<CAction> action = nullptr,
                std::shared_ptr<CCondition> condition = nullptr,
                QString taskName = "",
                QObject *parent = nullptr);
    ~CUDPTrigger() override;
    Q_DISABLE_COPY_MOVE(CUDPTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    SNetCmd getNetCmd() const;

private:
    static QUdpSocket m_udpSocket;
    static QMultiMap<CUDPTrigger*, SNetCmd> m_triggerMap;
    SNetCmd m_netCmd;

private slots:
    static void readData();
};

#endif // CUDPTRIGGER_H
