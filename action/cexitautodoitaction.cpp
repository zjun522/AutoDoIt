#include "cexitautodoitaction.h"
#include <Windows.h>
#include "mainwindow.h"

extern MainWindow *g_mw;

CExitAutoDoItAction::CExitAutoDoItAction(ulong delayBefore, ulong delayAfter) :
    CAction(delayBefore, delayAfter, QStringLiteral(":/image/poweroff.png"))
{
    qDebug() << "CExitAutoDoItAction()";
}

CExitAutoDoItAction::~CExitAutoDoItAction()
{
    qDebug() << "~CExitAutoDoItAction()";
}

QString CExitAutoDoItAction::getActionName() const
{
    return QStringLiteral("退出AutoDoIt");
}

bool CExitAutoDoItAction::doIt()
{
    if(m_delayBefore)
        Sleep(m_delayBefore * 1000);

    g_mw->on_action_quit_triggered();
    return true;
}
