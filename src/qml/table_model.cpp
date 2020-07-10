#include "table_model.h"

namespace timetable {
static QString datePattern = "dd.MM.yy";
static QString timePattern = "hh:mm";

void TableModel::generateVerticalHeader() {
    beginResetModel();
    for (const internal::Lesson& lesson : lessons) {
        m_verticalHeaderData.insert(lesson.timeStart.toString(timePattern));
    }
    emit verticalHeaderFinished();
    endResetModel();
}


void TableModel::generateHorizontalHeader() {
    beginResetModel();
    auto [beginDate,endDate] = std::minmax_element(lessons.begin(),lessons.end(),[](const internal::Lesson& lhs,
            const internal::Lesson& rhs){
        return lhs.date < rhs.date;
    });
    m_horizontalHeaderData.reserve(beginDate->date.daysTo(endDate->date));
    for (QDate date = beginDate->date;date != endDate->date;date = date.addDays(1)) {
        m_horizontalHeaderData.push_back(date.toString(datePattern));
    }
    emit horizontalHeaderFinished();
    endResetModel();
}


TableModel::TableModel() {
    connect(this,&TableModel::timetableCompleted,this,&TableModel::generateVerticalHeader);
    connect(this,&TableModel::timetableCompleted,this,&TableModel::generateHorizontalHeader);
}


void TableModel::addLesson(int id, const QVariant &lesson) {
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


int TableModel::id() const noexcept {
    return timetableId;
}


int TableModel::lessonDuration() const {
    if (m_verticalHeaderData.size() < 2)
        return 0;
    else
        return QTime::fromString(*m_verticalHeaderData.begin(),timePattern).secsTo(
                    QTime::fromString(*std::next(m_verticalHeaderData.begin()),timePattern));
}


int TableModel::rowProgress(int row) {
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
    return currentLesson.first.secsTo(QTime::currentTime());
}


void TableModel::prepareForNewTimetable(int rowCount) {
    // remove the last table
    clear();

    lessons.reserve(rowCount);
    totalRows = rowCount;
}


QString TableModel::secondsToString(uint seconds) {
    return QDateTime::fromTime_t(seconds).toUTC().toString("hh:mm:ss");
}


void TableModel::clear() {
    beginResetModel();
    timetableId = -1;
    lessons.clear();
    m_verticalHeaderData.clear();
    m_horizontalHeaderData.clear();
    currentLesson = {};
    endResetModel();
}


QVariantList TableModel::getModel() const {
    QVariantList list;
    list.reserve(lessons.size());
    for (auto it : lessons)
        list.push_back(QVariant::fromValue(it));
    return list;
}


int TableModel::currentColumn() {
    if (m_horizontalHeaderData.empty())
        return 0;

    int index = std::abs(QDate::currentDate()
                         .daysTo(QDate::fromString(m_horizontalHeaderData[0],datePattern)));
    return (index > m_horizontalHeaderData.size()) ? m_horizontalHeaderData.size()/2 : index;
}


QStringList TableModel::horizontalHeaderData() const {
    return m_horizontalHeaderData;
}


QStringList TableModel::verticalHeaderData() const {
    QStringList headerData(m_verticalHeaderData.begin(),m_verticalHeaderData.end());
    return headerData;
}


int TableModel::rowCount(const QModelIndex &) const{
    return m_verticalHeaderData.size()+1;
}


int TableModel::columnCount(const QModelIndex &) const {
    return m_horizontalHeaderData.size();
}


QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int32_t>(m_verticalHeaderData.size()))
        return {""};
    QDateTime modelIndex;
    modelIndex.setDate(QDate::fromString(m_horizontalHeaderData[index.column()],datePattern));
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
    default:
        break;
    }
    return "";
}


QHash<int, QByteArray> TableModel::roleNames() const {
    return {{Qt::UserRole,"subject"},
        {Qt::UserRole+1,"type"},
        {Qt::UserRole+2,"auditory"}};
}
}
