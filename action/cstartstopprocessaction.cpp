#include "cstartstopprocessaction.h"
#include <Windows.h>
#include <QProcess>
#include <utility>
#include "common/csyswin.h"


CStartStopProcessAction::CStartStopProcessAction(QString program, QStringList arguments, bool start, ulong delayBefore, ulong delayAfter) :
    CAction(delayBefore, delayAfter, QStringLiteral(":/image/music.png")),
    m_program(std::move(program)),
    m_arguments(std::move(arguments)),
    m_start(start)
{
    qDebug() << "CStartProcessAction()" << this;
}

CStartStopProcessAction::~CStartStopProcessAction()
{
    qDebug() << "~CStartProcessAction()" << this;
}

QString CStartStopProcessAction::getActionName() const
{
    return QStringLiteral("%1“%2”").arg(m_start ? QStringLiteral("启动") : QStringLiteral("停止"), m_program);
}

bool CStartStopProcessAction::doIt()
{
    if(m_delayBefore)
        Sleep(m_delayBefore * 1000);
    if(m_start) {
        QProcess process;
        process.setProgram(m_program);
        process.setArguments(m_arguments);
        if(!process.startDetached()) {
            m_errorString = process.errorString();
            return false;
        }
    }
    else {
        if(CSysWin::getInstance()->killProcess(m_program) != 0) {
            m_errorString = QStringLiteral("关闭进程失败");
            return false;
        }
    }

    return CAction::doIt();
}

QString CStartStopProcessAction::getProcessName() const
{
    return m_program + " " + m_arguments.join(QStringLiteral(" "));
}

bool CStartStopProcessAction::getStart() const
{
    return m_start;
}

QDataStream &CStartStopProcessAction::operator <<(QDataStream &stream)
{
    CAction::operator <<(stream);
    stream >> m_program >> m_arguments >> m_start;
    return stream;
}

QDataStream &CStartStopProcessAction::operator >>(QDataStream &stream) const
{
    CAction::operator >>(stream);
    stream << m_program << m_arguments << m_start;
    return stream;
}
