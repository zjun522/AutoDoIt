#include "ctimertrigger.h"

#include <utility>

CTimerTrigger::CTimerTrigger(QObject *parent) :
    CTrigger(parent)
{
    qDebug() << "CTimerTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(triggered()));
}

CTimerTrigger::CTimerTrigger(QTime time, ETimerMode timerMode,
                             ERepeatMode repeatMode,
                             std::shared_ptr<CAction> action,
                             std::shared_ptr<CCondition> condition, QString taskName,
                             QObject *parent) :
    CTrigger(repeatMode, true, std::move(action), std::move(condition), std::move(taskName), parent),
    m_time(time),
    m_timerMode(timerMode)
{
    qDebug() << "CTimerTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(triggered()));
}

CTimerTrigger::~CTimerTrigger()
{
    qDebug() << "~CTimerTrigger()" << this;
}

QString CTimerTrigger::getTriggerName() const
{
    QString str;
    switch (m_timerMode) {
    case ETimerMode::ETM_Timer:
    {
        str = m_time.toString(QStringLiteral("HH:mm(时:分)时"));
    }
        break;
    case ETimerMode::ETM_CountDown:
    {
        str = m_time.toString(QStringLiteral("HH小时mm分种后"));
    }
        break;
    }
    return str;
}

void CTimerTrigger::startWatch()
{
    CTrigger::startWatch();
    switch (m_timerMode) {
    case ETimerMode::ETM_Timer:
    {
        QDateTime dateTime;
        dateTime.setDate(QDate::currentDate());
        dateTime.setTime(m_time);
        const QDateTime &curDT = QDateTime::currentDateTime();
        if(curDT.time().msecsTo(m_time) < 0)
        {
            dateTime.setDate(curDT.addDays(1).date());
        }
        m_timer.start(static_cast<int>(curDT.msecsTo(dateTime)));
    }
        break;
    case ETimerMode::ETM_CountDown:
    {
        int sec = m_time.hour() * 3600 + m_time.minute() * 60 + m_time.second();
        m_timer.start(sec * 1000);
    }
        break;
    }
}

void CTimerTrigger::stopWatch()
{
    m_timer.stop();
    CTrigger::stopWatch();
}

QString CTimerTrigger::getTipMsg()
{
    CTrigger::getTipMsg();
    if(m_isWatch)
    {
        if(!m_tipMsg.isEmpty()) {
            m_tipMsg += QStringLiteral("\n");
        }
        int remain = m_timer.remainingTime()/1000;
        m_tipMsg += QStringLiteral("下次触发：%1时%2分%3秒后").arg(remain/3600).arg(remain/60).arg(remain%60);
    }
    return m_tipMsg;
}

QDataStream &CTimerTrigger::operator <<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    qint8 timeMode = 0;
    stream >> m_time >> timeMode;
    m_timerMode = ETimerMode(timeMode);
    return stream;
}

QDataStream &CTimerTrigger::operator >>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_time << static_cast<qint8>(m_timerMode);
    return stream;
}

//void CTimerTrigger::triggered()
//{
////    switch (m_repeatMode) {
////    case ERepeatMode::EPM_REPEAT:
////        startWatch();
////        break;
////    case ERepeatMode::EPM_ONCE:
////        stopWatch();
////        break;
////    default:
////        Q_ASSERT(false);
////        break;
////    }
//    CTrigger::triggered();
//}

QTime CTimerTrigger::getTime() const
{
    return m_time;
}

CTimerTrigger::ETimerMode CTimerTrigger::getTimerMode() const
{
    return m_timerMode;
}
