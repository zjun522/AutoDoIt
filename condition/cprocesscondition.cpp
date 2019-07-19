#include "cprocesscondition.h"

#include <utility>
//#include <Windows.h>
#include "common/csyswin.h"

CProcessCondition::CProcessCondition()
{
    qDebug() << "CProcessCondition()" << this;
}

CProcessCondition::CProcessCondition(QString processName, bool running) :
    m_processName(std::move(processName)),
    m_running(running)
{
    qDebug() << "CProcessCondition()" << this;
}

CProcessCondition::~CProcessCondition()
{
    qDebug() << "~CProcessCondition()" << this;
}

QString CProcessCondition::getConditionName() const
{
    return QStringLiteral("进程“%1”%2").arg(m_processName, m_running ? QStringLiteral("运行") : QStringLiteral("退出"));
}

bool CProcessCondition::getStatus() const
{
    bool curRunning = CSysWin::getInstance()->processIsRunning(m_processName.toLocal8Bit().data());
    return (curRunning && m_running) ||
            (!curRunning && !m_running);
}

QDataStream &CProcessCondition::operator <<(QDataStream &stream)
{
    CCondition::operator <<(stream);
    stream >> m_processName >> m_running;
    return stream;
}

QDataStream &CProcessCondition::operator >>(QDataStream &stream) const
{
    CCondition::operator >>(stream);
    stream << m_processName << m_running;
    return stream;
}

QString CProcessCondition::getProcessName() const
{
    return m_processName;
}

bool CProcessCondition::getRunning() const
{
    return m_running;
}
