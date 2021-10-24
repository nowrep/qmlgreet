#include "backend.h"
#include "ipc.h"
#include "session.h"

#include <QDebug>
#include <QProcess>

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_ipc(new Ipc(this))
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

QUrl Backend::backgroundSrc() const
{
    return m_backgroundSrc;
}

void Backend::setBackgroundSrc(const QUrl &url)
{
    m_backgroundSrc = url;
}

bool Backend::sessionInProgress() const
{
    return m_session;
}

void Backend::setCommand(const QString &command)
{
    m_command = command;
}

bool Backend::login(const QString &password)
{
    if (m_session) {
        qWarning() << "Another session in progress!";
        return false;
    }

    m_session = new Session(m_ipc, this);
    m_session->setUsername(m_user);
    m_session->setPassword(password);
    m_session->setCommand(QProcess::splitCommand(m_command));

    connect(m_session, &Session::success, this, [this]() {
        m_session = nullptr;
        Q_EMIT sessionSuccess();
        Q_EMIT sessionInProgressChanged();
    });

    connect(m_session, &Session::error, this, [this](const QString &errorType, const QString &description) {
        m_session = nullptr;
        Q_EMIT sessionError(errorType, description);
        Q_EMIT sessionInProgressChanged();
    });

    connect(m_session, &Session::infoMessage, this, &Backend::infoMessage);
    connect(m_session, &Session::errorMessage, this, &Backend::errorMessage);

    m_session->start();
    Q_EMIT sessionInProgressChanged();

    return true;
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
