#ifndef CSSLSERVER_H
#define CSSLSERVER_H

#include <QTcpServer>
#include <QSslError>
#include <QSslSocket>

class QTimer;

//#define ENCRYPTED
#define DBG qDebug() << QString("[DBG] <FILE>%1 <FUNC>%2() <LINE>%3 ==>").arg(QString(__FILE__), QString(__FUNCTION__), QString(__LINE__))

class CSslServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CSslServer(QObject *parent = nullptr);
    ~CSslServer() final;
    Q_DISABLE_COPY_MOVE(CSslServer)

signals:
    void receiveData(const QByteArray&, QString);

public slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void socketError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);
    void socketReadyRead();
    void onNewConnection();

protected:
//    void incomingConnection(qintptr socketDescriptor);
};

#endif // CSSLSERVER_H
