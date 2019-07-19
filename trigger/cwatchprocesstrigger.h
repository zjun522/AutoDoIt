#ifndef CWATCHPROCESSTRIGGER_H
#define CWATCHPROCESSTRIGGER_H

#include "ctrigger.h"
#include <QTimer>

//监视进程触发器，如：某进程退出时做...
class CWatchProcessTrigger : public CTrigger
{
     Q_OBJECT
public:
    explicit CWatchProcessTrigger(QObject *parent = nullptr);
    CWatchProcessTrigger(QString processName,
                         bool running = false, int duration = 10000,
                         ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                         std::shared_ptr<CAction> action = nullptr,
                         std::shared_ptr<CCondition> condition = nullptr,
                         QString taskName = "",
                         QObject *parent = nullptr);
    ~CWatchProcessTrigger() override;
    Q_DISABLE_COPY_MOVE(CWatchProcessTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    QString getProcessName() const;

    bool getRunning() const;

private slots:
    void scanProcess();

private:
    QTimer m_timer;
    int m_duration = 10000;
    QString m_processName;
    bool m_running = false;
};

#endif // CWATCHPROCESSTRIGGER_H
