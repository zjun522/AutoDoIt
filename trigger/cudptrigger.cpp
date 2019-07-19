#include "cudptrigger.h"

namespace {
constexpr quint16 g_port = 20023;
}

QUdpSocket CUDPTrigger::m_udpSocket;
QMultiMap<CUDPTrigger*, SNetCmd> CUDPTrigger::m_triggerMap;

CUDPTrigger::CUDPTrigger(QObject *parent) :
    CTrigger(parent)
{
    qDebug() << "CUDPTrigger()" << this;
//    m_triggerMap.insert(this, m_netCmd); //NOTE move to line 77
    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}

CUDPTrigger::CUDPTrigger(const QString &remoteHost, const QString &command, CTrigger::ERepeatMode repeatMode,
                         std::shared_ptr<CAction> action,
                         std::shared_ptr<CCondition> condition,
                         QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), parent),
    m_netCmd(SNetCmd(remoteHost, 0, command))
{
    qDebug() << "CUDPTrigger()" << this;
    m_triggerMap.insert(this, m_netCmd);
    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}

CUDPTrigger::~CUDPTrigger()
{
    m_triggerMap.remove(this, m_netCmd);
    if(m_triggerMap.count() < 1)
        m_udpSocket.close();
    qDebug() << "~CUDPTrigger()" << this;
}

QString CUDPTrigger::getTriggerName() const
{
    return QStringLiteral("收到UDP指令后");
}

void CUDPTrigger::startWatch()
{
    CTrigger::startWatch();
    if(m_udpSocket.state() & QAbstractSocket::BoundState)
        return;
    if(!m_udpSocket.bind(QHostAddress::Any, g_port, QAbstractSocket::DontShareAddress))
    {
        m_errorString = m_udpSocket.errorString();
        qDebug() << m_errorString;
        stopWatch();
    }
}

void CUDPTrigger::stopWatch()
{
    CTrigger::stopWatch();
}

void CUDPTrigger::readData()
{
    QByteArray array;
    QHostAddress address;
    array.resize(static_cast<int>(m_udpSocket.bytesAvailable()));
    if(-1 == m_udpSocket.readDatagram(array.data(), array.size(), &address))
        return;
    qDebug() << QString(array) << address.toString();
    QString hostName = address.toString();
    if(hostName.startsWith(QStringLiteral("::ffff:")))
        hostName.remove(QStringLiteral("::ffff:"));
    auto triggers = m_triggerMap.keys(SNetCmd(hostName, 0, QString(array)));
    foreach (CTrigger *trigger, triggers) {
        if(trigger && trigger->getIsWatch())
            trigger->triggered();
    }
}

QDataStream &CUDPTrigger::operator <<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    stream >> m_netCmd;
    m_triggerMap.insert(this, m_netCmd); //NOTE move from line 77
    return stream;
}

QDataStream &CUDPTrigger::operator >>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_netCmd;
    return stream;
}

SNetCmd CUDPTrigger::getNetCmd() const
{
    return m_netCmd;
}

