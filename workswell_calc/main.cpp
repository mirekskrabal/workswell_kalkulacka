#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <clocale>
#include "exprparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    std::setlocale(LC_NUMERIC,"C");

    QQmlApplicationEngine engine;
    QQmlContext* context(engine.rootContext());
    context->setContextProperty("parser", new ExprParser());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
