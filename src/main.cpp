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
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption userOption({QStringLiteral("u"), QStringLiteral("user")},
                                   QStringLiteral("User."), QStringLiteral("user"));
    QCommandLineOption backgroundOption({QStringLiteral("b"), QStringLiteral("background")},
                                       QStringLiteral("Background."), QStringLiteral("path"));
    QCommandLineOption commandOption({QStringLiteral("c"), QStringLiteral("command")},
                                      QStringLiteral("Command."), QStringLiteral("command"));

    parser.addOption(userOption);
    parser.addOption(backgroundOption);
    parser.addOption(commandOption);

    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    if (parser.isSet(userOption)) {
        Backend::instance()->setUser(parser.value(userOption));
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