#include "cidletrigger.h"
#include "common/csyswin.h"

CIdleTrigger::CIdleTrigger(quint64 idleSeconds, int duration,
                           ERepeatMode repeatMode ,
                           std::shared_ptr<CAction> action,
                           std::shared_ptr<CCondition> condition,
                           QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), parent),
    m_duration(duration),
    m_idleSeconds(idleSeconds)
{
    qDebug() << "CIdleTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(scanIdle()));
}

CIdleTrigger::~CIdleTrigger()
{
    qDebug() << "~CIdleTrigger()" << this;
}

QString CIdleTrigger::getTriggerName() const
{
    return QStringLiteral("计算机空闲时");
}

void CIdleTrigger::startWatch()
{
    CTrigger::startWatch();
    m_timer.start(m_duration);
}

void CIdleTrigger::stopWatch()
{
    m_timer.stop();
    CTrigger::stopWatch();
}

QDataStream &CIdleTrigger::operator<<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    stream >> m_duration >> m_idleSeconds;
    return stream;
}

QDataStream &CIdleTrigger::operator>>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_duration << m_idleSeconds;
    return stream;
}

void CIdleTrigger::scanIdle()
{
    if(CSysWin::getInstance()->getIdleTime() >= m_idleSeconds)
        triggered();
}

quint64 CIdleTrigger::getIdleSeconds() const
{
    return m_idleSeconds;
}
