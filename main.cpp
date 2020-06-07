#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "src/api.h"
#include "src/qml/search_model.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<timetable::ApiJSON>("lib",1,0,"Api");
    qmlRegisterType<timetable::SearchItemModel>("lib",1,0,"BaseSearchModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();

}
