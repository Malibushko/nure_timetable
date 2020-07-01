#pragma once
#include <QTranslator>
#include <QApplication>
#include <QObject>
#include <QDebug>
namespace timetable {
class LanguageSwitcher : public QObject {
    Q_OBJECT
private:
    QTranslator language;
    QString langStr;
public:
    LanguageSwitcher(QObject * parent = nullptr) : QObject{parent} {
        qApp->installTranslator(&language);
    }
    Q_INVOKABLE void setLanguage(const QString& lang) {
        if (lang == "English") {
            language.load("en");
            langStr = "en";
        }
        else if (lang == "Русский") {
            language.load("ru");
            langStr = "ru";
        }
    }
    Q_INVOKABLE QString getLanguage() const {
        if (langStr == "en")
            return "English";
        else
            return "Русский";
    }
};
}
