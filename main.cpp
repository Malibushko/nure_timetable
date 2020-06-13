#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "src/api.h"
#include "src/qml/search_model.h"
#include "src/qml/table_model.h"
#include "src/qml/saved_model.h"
#include "src/qml/settings_model.h"
#include "src/qml/settings_group_model.h"
#include "src/src/local_storage.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);
    QCoreApplication::setOrganizationName("Timetable");
    QCoreApplication::setApplicationName("Timetable");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQmlApplicationEngine engine;
    qRegisterMetaType<Promise>();
    qmlRegisterType<timetable::ApiJSON>("lib",1,0,"Api");
    qmlRegisterType<timetable::SearchItemModel>("lib",1,0,"BaseSearchModel");
    qmlRegisterType<timetable::TableModel>("lib",1,0,"TableModel");
    qmlRegisterType<timetable::LocalStorage>("lib",1,0,"Storage");;
    qmlRegisterType<timetable::SettingsModel>("lib",1,0,"SettingsModel");
    qmlRegisterType<timetable::SettingsGroupModel>("lib",1,0,"SettingsGroupModel");
    qmlRegisterType<timetable::SavedTimetableModel>("lib",1,0,"SaveModel");
    qRegisterMetaType<timetable::TableType>("TableType");

    qmlRegisterUncreatableMetaObject(
      timetable::staticMetaObject,
      "lib",
      1, 0,
      "TableType",
      "Error: only enums"
    );
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
