#include "session.h"
#include "ipc.h"

#include <QDebug>

Session::Session(Ipc *ipc, QObject *parent)
    : QObject(parent)
    , m_ipc(ipc)
{
}

void Session::setUsername(const QString &username)
{
    m_username = username;
}

void Session::setPassword(const QString &password)
{
    m_password = password;
}

void Session::setCommand(const QStringList &command)
{
    m_command = command;
}

void Session::start()
{
    qInfo() << "Start session" << m_username << QString('*').repeated(m_password.size());
    addRequest(m_ipc->createSession(m_username));
}

void Session::addRequest(IpcReply *reply)
{
    connect(reply, &IpcReply::finished, this, &Session::replyFinished);
}

void Session::replyFinished(IpcReply *reply)
{
    if (reply->type() == QLatin1String("error")) {
        qWarning() << reply->requestType() << "error" << reply->errorType() << reply->errorDescription();
        Q_EMIT error(reply->errorType(), reply->errorDescription());
        addRequest(m_ipc->cancelSession());
        return;
    }

    if (reply->type() == QLatin1String("success")) {
        if (reply->requestType() == QLatin1String("create_session") || reply->requestType() == QLatin1String("post_auth_message_response")) {
            addRequest(m_ipc->startSession(m_command));
            return;
        }
        if (reply->requestType() == QLatin1String("start_session")) {
            Q_EMIT success();
        }
        deleteLater();
        return;
    }

    if (reply->type() == QLatin1String("auth_message")) {
        if (reply->authMessageType() == QLatin1String("visible")) {
            qWarning() << "Unhandled visible auth message type!";
            Q_EMIT error(QStringLiteral("not_implemented"), QStringLiteral("Auth message 'visible' not implemented."));
            addRequest(m_ipc->cancelSession());
        } else if (reply->authMessageType() == QLatin1String("secret")) {
            addRequest(m_ipc->postAuthMessageResponse(m_password));
        } else if (reply->authMessageType() == QLatin1String("info")) {
            qInfo() << "Info" << reply->authMessage();
            Q_EMIT infoMessage(reply->authMessage());
            // FIXME: What now?
        } else if (reply->authMessageType() == QLatin1String("error")) {
            qInfo() << "Error" << reply->authMessage();
            Q_EMIT errorMessage(reply->authMessage());
            // FIXME: What now?
        }
        return;
    }
}
