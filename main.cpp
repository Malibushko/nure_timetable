#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QDebug>

#include "src/api.h"
#include "src/qml/table_model.h"
#include "src/qml/saved_model.h"
#include "src/qml/search_model.h"
#include "src/src/local_storage.h"
#include "src/qml/search_filter_model.h"
#include "src/qml/settings_group_model.h"
#include "src/src/settings.h"
#include "src/src/language.h"

int main(int argc, char *argv[])
{
    qDebug();
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc,argv);
    app.setWindowIcon(QIcon(":/qml/icons/app_icon.svg"));

    QGuiApplication::setOrganizationName("Timetable");
    QGuiApplication::setApplicationName("Timetable");
    QQmlApplicationEngine engine;
    qRegisterMetaType<Promise>();
    qmlRegisterType<timetable::ApiJSON>("lib",1,0,"Api");
    qmlRegisterType<timetable::SearchItemModel>("lib",1,0,"BaseSearchModel");
    qmlRegisterType<timetable::TableModel>("lib",1,0,"TableModel");
    qmlRegisterType<timetable::LocalStorage>("lib",1,0,"Storage");;
    qmlRegisterType<timetable::SettingsGroupModel>("lib",1,0,"SettingsGroupModel");
    qmlRegisterType<timetable::SavedTimetableModel>("lib",1,0,"SaveModel");
    qmlRegisterType<timetable::SortFilterModel>("lib",1,0,"SearchFilterModel");
    qmlRegisterType<timetable::Settings>("lib",1,0,"Settings");
    qmlRegisterType<timetable::LanguageSwitcher>("lib",1,0,"LanguageSwitcher");

    qRegisterMetaType<timetable::TableType>("TableType");

    qmlRegisterUncreatableMetaObject(
                timetable::staticMetaObject,
                "lib",
                1, 0,
                "TableType",
                "Error: only enums"
                );
    qmlRegisterUncreatableMetaObject(
                SETTINGS_TYPE::staticMetaObject,
                "lib",
                1, 0,
                "SETTINGS_TYPE",
                "Error: only enums"
                );
    qmlRegisterUncreatableMetaObject(
                SETTINGS_GROUP::staticMetaObject,
                "lib",
                1, 0,
                "SETTINGS_GROUP",
                "Error: only enums"
                );
    qmlRegisterUncreatableMetaObject(
                CONTROL_TYPE::staticMetaObject,
                "lib",
                1, 0,
                "CONTROL",
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
