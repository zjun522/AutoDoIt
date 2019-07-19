#include "cpoweroffaction.h"
#include <QProcess>
#include <QIcon>
#include <QTimer>
#include "common/cmessagebox.h"
#include "common/csyswin.h"
#include "mainwindow.h"

extern MainWindow *g_mw;

//#pragma comment(lib,"user32.lib")

CPowerOffAction::CPowerOffAction(EPowerOffMode powerOffMode, ulong delayBefore, ulong delayAfter) :
    CAction(delayBefore, delayAfter, QStringLiteral(":/image/poweroff.png")),
    m_powerOffMode(powerOffMode)
{
    qDebug() << "CPowerOffAction()" << this;
}

CPowerOffAction::~CPowerOffAction()
{
    qDebug() << "~CPowerOffAction()" << this;
}

QString CPowerOffAction::getActionName() const
{
    switch (m_powerOffMode) {
    case EPOM_PowerOff:
        return QStringLiteral("关机");
    case EPOM_Hibernate:
        return QStringLiteral("休眠");
    case EPOM_Suspend:
        return QStringLiteral("睡眠");
    }
    return QString();
}

bool CPowerOffAction::doIt()
{
    if(m_delayBefore)
        Sleep(m_delayBefore * 1000);
    CMessageBox message(getActionName());
    message.setWindowIcon(QIcon(":/image/Attachment.png"));
    if(QMessageBox::Cancel == message.exec()) {
        return CAction::doIt();
    }

    switch (m_powerOffMode) {
    case EPOM_PowerOff:
    {
        if(!CSysWin::getInstance()->systemShutDown())
        {
            QProcess process;
            process.setProgram(QStringLiteral("shutdown"));
            process.setArguments(QStringList{QStringLiteral("/s"), QStringLiteral("/t 10")});
            if(!process.startDetached()) {
                m_errorString = process.errorString();
                return false;
            }
        }
    }
        break;
    case EPOM_Hibernate:
    {
        if(!CSysWin::getInstance()->systemSuspend(2000, true))
        {
            QProcess process;
            process.setProgram(QStringLiteral("shutdown"));
            process.setArguments(QStringList(QStringLiteral("/h")));
            if(!process.startDetached()) {
                m_errorString = process.errorString();
                return false;
            }
        }
    }
        break;
    case EPOM_Suspend:
    {
        if(!CSysWin::getInstance()->systemSuspend(2000, false))
        {
//            QProcess process;
//            process.setProgram("shutdown");
//            process.setArguments(QStringList("/h"));
//            if(!process.startDetached()) {
//                m_errorString = process.errorString();
//                return false;
//            }
            return false;
        }
    }
        break;
    }
    return true;
}

QDataStream &CPowerOffAction::operator <<(QDataStream &stream)
{
    CAction::operator <<(stream);
    qint8 powerOffMode = 0;
    stream >> powerOffMode;
    m_powerOffMode = EPowerOffMode(powerOffMode);
    return stream;
}

QDataStream &CPowerOffAction::operator >>(QDataStream &stream) const
{
    CAction::operator >>(stream);
    stream << static_cast<qint8>(m_powerOffMode);
    return stream;
}

CPowerOffAction::EPowerOffMode CPowerOffAction::getPowerOffMode() const
{
    return m_powerOffMode;
}
