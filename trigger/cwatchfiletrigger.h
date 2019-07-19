#ifndef CWATCHFILETRIGGER_H
#define CWATCHFILETRIGGER_H

#include "ctrigger.h"
#include <QTimer>

//监视文件触发器，如：某文件夹内有文件时做...
class CWatchFileTrigger : public CTrigger
{
    Q_OBJECT
public:
    CWatchFileTrigger(QObject *parent = nullptr);
    CWatchFileTrigger(QString fileName, int duration = 10000,
                      ERepeatMode repeatMode = ERepeatMode::EPM_ONCE,
                      std::shared_ptr<CAction> action = nullptr,
                      std::shared_ptr<CCondition> condition = nullptr,
                      QObject *parent = nullptr);
    ~CWatchFileTrigger() override;
    Q_DISABLE_COPY_MOVE(CWatchFileTrigger)

    QString getTriggerName() const override;
    void startWatch() override;
    void stopWatch() override;
    QDataStream &operator<<(QDataStream &stream) override;
    QDataStream &operator>>(QDataStream &stream) const override;

    QString getFileName() const;

public slots:
    void triggered() override;

private slots:
    void scanFile();

private:
    QTimer m_timer;
    int m_duration = 10*1000;
    QString m_fileName;
};

#endif // CWATCHFILETRIGGER_H
