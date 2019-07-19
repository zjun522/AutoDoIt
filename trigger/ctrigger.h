#ifndef CTRIGGER_H
#define CTRIGGER_H

#include <QObject>
#include <QDateTime>
#include <memory>
#include "common/cappenv.hpp"

class CGroupCondition;
class CCondition;
class CAction;

class CTrigger : public QObject
{
    Q_OBJECT
public:
    enum ERepeatMode {
        EPM_ONCE,
        EPM_REPEAT,
    };
public:
    explicit CTrigger(QObject *parent = nullptr);
    explicit CTrigger(ERepeatMode repeatMode = ERepeatMode::EPM_ONCE, bool repeatStartWatch = false, std::shared_ptr<CAction> action = nullptr, std::shared_ptr<CCondition> condition = nullptr, QObject *parent = nullptr);
    ~CTrigger() override = default;
    Q_DISABLE_COPY_MOVE(CTrigger)

    virtual QString getTaskName() const;
    virtual QString getTriggerName() const = 0;
    virtual void startWatch() {
        m_isWatch = true;
        m_done = false;
    }
    virtual void stopWatch() {
        m_isWatch = false;
    }
    virtual QString getTipMsg();
    virtual bool getIsWatch() const;
    virtual QDataStream &operator<<(QDataStream &stream);
    virtual QDataStream &operator>>(QDataStream &stream) const;

    ERepeatMode getRepeatMode() const;
    std::shared_ptr<CAction> getAction() const;
    std::shared_ptr<CCondition> getCondition() const;
    bool getDone() const;
    bool getLastActionSuccess() const;
    void doItManual();

signals:
    void trigger();

public slots:
    void doIt();
    virtual void triggered();

private:
    std::shared_ptr<CAction> m_action{nullptr};
    std::shared_ptr<CCondition> m_condition{nullptr};

protected:
    ERepeatMode m_repeatMode = ERepeatMode::EPM_ONCE;
    bool m_repeatStartWatch = false;  //repeat模式下，触发后是否需要再次startwatch
    bool m_isWatch = false;
    bool m_done = false;              //trigger是否已触发过一次
    QString m_errorString;
    QDateTime m_triggeredTime;        //上一次触发时间
    QString m_tipMsg;                 //tip消息
    bool m_lastActionSuccess = false;  //上一次触发是否成功
};



#endif // CTRIGGER_H
