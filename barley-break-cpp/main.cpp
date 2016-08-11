#include <QtQml>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "logic.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterSingletonType<Logic>("Logic_CPP", 1, 0, "Logic", &Logic::singletone_provider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
