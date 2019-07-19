#include "cmessageaction.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QIcon>
#include <Windows.h>
#include "mainwindow.h"

extern MainWindow *g_mw;

CMessageAction::CMessageAction(QString message, EMsgMode msgMode, ulong delayBefore, ulong delayAfter) :
    CAction(delayBefore, delayAfter, QStringLiteral(":/image/info.png")),
    m_message(std::move(message)),
    m_msgMode(msgMode)
{
    qDebug() << "CMessageAction()" << this;
}

CMessageAction::~CMessageAction()
{
    qDebug() << "~CMessageAction()" << this;
}

QString CMessageAction::getActionName() const
{
    return QStringLiteral("提示消息");
}

bool CMessageAction::doIt()
{
    if(m_delayBefore)
        Sleep(m_delayBefore * 1000);

    switch (m_msgMode) {
    case ETM_Message:
    {
        QMessageBox message(QMessageBox::Information, qAppName(), m_message);
        message.setWindowIcon(QIcon(":/image/Attachment.png"));
        if(g_mw->isVisible())
        {
            if (g_mw->windowState() & Qt::WindowMinimized)
            {
                g_mw->setWindowState(Qt::WindowNoState);
                //            restoreGeometry();
            }
            g_mw->activateWindow();
            g_mw->raise();
//            message.setParent(g_mw);
        }
        message.exec();
    }
        break;
    case ETM_Notify:
        g_mw->showNotify(m_message);
        break;
    }

    return CAction::doIt();
}

CMessageAction::EMsgMode CMessageAction::getMsgMode() const
{
    return m_msgMode;
}

QString CMessageAction::getMessage() const
{
    return m_message;
}


QDataStream &CMessageAction::operator <<(QDataStream &stream)
{
    CAction::operator <<(stream);
    qint8 msgMode = 0;
    stream >> m_message >> msgMode;
    m_msgMode = EMsgMode(msgMode);
    return stream;
}

QDataStream &CMessageAction::operator >>(QDataStream &stream) const
{
    CAction::operator >>(stream);
    stream << m_message << static_cast<qint8>(m_msgMode);
    return stream;
}

