#pragma once
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include "../internal/api_structs.h"
#include "../internal/material.h"

namespace timetable {
class SettingsModel : public QAbstractListModel {
    Q_OBJECT
    mutable QSettings settings;
    QTranslator language;
private:
    void setIfNotExist(const QString& key,const QVariant& value,bool forceDefault = false);
public:
    SettingsModel(QObject* /* parent */= nullptr);
    Q_INVOKABLE void loadSettings(bool forceDefault = false);
    Q_INVOKABLE QVariant value(const QString& group,
                               const QString& key,
                               const QVariant& defaultValue = QVariant{}) const;
    int columnCount(const QModelIndex & /* parent */) const override;
    int rowCount(const QModelIndex& = /* parent */{}) const override;
    Q_INVOKABLE void updateSetting(const QString& group,
                                   const QString& name,
                                   const QVariant& value);
    Q_INVOKABLE void setLanguage(const QString& lang);
    Q_INVOKABLE QString getLanguage() const;
    Q_INVOKABLE QString mapSettings(const QString& name) const;
    QVariant data(const QModelIndex& index,int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
signals:
    void valueChanged(const QString& group,
                      const QString& key,
                      const QVariant& value);
};
}
