#include "table_model.h"

namespace timetable {

static QString datePattern = "dd.MM.yy";
static QString timeFormat = "hh:mm";
static QString datePatternFull = "dd.MM.yyyy";

void TableModel::generateVerticalHeader() {
    beginResetModel();
    m_verticalHeaderData.reserve(m_lessons.size());
    for (const internal::Lesson& lesson : m_lessons) {
        m_verticalHeaderData.push_back(lesson.timeStart);
    }
    std::sort(m_verticalHeaderData.begin(),m_verticalHeaderData.end());
    m_verticalHeaderData.erase(std::unique(m_verticalHeaderData.begin(),m_verticalHeaderData.end()),
                               m_verticalHeaderData.end());
    emit verticalHeaderFinished();
    endResetModel();
}


void TableModel::generateHorizontalHeader() {
    beginResetModel();
    auto [beginDate,endDate] = std::minmax_element(m_lessons.begin(),m_lessons.end(),[](const internal::Lesson& lhs,
            const internal::Lesson& rhs){
        return lhs.date < rhs.date;
    });
    m_horizontalHeaderData.reserve(beginDate->date.daysTo(endDate->date));
    for (QDate date = beginDate->date;date != endDate->date;date = date.addDays(1)) {
        m_horizontalHeaderData.push_back(QDateTime(date,{}));
    }
    emit horizontalHeaderFinished();
    endResetModel();
}


TableModel::TableModel() {
    connect(this,&TableModel::timetableCompleted,this,&TableModel::generateVerticalHeader);
    connect(this,&TableModel::timetableCompleted,this,&TableModel::generateHorizontalHeader);
}


void TableModel::addLesson(int id, const QVariant &lesson) {
    if (m_timetableId == -1)
        m_timetableId = id;
    if (id == m_timetableId) {
        auto l = qvariant_cast<internal::Lesson>(lesson);
        QDateTime index;
        index.setDate(l.date);
        index.setTime(l.timeStart);
        m_lessons.insert(index.toMSecsSinceEpoch(),
                       qvariant_cast<internal::Lesson>(lesson));
        if (m_lessons.size() == m_totalRows)
            emit timetableCompleted();
    }
}


int TableModel::id() const noexcept {
    return m_timetableId;
}


int TableModel::lessonDuration() const {
    if (m_verticalHeaderData.size() < 2)
        return 0;
    else {
        return m_verticalHeaderData.begin()->secsTo(*std::next(m_verticalHeaderData.begin()));
    }
}


int TableModel::rowProgress(int row) {
    // check if there is a value cached and it is not expired
    if (m_currentLesson.first.isNull() || m_currentLesson.first.secsTo(QTime::currentTime()) >= lessonDuration()) {
        // if expired calculate new one
        decltype (auto) it = m_verticalHeaderData.begin();
        QTime currentTime = QTime::currentTime();
        for ( ;it != m_verticalHeaderData.end();++it) {
            // find the closest lesson for now
            if (it->secsTo(currentTime) < lessonDuration()) {
                m_currentLesson.first = *it;
                m_currentLesson.second = std::distance(m_verticalHeaderData.begin(),it);
                break;
            }
        }
        // if not value found - all lessons ended for today
        if (it != m_verticalHeaderData.end()) {
            if (std::distance(m_verticalHeaderData.begin(),it) != row)
                return 0;
        } else
            return 0;
    }
    // else return 0 if current lesson is not the cached one
    if (m_currentLesson.second != row)
        return 0;
    // finally return progress
    return m_currentLesson.first.secsTo(QTime::currentTime());
}


void TableModel::prepareForNewTimetable(int rowCount) {
    // remove the last table
    clear();

    m_lessons.reserve(rowCount);
    m_totalRows = rowCount;
}


QString TableModel::secondsToString(uint seconds) {
    return QDateTime::fromTime_t(seconds).toUTC().toString("hh:mm:ss");
}


void TableModel::clear() {
    beginResetModel();
    m_timetableId = -1;
    m_lessons.clear();
    m_verticalHeaderData.clear();
    m_horizontalHeaderData.clear();
    m_currentLesson = {};
    endResetModel();
}


QVariantList TableModel::getModel() const {
    QVariantList list;
    list.reserve(m_lessons.size());
    for (auto it : m_lessons)
        list.push_back(QVariant::fromValue(it));
    return list;
}


int TableModel::currentColumn() {
    if (m_horizontalHeaderData.empty())
        return 0;

    int index = std::abs(QDate::currentDate()
                         .daysTo(m_horizontalHeaderData[0].date()));
    return (index > m_horizontalHeaderData.size()) ? m_horizontalHeaderData.size()/2 : index;
}


QList<QDateTime> TableModel::horizontalHeaderData() const {
    return m_horizontalHeaderData;
}


QList<QTime> TableModel::verticalHeaderData() const {
    return m_verticalHeaderData;
}


int TableModel::rowCount(const QModelIndex &) const{
    return m_verticalHeaderData.size()+1;
}


int TableModel::columnCount(const QModelIndex &) const {
    return m_horizontalHeaderData.size();
}


QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int32_t>(m_verticalHeaderData.size()))
        return "";
    /*
     * Calculating index
     * It contains of Time (rows headers) and Date (columns header)
     */

    auto iter = m_verticalHeaderData.begin();
    std::advance(iter,index.row());
    QDateTime modelIndex(m_horizontalHeaderData[index.column()].date(),*iter);

    if (m_lessons.find(modelIndex.toMSecsSinceEpoch()) == m_lessons.end()) {
        return "";
    }

    const internal::Lesson& lesson = m_lessons[modelIndex.toMSecsSinceEpoch()];
    switch (role) {
    case Qt::UserRole:
        return lesson.date.toString(datePatternFull);
    case Qt::UserRole+1:
        return lesson.subject;
    case Qt::UserRole+2:
        return lesson.type;
    case Qt::UserRole+3:
        return lesson.groups;
    case Qt::UserRole+4:
        return lesson.auditory;
    case Qt::UserRole+5:
        return lesson.timeStart.toString(timeFormat);
    case Qt::UserRole+6:
        return lesson.timeEnd.toString(timeFormat);
    default:
        return {""};
    }
}


QHash<int, QByteArray> TableModel::roleNames() const {
    return {
        {Qt::UserRole,"date"},
        {Qt::UserRole+1,"subject"},
        {Qt::UserRole+2,"type"},
        {Qt::UserRole+3,"groups"},
        {Qt::UserRole+4,"auditory"},
        {Qt::UserRole+5,"timeStart"},
        {Qt::UserRole+6,"timeEnd"}
    };
}
}
