#include "cwatchprocesstrigger.h"
#include <Windows.h>

#include <utility>

#include <utility>
#include "common/csyswin.h"

CWatchProcessTrigger::CWatchProcessTrigger(QObject *parent) :
    CTrigger(parent)
{
    qDebug() << "CWatchProcessTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(scanProcess()));
}

CWatchProcessTrigger::CWatchProcessTrigger(QString processName,
                                           bool running, int duration,
                                           ERepeatMode repeatMode,
                                           std::shared_ptr<CAction> action,
                                           std::shared_ptr<CCondition> condition,
                                           QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), parent),
    m_duration(duration),
    m_processName(std::move(processName)),
    m_running(running)
{
    qDebug() << "CWatchProcessTrigger()" << this;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(scanProcess()));
}

CWatchProcessTrigger::~CWatchProcessTrigger()
{
    qDebug() << "~CWatchProcessTrigger()" << this;
}

QString CWatchProcessTrigger::getTriggerName() const
{
    return QStringLiteral("进程“%1”%2时").arg(m_processName, m_running ? QStringLiteral("运行") : QStringLiteral("退出"));
}

void CWatchProcessTrigger::startWatch()
{
    CTrigger::startWatch();
    m_timer.start(m_duration);
}

void CWatchProcessTrigger::stopWatch()
{
    m_timer.stop();
    CTrigger::stopWatch();
}

QDataStream &CWatchProcessTrigger::operator<<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    stream >> m_duration >> m_processName >> m_running;
    return stream;
}

QDataStream &CWatchProcessTrigger::operator>>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_duration << m_processName << m_running;
    return stream;
}

void CWatchProcessTrigger::scanProcess()
{
    bool curRunning = CSysWin::getInstance()->processIsRunning(m_processName.toLocal8Bit().data());
    if(m_running && curRunning)
    {
        triggered();
    }
    else if(!m_running && !curRunning)
    {
        triggered();
    }
}

bool CWatchProcessTrigger::getRunning() const
{
    return m_running;
}

QString CWatchProcessTrigger::getProcessName() const
{
    return m_processName;
}
