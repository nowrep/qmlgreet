#pragma once

#include <QUrl>
#include <QObject>

class Ipc;
class Session;

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QUrl backgroundSrc READ backgroundSrc CONSTANT)
    Q_PROPERTY(bool sessionInProgress READ sessionInProgress NOTIFY sessionInProgressChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QString user() const;
    void setUser(const QString &user);

    QUrl backgroundSrc() const;
    void setBackgroundSrc(const QUrl &url);

    bool sessionInProgress() const;

    void setCommand(const QString &command);

    Q_INVOKABLE bool login(const QString &password);

    static Backend *instance();

Q_SIGNALS:
    void userChanged();
    void sessionInProgressChanged();

    void sessionSuccess();
    void sessionError(const QString &type, const QString &description);
    void infoMessage(const QString &message);
    void errorMessage(const QString &message);

private:
    QString m_user;
    QUrl m_backgroundSrc;
    QString m_command;
    Ipc *m_ipc = nullptr;
    Session *m_session = nullptr;
};
