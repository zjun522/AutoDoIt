#include <QSslCipher>
#include <QDebug>
#include "csslserver.h"

CSslServer::CSslServer(QObject *parent)
    : QTcpServer(parent)
{
    qDebug() << "CSslServer()";
    connect(this, &QTcpServer::newConnection, this, &CSslServer::onNewConnection);
}

CSslServer::~CSslServer()
{
    qDebug() << "~CSslServer()";
}

void CSslServer::socketStateChanged(QAbstractSocket::SocketState state)
{
    Q_UNUSED(state)
#ifdef ENCRYPTED
    QSslSocket *socket = static_cast<QSslSocket *>(sender());
    if(state == QAbstractSocket::ConnectedState)
        socket->startServerEncryption();
#endif
}

void CSslServer::socketEncrypted()
{
#ifdef ENCRYPTED
    QSslSocket *socket = static_cast<QSslSocket *>(sender());
    const QSslCipher cipher = socket->sessionCipher();
    const QString cipherInfo = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod())
                                       .arg(cipher.name()).arg(cipher.usedBits())
                                       .arg(cipher.supportedBits());;
    DBG << "socketEncrypted " << cipherInfo;
#endif
}

void CSslServer::socketError(QAbstractSocket::SocketError error)
{
#ifdef ENCRYPTED
    QSslSocket *socket = static_cast<QSslSocket *>(sender());
#else
    auto *socket = qobject_cast<QTcpSocket *>(sender());
#endif
    DBG << "socket error " << error << socket->errorString();
}

void CSslServer::sslErrors(const QList<QSslError> &errors)
{
    DBG << "sslErrors";
    for (const auto &error : errors)
        DBG << error.errorString();

#ifdef ENCRYPTED
    QSslSocket *socket = static_cast<QSslSocket *>(sender());
    socket->ignoreSslErrors();
#endif
}

void CSslServer::socketReadyRead()
{
#ifdef ENCRYPTED
    QSslSocket *socket = static_cast<QSslSocket *>(sender());
#else
    auto *socket = qobject_cast<QTcpSocket *>(sender());
#endif
    const QByteArray &content = socket->readAll();
    emit receiveData(content, socket->peerAddress().toString());
//    socket->disconnectFromHost();
}

void CSslServer::onNewConnection()
{
    QTcpSocket *clientConnection = this->nextPendingConnection();
#ifdef ENCRYPTED
        clientConnection->setPrivateKey(":/ca/server.key");
        clientConnection->setLocalCertificate(":/ca/server.crt");
        clientConnection->setPeerVerifyMode(QSslSocket::VerifyNone);
        connect(clientConnection, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
        connect(clientConnection, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
#endif
//        connect(clientConnection, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(clientConnection, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
        connect(clientConnection, &QAbstractSocket::disconnected,
                clientConnection, &QObject::deleteLater);

#ifdef ENCRYPTED
        serverSocket->startServerEncryption();
        DBG << "socket mode" << serverSocket->mode();
        DBG << "ssl verify mode" << serverSocket->peerVerifyMode();
        DBG << "ssl verify name" << serverSocket->peerVerifyName();
#endif
}
