#pragma once

#include <QUrl>
#include <QObject>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QUrl backgroundSrc READ backgroundSrc CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);

    QString user() const;
    void setUser(const QString &user);

    QString password() const;
    void setPassword(const QString &password);

    QUrl backgroundSrc() const;
    void setBackgroundSrc(const QUrl &url);

    void setCommand(const QString &command);

    Q_INVOKABLE void login();

    static Backend *instance();

Q_SIGNALS:
    void userChanged();
    void passwordChanged();

private:
    QString m_user;
    QString m_password;
    QUrl m_backgroundSrc;
    QString m_command;
};
