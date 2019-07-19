#include "ctrigger.h"
#include "../cmakefactory.hpp"
#include "../action/cgroupaction.h"
#include "../condition/cgroupcondition.h"

CTrigger::CTrigger(QObject *parent) :
    QObject(parent)
{
    connect(this, SIGNAL(trigger()), this, SLOT(doIt()));
}

CTrigger::CTrigger(ERepeatMode repeatMode, bool repeatStartWatch,
                   std::shared_ptr<CAction> action,
                   std::shared_ptr<CCondition> condition,
                   QObject *parent) :
    QObject(parent),
    m_action(std::move(action)),
    m_condition(std::move(condition)),
    m_repeatMode(repeatMode),
    m_repeatStartWatch(repeatStartWatch)
{
//    m_action.reset(action);
//    m_condition.reset(condition);
    connect(this, SIGNAL(trigger()), this, SLOT(doIt()));
}

QString CTrigger::getTaskName() const
{
    return getTriggerName() + m_action->getActionName();
}

void CTrigger::triggered()
{
    switch (m_repeatMode) {
    case ERepeatMode::EPM_REPEAT:
        if(m_repeatStartWatch)
            startWatch();
        break;
    case ERepeatMode::EPM_ONCE:
        stopWatch();
        break;
    }
    emit trigger();
}

bool CTrigger::getDone() const
{
    return m_done;
}

bool CTrigger::getLastActionSuccess() const
{
    return m_lastActionSuccess;
}

bool CTrigger::getIsWatch() const
{
    return m_isWatch;
}

QString CTrigger::getTipMsg()
{
    m_tipMsg.clear();
    if(m_action && m_done)
    {
        m_tipMsg = QStringLiteral("最近触发：%1 ").arg(m_triggeredTime.toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")));
        if(!m_lastActionSuccess) {
            m_tipMsg += QStringLiteral("失败\n");
            m_tipMsg += QStringLiteral("错误原因：%1").arg(m_action->getErrorString());
        }
        else {
            m_tipMsg += QStringLiteral("成功");
        }
    }
    return m_tipMsg;
}

std::shared_ptr<CCondition> CTrigger::getCondition() const
{
    return m_condition;
}

std::shared_ptr<CAction> CTrigger::getAction() const
{
    return m_action;
}

CTrigger::ERepeatMode CTrigger::getRepeatMode() const
{
    return m_repeatMode;
}

void CTrigger::doIt()
{
    if(m_repeatMode == ERepeatMode::EPM_ONCE && m_done)
        return;
    doItManual();
}

void CTrigger::doItManual()
{
    if(!m_condition || m_condition->getStatus())
    {
        if(m_action)
        {
            m_lastActionSuccess = m_action->doIt();
            m_triggeredTime = QDateTime::currentDateTime();
            m_done = true;
        }
    }
}

QDataStream &CTrigger::operator <<(QDataStream &stream)
{
    qint8 repeatMode = 0;
    stream >> repeatMode >> m_repeatStartWatch
           >> m_isWatch >> m_done
           >> m_errorString >> m_triggeredTime >> m_tipMsg >> m_lastActionSuccess;
    m_repeatMode = ERepeatMode(repeatMode);

    QString className;
    stream >> className;
    if(className == QString(typeid(CGroupAction).name())){
        m_action = CMakeFactory::makeUnique<CGroupAction>();
        *m_action << stream;
    }

    stream >> className;
    if(className == QString(typeid(CGroupCondition).name()))
    {
        m_condition = CMakeFactory::makeUnique<CGroupCondition>();
        *m_condition << stream;
    }
    return stream;
}

QDataStream &CTrigger::operator >>(QDataStream &stream) const
{
    stream << QString(typeid(*this).name()) << static_cast<qint8>(m_repeatMode) << m_repeatStartWatch
           << m_isWatch << m_done
           << m_errorString << m_triggeredTime << m_tipMsg << m_lastActionSuccess;

    if(m_action)
        *m_action >> stream;
    else
        stream << QStringLiteral("NULL");
    if(m_condition)
        *m_condition >> stream;
    else
        stream << QStringLiteral("NULL");

    return stream;
}

