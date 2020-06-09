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
    QList<internal::Lesson> lessons;
    QStringList m_horizontalHeaderData;
    std::set<QString> m_verticalHeaderData;
    int timetableId = -1;
    int totalRows;
private:
    void generateVerticalHeader() {
        for (const internal::Lesson& lesson : lessons) {
            m_verticalHeaderData.insert(lesson.timeStart.toString("hh:mm"));
        }
        emit verticalHeaderFinished();
    }
    void generateHorizontalHeader() {
        auto [beginDate,endDate] = std::minmax_element(lessons.begin(),lessons.end(),[](const internal::Lesson& lhs,
                const internal::Lesson& rhs){
               return lhs.date < rhs.date;
        });
        m_horizontalHeaderData.reserve(beginDate->date.daysTo(endDate->date));
        for (QDate date = beginDate->date;date != endDate->date;date = date.addDays(1)) {
            m_horizontalHeaderData.push_back(date.toString("dd.MM.yy"));
        }
        emit horizontalHeaderFinished();
    }
public:
    TableModel() {
        connect(this,&TableModel::timetableCompleted,this,&TableModel::generateVerticalHeader);
        connect(this,&TableModel::timetableCompleted,this,&TableModel::generateHorizontalHeader);
    }
    Q_INVOKABLE void addLesson(int id,const QVariant& lesson) {
        if (timetableId == -1)
            timetableId = id;
        if (id == timetableId) {
            lessons.push_back(qvariant_cast<internal::Lesson>(lesson));
            if (lessons.size() == totalRows)
                emit timetableCompleted();
        }
    }
    Q_INVOKABLE void prepareForNewTimetable(int rowCount) {
        clear();
        lessons.reserve(rowCount);
        totalRows = rowCount;
    }
    Q_INVOKABLE void clear() {
        timetableId = -1;

        lessons.clear();
        m_verticalHeaderData.clear();
        m_horizontalHeaderData.clear();
    }
    Q_INVOKABLE QStringList horizontalHeaderData() const {
        return m_horizontalHeaderData;
    }
    Q_INVOKABLE QStringList verticalHeaderData() const {
        QStringList headerData(m_verticalHeaderData.begin(),m_verticalHeaderData.end());
        return headerData;
    }
    int rowCount(const QModelIndex& = {}) const override{
        return 6;
    }
    int columnCount(const QModelIndex& = {}) const override {
        return 200;
    }
    QVariant data([[maybe_unused]]const QModelIndex& index, int role = Qt::UserRole) const override {
        return QVariant{};
    }
    QHash<int,QByteArray> roleNames() const override {
        return {};
    }
signals:
    void timetableCompleted();
    void horizontalHeaderFinished();
    void verticalHeaderFinished();
};
}
