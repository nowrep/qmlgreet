#include "ipc.h"

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>

// IpcReply
IpcReply::IpcReply(QObject *parent)
    : QObject(parent)
{
}

QString IpcReply::type() const
{
    return m_reply.value(QStringLiteral("type")).toString();
}

QString IpcReply::requestType() const
{
    return m_request.value(QStringLiteral("type")).toString();
}

QString IpcReply::errorType() const
{
    return m_reply.value(QStringLiteral("error_type")).toString();
}

QString IpcReply::errorDescription() const
{
    return m_reply.value(QStringLiteral("description")).toString();
}

QString IpcReply::authMessageType() const
{
    return m_reply.value(QStringLiteral("auth_message_type")).toString();
}

QString IpcReply::authMessage() const
{
    return m_reply.value(QStringLiteral("auth_message")).toString();
}

// Ipc
Ipc::Ipc(QObject *parent)
    : QObject(parent)
{
    const QString greetdSock = qEnvironmentVariable("GREETD_SOCK");
    if (greetdSock.isEmpty()) {
        qCritical() << "GREETD_SOCK not set";
        return;
    }

    m_socket = new QLocalSocket(this);
    m_socket->connectToServer(greetdSock);

    connect(m_socket, &QLocalSocket::connected, this, [this]() {
        qInfo() << "Socket connected";
    });

    connect(m_socket, &QLocalSocket::disconnected, this, [this]() {
        qInfo() << "Socket disconnected";
    });

    connect(m_socket, &QLocalSocket::errorOccurred, this, [this](QLocalSocket::LocalSocketError socketError) {
        qWarning() << "Socket error" << socketError;
    });

    connect(m_socket, &QLocalSocket::readyRead, this, &Ipc::readyRead);
}

IpcReply *Ipc::createSession(const QString &username)
{
    Q_ASSERT(!m_reply);
    QVariantMap m;
    m[QStringLiteral("type")] = QStringLiteral("create_session");
    m[QStringLiteral("username")] = username;
    return sendRequest(m);
}

IpcReply *Ipc::postAuthMessageResponse(const QString &response)
{
    Q_ASSERT(!m_reply);
    QVariantMap m;
    m[QStringLiteral("type")] = QStringLiteral("post_auth_message_response");
    m[QStringLiteral("response")] = response;
    return sendRequest(m);
}

IpcReply *Ipc::startSession(const QStringList &cmd)
{
    Q_ASSERT(!m_reply);
    QVariantMap m;
    m[QStringLiteral("type")] = QStringLiteral("start_session");
    m[QStringLiteral("cmd")] = cmd;
    return sendRequest(m);
}

IpcReply *Ipc::cancelSession()
{
    Q_ASSERT(!m_reply);
    QVariantMap m;
    m[QStringLiteral("type")] = QStringLiteral("cancel_session");
    return sendRequest(m);
}

IpcReply *Ipc::sendRequest(const QVariantMap &m)
{
    Q_ASSERT(!m_reply);
    m_reply = new IpcReply(this);
    m_reply->m_request = m;
    QByteArray data(QJsonDocument::fromVariant(m).toJson(QJsonDocument::Compact));
    const qint32 size = data.size();
    data.prepend(4, 0);
    memcpy(data.data(), &size, sizeof(size));
    m_socket->write(data);
    return m_reply;
}

void Ipc::readyRead()
{
    if (m_length == -1) {
        if (m_socket->bytesAvailable() < 4) {
            return;
        }
        m_socket->read(reinterpret_cast<char*>(&m_length), 4);
    }

    if (m_socket->bytesAvailable() < m_length) {
        return;
    }

    const QByteArray payload = m_socket->read(m_length);
    m_length = -1;

    if (!m_reply) {
        qCritical() << "Received reply without sending request!";
        return;
    }

    IpcReply *reply = m_reply;
    m_reply = nullptr;

    reply->m_reply = QJsonDocument::fromJson(payload).toVariant().toMap();
    reply->deleteLater();
    Q_EMIT reply->finished(reply);
}
