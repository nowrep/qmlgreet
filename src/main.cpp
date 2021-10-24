#include "backend.h"

#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>

static QObject *backend_singleton(QQmlEngine *engine, QJSEngine *)
{
    return Backend::instance();
}

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_EGLFS_HIDECURSOR", "1");
    qputenv("QT_QPA_EGLFS_KMS_ATOMIC", "1");

    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption userOption({QStringLiteral("u"), QStringLiteral("user")},
                                   QStringLiteral("User."), QStringLiteral("user"));
    QCommandLineOption iconsOption({QStringLiteral("i"), QStringLiteral("icons")},
                                    QStringLiteral("User icons directory."), QStringLiteral("path"));
    QCommandLineOption backgroundOption({QStringLiteral("b"), QStringLiteral("background")},
                                       QStringLiteral("Background."), QStringLiteral("path"));
    QCommandLineOption commandOption({QStringLiteral("c"), QStringLiteral("command")},
                                      QStringLiteral("Command."), QStringLiteral("command"));

    parser.addOption(userOption);
    parser.addOption(iconsOption);
    parser.addOption(backgroundOption);
    parser.addOption(commandOption);

    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    if (parser.isSet(userOption)) {
        Backend::instance()->setUser(parser.value(userOption));
    }
    if (parser.isSet(iconsOption)) {
        Backend::instance()->setIconsSrc(QUrl::fromLocalFile(parser.value(iconsOption)));
    }
    if (parser.isSet(backgroundOption)) {
        Backend::instance()->setBackgroundSrc(QUrl::fromLocalFile(parser.value(backgroundOption)));
    }
    if (parser.isSet(commandOption)) {
        Backend::instance()->setCommand(parser.value(commandOption));
    } else {
        std::cerr << "Command not set" << std::endl;
        return 1;
    }

    const char *uri = "cz.nowrep.qmlgreet";
    qmlRegisterSingletonType<Backend>(uri, 1, 0, "Backend", backend_singleton);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:qml/main.qml")));

    return app.exec();
}
