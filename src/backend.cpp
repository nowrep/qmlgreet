#include "backend.h"

#include <QDebug>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
}

QString Backend::user() const
{
    return m_user;
}

void Backend::setUser(const QString &user)
{
    if (m_user == user) {
        return;
    }
    m_user = user;
    Q_EMIT userChanged();
}

QString Backend::password() const
{
    return m_password;
}

void Backend::setPassword(const QString &password)
{
    if (m_password == password) {
        return;
    }
    m_password = password;
    Q_EMIT passwordChanged();
}

QUrl Backend::backgroundSrc() const
{
    return m_backgroundSrc;
}

void Backend::setBackgroundSrc(const QUrl &url)
{
    m_backgroundSrc = url;
}

void Backend::setCommand(const QString &command)
{
    m_command = command;
}

void Backend::login()
{
    qInfo() << "Login" << m_user << QString('*').repeated(m_password.length());
}

// static
Backend *Backend::instance()
{
    static Backend *backend = nullptr;
    if (!backend) {
        backend = new Backend;
    }
    return backend;
}
