#pragma once
#include <QObject>
#include <QAbstractTableModel>
#include <QDebug>
#include <algorithm>
#include <set>
#include "../internal/api_structs.h"
namespace timetable {
class TableModel : public QAbstractTableModel {
    Q_OBJECT
    QString m_title;
    QHash<qint64,internal::Lesson> lessons;
    QStringList m_horizontalHeaderData;
    std::set<QString> m_verticalHeaderData;
    std::pair<QTime,int> currentLesson;
    int timetableId = -1;
    int totalRows;
private:
    void generateVerticalHeader();
    void generateHorizontalHeader();
public:
    TableModel();
    Q_INVOKABLE void addLesson(int id,const QVariant& lesson);
    Q_INVOKABLE int id() const noexcept;
    Q_INVOKABLE int lessonDuration() const;
    Q_INVOKABLE int rowProgress(int row);
    Q_INVOKABLE void prepareForNewTimetable(int rowCount);
    Q_INVOKABLE QString secondsToString(uint seconds);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVariantList getModel() const;
    Q_INVOKABLE int currentColumn();
    Q_INVOKABLE QStringList horizontalHeaderData() const;
    Q_INVOKABLE QStringList verticalHeaderData() const;
    int rowCount(const QModelIndex& = {}) const override;
    int columnCount(const QModelIndex& = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::UserRole) const override;
    QHash<int,QByteArray> roleNames() const override;
signals:
    void timetableCompleted();
    void horizontalHeaderFinished();
    void verticalHeaderFinished();
};

}
