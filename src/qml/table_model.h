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
    static inline const QHash<QString,QString> colors {
       {"Зал","#C2A0B8",},
        {"Пз","#DAE9D9",},
        {"Лк","#FEFEEA"},
        {"Лб","#CDCCFF"}
    };
    QString m_title;
    QHash<uint64_t,internal::Lesson> lessons;
    QStringList m_horizontalHeaderData;
    std::set<QString> m_verticalHeaderData;
    std::pair<QTime,int> currentLesson;
    int timetableId = -1;
    int totalRows;
private:
    void generateVerticalHeader() {
        beginResetModel();
        for (const internal::Lesson& lesson : lessons) {
            m_verticalHeaderData.insert(lesson.timeStart.toString("hh:mm"));
        }
        emit verticalHeaderFinished();
        endResetModel();
    }
    void generateHorizontalHeader() {
        beginResetModel();
        auto [beginDate,endDate] = std::minmax_element(lessons.begin(),lessons.end(),[](const internal::Lesson& lhs,
                const internal::Lesson& rhs){
            return lhs.date < rhs.date;
        });
        m_horizontalHeaderData.reserve(beginDate->date.daysTo(endDate->date));
        for (QDate date = beginDate->date;date != endDate->date;date = date.addDays(1)) {
            m_horizontalHeaderData.push_back(date.toString("dd.MM.yyyy"));
        }
        emit horizontalHeaderFinished();
        endResetModel();
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
            auto l = qvariant_cast<internal::Lesson>(lesson);
            QDateTime index;
            index.setDate(l.date);
            index.setTime(l.timeStart);
            lessons.insert(index.toMSecsSinceEpoch(),
                           qvariant_cast<internal::Lesson>(lesson));
            if (lessons.size() == totalRows)
                emit timetableCompleted();
        }
    }
    Q_INVOKABLE int lessonDuration() const {
        if (m_verticalHeaderData.size() < 2)
            return 0;
        else
            return QTime::fromString(*m_verticalHeaderData.begin(),"hh:mm").secsTo(
                        QTime::fromString(*std::next(m_verticalHeaderData.begin()),"hh:mm"));
    }
    Q_INVOKABLE int rowProgress(int row) {
        if (currentLesson.first.isNull() || currentLesson.first.secsTo(QTime::currentTime()) >= lessonDuration()) {
            decltype (auto) it = m_verticalHeaderData.begin();
            QTime currentTime = QTime::currentTime();
            for ( ;it != m_verticalHeaderData.end();++it) {
                QTime timeStart = QTime::fromString(*it,"hh:mm");
                if (timeStart.secsTo(currentTime) < lessonDuration()) {
                    currentLesson.first = timeStart;
                    currentLesson.second = std::distance(m_verticalHeaderData.begin(),it);
                    break;
                }
            }
            if (it != m_verticalHeaderData.end()) {
                if (std::distance(m_verticalHeaderData.begin(),it) != row)
                    return 0;
            } else
                return 0;
        }
        if (currentLesson.second != row)
            return 0;
        return lessonDuration() - currentLesson.first.secsTo(QTime::currentTime());
    }
    Q_INVOKABLE void prepareForNewTimetable(int rowCount) {
        // remove the last table
        clear();

        lessons.reserve(rowCount);
        totalRows = rowCount;
    }
    Q_INVOKABLE void clear() {
        beginResetModel();
        timetableId = -1;
        lessons.clear();
        m_verticalHeaderData.clear();
        m_horizontalHeaderData.clear();
        currentLesson = {};
        endResetModel();
    }
    Q_INVOKABLE QVariantList getModel() const {
        QVariantList list;
        list.reserve(lessons.size());
        for (auto it : lessons)
            list.push_back(QVariant::fromValue(it));
        return list;
    }
    Q_INVOKABLE int currentColumn() {
        if (m_horizontalHeaderData.empty())
            return 0;

        int index = std::abs(QDate::currentDate()
                        .daysTo(QDate::fromString(m_horizontalHeaderData[0],"dd.MM.yyyy")));
        return (index > m_horizontalHeaderData.size()) ? m_horizontalHeaderData.size()/2 : index;
    }
    Q_INVOKABLE QStringList horizontalHeaderData() const {
        return m_horizontalHeaderData;
    }
    Q_INVOKABLE QStringList verticalHeaderData() const {
        QStringList headerData(m_verticalHeaderData.begin(),m_verticalHeaderData.end());
        return headerData;
    }
    int rowCount(const QModelIndex& = {}) const override{
        return m_verticalHeaderData.size()+1;
    }
    int columnCount(const QModelIndex& = {}) const override {
        return m_horizontalHeaderData.size();
    }
    QVariant data(const QModelIndex& index, int role = Qt::UserRole) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_verticalHeaderData.size())
            return {""};
        QDateTime modelIndex;
        modelIndex.setDate(QDate::fromString(m_horizontalHeaderData[index.column()],"dd.MM.yyyy"));
        auto iter = m_verticalHeaderData.begin();
        std::advance(iter,index.row());
        modelIndex.setTime(QTime::fromString(*iter,"hh:mm"));

        if (lessons.find(modelIndex.toMSecsSinceEpoch()) == lessons.end()) {
            return "";
        }

        const internal::Lesson& lesson = lessons[modelIndex.toMSecsSinceEpoch()];
        switch (role) {
            case Qt::UserRole:
                return lesson.subject;
            case Qt::UserRole+1:
                return lesson.type;
            case Qt::UserRole+2:
                return lesson.auditory;
            case Qt::UserRole+3:
                return colors[lesson.type];
        }
        return "";
    }
    QHash<int,QByteArray> roleNames() const override {
        return {{Qt::UserRole,"subject"},
            {Qt::UserRole+1,"type"},
            {Qt::UserRole+2,"auditory"},
            {Qt::UserRole+3,"color"}};
    }
signals:
    void timetableCompleted();
    void horizontalHeaderFinished();
    void verticalHeaderFinished();
};
}
