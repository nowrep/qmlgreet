#pragma once

#include <QObject>

class Ipc;
class IpcReply;

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(Ipc *ipc, QObject *parent = nullptr);

    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setCommand(const QStringList &command);

    void start();

Q_SIGNALS:
    void success();
    void error(const QString &errorType, const QString &description);
    void infoMessage(const QString &message);
    void errorMessage(const QString &message);

private:
    void addRequest(IpcReply *reply);
    void replyFinished(IpcReply *reply);

    Ipc *m_ipc = nullptr;
    QString m_username;
    QString m_password;
    QStringList m_command;
};
