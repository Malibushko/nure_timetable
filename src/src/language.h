#pragma once
#include <QTranslator>
#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>
namespace timetable {
/**
 * @brief The LanguageSwitcher class
 *
 * Helper class that performs language switching in application
 */
class LanguageSwitcher : public QObject {
    Q_OBJECT
private:
    QTranslator m_language;
    QString m_langStr; /// contains string representation of current language
public:
    /**
     * @brief Default ctor
     * Calls QApplication::installTranslator with QTranslator object
     */
    LanguageSwitcher(QObject * parent = nullptr) : QObject{parent} {
        qApp->installTranslator(&m_language);
    }
    /**
     * @brief Changes language and retranslates the app
     * @param lang Language name
     */
    Q_INVOKABLE void setLanguage(const QString& lang) {
        if (lang == "English") {
            m_langStr = ":/languages/en.qm";
        }
        else {
            m_langStr = ":/languages/ru.qm";
        }
        m_language.load(m_langStr);
        retranslate();
    }
    /**
     * @brief Retranslate the app
     * \note Retranslate means repaint all translation dependent names
     */
    Q_INVOKABLE void retranslate() {
        QQmlContext *currentContext = QQmlEngine::contextForObject(this);
        QQmlEngine *engine = currentContext->engine();
        engine->retranslate();
    }
    /**
     * @brief Returns string representation of current language
     * @return
     */
    Q_INVOKABLE QString getLanguage() const {
        if (m_langStr == "en")
            return "English";
        else
            return "Русский";
    }
signals:
    /// Signal is emited when language changes
    void languageChanged();
};
}
