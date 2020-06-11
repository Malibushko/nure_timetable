#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "src/api.h"
#include "src/qml/search_model.h"
#include "src/cache.h"
#include "src/qml/table_model.h"
#include "src/src/database.h"
int main(int argc, char *argv[])
{
    QGuiApplication p(argc,argv);
    qRegisterMetaType<Promise>();
    using namespace timetable;
    using namespace timetable::internal;
    auto& instance = Database::instance();
    instance.Delete(Database::TableType::SEARCH_GROUP,9393939);
    p.exec();
    /*
    using namespace timetable;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<timetable::ApiJSON>("lib",1,0,"Api");
    qmlRegisterType<timetable::SearchItemModel>("lib",1,0,"BaseSearchModel");
    qmlRegisterType<timetable::TableModel>("lib",1,0,"TableModel");

   // qRegisterMetaType<timetable::Cache::TableType>("Cache::CacheType");
    using Kek = Kek<int>;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
*/
}
