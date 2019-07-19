#include "chttptrigger.h"

namespace {
constexpr quint16 g_port = 20022;
}

CSslServer CHTTPTrigger::m_server;
QMultiMap<CHTTPTrigger*, SNetCmd> CHTTPTrigger::m_triggerMap;

CHTTPTrigger::CHTTPTrigger(QObject *parent) :
    CTrigger(parent)
{
    qDebug() << "CHTTPTrigger()" << this;
//    m_triggerMap.insert(this, m_netCmd); //NOTE move to line 77
    connect(&m_server, SIGNAL(receiveData(const QByteArray&, QString)),
            this, SLOT(readData(const QByteArray&, QString)));
}

CHTTPTrigger::CHTTPTrigger(const QString &remoteHost, const QString &command, CTrigger::ERepeatMode repeatMode,
                         std::shared_ptr<CAction> action,
                         std::shared_ptr<CCondition> condition, QString taskName,
                         QObject *parent) :
    CTrigger(repeatMode, false, std::move(action), std::move(condition), std::move(taskName), parent),
    m_netCmd(SNetCmd(remoteHost, 0, command))
{
    qDebug() << "CHTTPTrigger()" << this;
    connect(&m_server, SIGNAL(receiveData(const QByteArray&, QString)),
            this, SLOT(readData(const QByteArray&, QString)));
    m_triggerMap.insert(this, m_netCmd);
}

CHTTPTrigger::~CHTTPTrigger()
{
    m_triggerMap.remove(this, m_netCmd);
    qDebug() << "~CHTTPTrigger()" << this;
}

QString CHTTPTrigger::getTriggerName() const
{
    return QStringLiteral("收到HTTP指令后");
}

void CHTTPTrigger::startWatch()
{
    CTrigger::startWatch();
    if(m_server.isListening())
        return;
    if(!m_server.listen(QHostAddress::Any, g_port))
    {
        m_errorString = m_server.errorString();
        qDebug() << m_errorString;
        stopWatch();
    }
}

void CHTTPTrigger::stopWatch()
{
    CTrigger::stopWatch();
}

auto parseHttp = [](const QByteArray& http) {
    QTextStream stream(http, QIODevice::ReadOnly);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        //使用者浏览器访问http://127.0.0.1:20022/poweroff 则获取到poweroff
        if(line.startsWith(QStringLiteral("GET /")))
        {
            line.remove(QStringLiteral("GET /"));
            QStringList list = line.split(QStringLiteral(" "));
            if(list.count() > 0)
                return QString(list.at(0));
        }
    }
    return QString();
};

void CHTTPTrigger::readData(const QByteArray &array, QString hostName)
{
    qDebug() << QString(array);
    if(hostName.startsWith(QStringLiteral("::ffff:")))
        hostName.remove(QStringLiteral("::ffff:"));
    qDebug() << parseHttp(array) << hostName;
    auto triggers = m_triggerMap.keys(SNetCmd(hostName, 0, parseHttp(array)));
    foreach (CTrigger *trigger, triggers) {
        if(trigger && trigger->getIsWatch())
            trigger->triggered();
    }
}

QDataStream &CHTTPTrigger::operator <<(QDataStream &stream)
{
    CTrigger::operator<<(stream);
    stream >> m_netCmd;
    m_triggerMap.insert(this, m_netCmd); //NOTE move from line 77
    return stream;
}

QDataStream &CHTTPTrigger::operator >>(QDataStream &stream) const
{
    CTrigger::operator>>(stream);
    stream << m_netCmd;
    return stream;
}

SNetCmd CHTTPTrigger::getNetCmd() const
{
    return m_netCmd;
}

