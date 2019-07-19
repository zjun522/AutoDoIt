#include "cstartstoptaskaction.h"
#include "Windows.h"
#include "mainwindow.h"

extern MainWindow *g_mw;

CStartStopTaskAction::CStartStopTaskAction(bool start, ulong delayBefore, ulong delayAfter) :
    CAction(delayBefore, delayAfter, QStringLiteral(":/image/checked.png")),
    m_start(start)
{
    qDebug() << "CStartStopTaskAction()";
}

CStartStopTaskAction::~CStartStopTaskAction()
{
    qDebug() << "~CStartStopTaskAction()";
}

QString CStartStopTaskAction::getActionName() const
{
    if(m_start)
        return QStringLiteral("启动任务");

    return QStringLiteral("停止任务");
}

bool CStartStopTaskAction::doIt()
{
    if(m_delayBefore)
        Sleep(m_delayBefore * 1000);

    if(m_start) {
        g_mw->on_action_startAllTask_triggered();
    }
    else {
        g_mw->on_action_stopAllTask_triggered();
    }
    return CAction::doIt();
}

bool CStartStopTaskAction::getStart() const
{
    return m_start;
}

QDataStream &CStartStopTaskAction::operator <<(QDataStream &stream)
{
    CAction::operator <<(stream);
    stream >> m_start;
    return stream;
}

QDataStream &CStartStopTaskAction::operator >>(QDataStream &stream) const
{
    CAction::operator >>(stream);
    stream << m_start;
    return stream;
}
