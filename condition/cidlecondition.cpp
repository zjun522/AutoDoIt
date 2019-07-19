#include "cidlecondition.h"
#include "common/csyswin.h"

CIdleCondition::CIdleCondition(quint64 idleSeconds) :
    m_idleSeconds(idleSeconds)
{
    qDebug() << "CIdleCondition" << this;
}

CIdleCondition::~CIdleCondition()
{
    qDebug() << "~CIdleCondition" << this;
}

QString CIdleCondition::getConditionName() const
{
    return QStringLiteral("计算机空闲");
}

bool CIdleCondition::getStatus() const
{
    return CSysWin::getInstance()->getIdleTime() >= m_idleSeconds;
}

QDataStream &CIdleCondition::operator <<(QDataStream &stream)
{
    CCondition::operator <<(stream);
    stream >> m_idleSeconds;
    return stream;
}

QDataStream &CIdleCondition::operator >>(QDataStream &stream) const
{
    CCondition::operator >>(stream);
    stream << m_idleSeconds;
    return stream;
}

quint64 CIdleCondition::getIdleSeconds() const
{
    return m_idleSeconds;
}
