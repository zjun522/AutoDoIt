#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QDataStream>

struct SNetCmd
{
    SNetCmd() = default;
    SNetCmd(QString r, quint16 p, QString c) :
        remoteHost(std::move(r)), remotePort(p), command(std::move(c)) {}
    bool operator==(const SNetCmd &udpCmd) const {
        return ((remoteHost == udpCmd.remoteHost || remoteHost == QStringLiteral("0.0.0.0")) &&
                remotePort == udpCmd.remotePort &&
                command == udpCmd.command);
    }
    QString remoteHost;
    quint16 remotePort = 0;
    QString command;
};

static QDataStream &operator<<(QDataStream &stream, const SNetCmd &netCmd)
{
    stream << netCmd.remoteHost << netCmd.remotePort << netCmd.command;
    return stream;
}

static QDataStream &operator>>(QDataStream &stream, SNetCmd &netCmd)
{
    stream >> netCmd.remoteHost >> netCmd.remotePort >> netCmd.command;
    return stream;
}


#endif // DATA_H
