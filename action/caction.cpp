#include "caction.h"
#include <Windows.h>

CAction::CAction(ulong delayBefore, ulong delayAfter, QString icon) :
    m_delayBefore(delayBefore),
    m_delayAfter(delayAfter),
    m_icon(std::move(icon))
{

}

bool CAction::doIt()
{
    if(m_delayAfter)
        Sleep(m_delayAfter * 1000);
    return true;
}

ulong CAction::getDelayBefore() const
{
    return m_delayBefore;
}

ulong CAction::getDelayAfter() const
{
    return m_delayAfter;
}

QString CAction::getIcon() const
{
    return m_icon;
}

QString CAction::getErrorString() const
{
    return m_errorString;
}

QDataStream &CAction::operator <<(QDataStream &stream)
{
    quint64 delayBefore, delayAfter;
    stream >> delayBefore >> delayAfter >> m_icon >> m_errorString;
    m_delayBefore = static_cast<ulong>(delayBefore);
    m_delayAfter = static_cast<ulong>(delayAfter);
    return stream;
}

QDataStream &CAction::operator >>(QDataStream &stream) const
{
    stream << QString(typeid(*this).name()) << static_cast<quint64>(m_delayBefore) << static_cast<quint64>(m_delayAfter) << m_icon << m_errorString;
    return stream;
}


