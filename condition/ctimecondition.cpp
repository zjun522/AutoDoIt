#include "ctimecondition.h"

#include <utility>

CTimeCondition::CTimeCondition()
{
    qDebug() << "CTimeCondition()" << this;
}

CTimeCondition::CTimeCondition(QTime time, ETimeType timeType) :
    m_time(time),
    m_timeType(timeType)
{
    qDebug() << "CTimeCondition()" << this;
}

CTimeCondition::~CTimeCondition()
{
    qDebug() << "~CTimeCondition()" << this;
}

QString CTimeCondition::getConditionName() const
{
    QString str = QStringLiteral("在%1").arg(m_time.toString(QStringLiteral("HH:mm")));
    switch (m_timeType) {
    case ETimeType::ETT_BEFORE:
    {
        str += QStringLiteral("之前");
    }
        break;
    case ETimeType::ETT_AFTER:
    {
        str += QStringLiteral("之后");
    }
        break;
    }
    return str;
}

bool CTimeCondition::getStatus() const
{
    const qint64 &secs = QTime::currentTime().secsTo(m_time);
    if(m_timeType == ETimeType::ETT_BEFORE)
    {
        return secs >= 0;
    }
    if(m_timeType == ETimeType::ETT_AFTER)
    {
        return secs <= 0;
    }
    return false;
}

QDataStream &CTimeCondition::operator <<(QDataStream &stream)
{
    CCondition::operator <<(stream);
    qint8 timeType = 0;
    stream >> m_time >> timeType;
    m_timeType = ETimeType(timeType);


    return stream;
}

QDataStream &CTimeCondition::operator >>(QDataStream &stream) const
{
    CCondition::operator >>(stream);
    stream << m_time << static_cast<qint8>(m_timeType);
    return stream;
}

CTimeCondition::ETimeType CTimeCondition::getTimeType() const
{
    return m_timeType;
}

QTime CTimeCondition::getTime() const
{
    return m_time;
}

