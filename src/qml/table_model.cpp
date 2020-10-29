#include "table_model.h"
#include "internal/hide_serialize.h"
namespace timetable {

namespace {
static QString datePattern = "dd.MM.yy";
static QString timeFormat = "hh:mm";
static QString datePatternFull = "dd.MM.yyyy";
} // namespace

void TableModel::generateVerticalHeader()
{
   beginResetModel();
   m_verticalHeaderData.reserve( m_lessons.size() );
   for( const internal::Lesson& lesson : m_lessons )
   {
      m_verticalHeaderData.push_back( lesson.timeStart );
   }
   std::sort( m_verticalHeaderData.begin(), m_verticalHeaderData.end() );
   m_verticalHeaderData.erase(
      std::unique( m_verticalHeaderData.begin(), m_verticalHeaderData.end() ), m_verticalHeaderData.end() );
   emit verticalHeaderFinished();
   endResetModel();
}

void TableModel::generateHorizontalHeader()
{
   beginResetModel();
   auto [beginDate, endDate] = std::minmax_element( m_lessons.begin(), m_lessons.end(),
      []( const internal::Lesson& lhs, const internal::Lesson& rhs ) { return lhs.date < rhs.date; } );
   m_horizontalHeaderData.reserve( beginDate->date.daysTo( endDate->date ) );
   for( QDate date = beginDate->date; date != endDate->date; date = date.addDays( 1 ) )
   {
      m_horizontalHeaderData.push_back( QDateTime( date, {} ) );
   }
   emit horizontalHeaderFinished();
   endResetModel();
}

TableModel::TableModel()
{
   connect( this, &TableModel::timetableCompleted, this, &TableModel::generateVerticalHeader );
   connect( this, &TableModel::timetableCompleted, this, &TableModel::generateHorizontalHeader );
}

void TableModel::addLesson( int id, const QVariant& lesson )
{
   if( m_timetableId == -1 )
      m_timetableId = id;
   if( id == m_timetableId )
   {
      auto      l = qvariant_cast<internal::Lesson>( lesson );
      QDateTime index;
      index.setDate( l.date );
      index.setTime( l.timeStart );
      m_lessons.insert( index.toMSecsSinceEpoch(), qvariant_cast<internal::Lesson>( lesson ) );
      m_lessonNames.insert( l.subject );
      if( m_lessons.size() == m_totalRows )
      {
         if( !m_hideSettings.size() )
         {
            for( const auto& it : m_lessonNames )
            {
               m_hideSettings.insert( it, { it, static_cast<unsigned short>( -1 ) } );
            }
         }
         emit timetableCompleted();
      }
   }
}

int TableModel::id() const noexcept
{
   return m_timetableId;
}

int TableModel::lessonDuration() const
{
   if( m_verticalHeaderData.size() < 2 )
      return 0;
   else
   {
      return m_verticalHeaderData.begin()->secsTo( *std::next( m_verticalHeaderData.begin() ) );
   }
}

int TableModel::rowProgress( int row )
{
   // check if there is a value cached and it is not expired
   if( m_currentLesson.first.isNull() || m_currentLesson.first.secsTo( QTime::currentTime() ) >= lessonDuration() )
   {
      // if expired calculate new one
      decltype( auto ) it = m_verticalHeaderData.begin();
      QTime            currentTime = QTime::currentTime();
      for( ; it != m_verticalHeaderData.end(); ++it )
      {
         // find the closest lesson for now
         if( it->secsTo( currentTime ) < lessonDuration() )
         {
            m_currentLesson.first = *it;
            m_currentLesson.second = std::distance( m_verticalHeaderData.begin(), it );
            break;
         }
      }
      // if not value found - all lessons ended for today
      if( it != m_verticalHeaderData.end() )
      {
         if( std::distance( m_verticalHeaderData.begin(), it ) != row )
            return 0;
      }
      else
         return 0;
   }
   // else return 0 if current lesson is not the cached one
   if( m_currentLesson.second != row )
      return 0;
   // finally return progress
   return m_currentLesson.first.secsTo( QTime::currentTime() );
}

void TableModel::setNotes( QVariantList notes )
{
   if( notes.size() )
   {
      m_notes = qvariant_cast<timetable::internal::Notes>( notes[0] );
   }
   else
      qDebug() << "No notes for id " << m_timetableId;

   m_notes.id = m_timetableId;
   emit dataChanged( createIndex( 0, 0 ), createIndex( m_totalRows, m_horizontalHeaderData.size() ), { NOTE } );
}

void TableModel::setNote( int sequentialIndex, const QString& value )
{
   if( auto iter = std::find_if( m_notes.notes.begin(), m_notes.notes.end(),
          [&]( const auto& item ) { return item.index == sequentialIndex; } );
       iter != m_notes.notes.end() )
   {
      iter->note = value;
   }
   else
   {
      m_notes.notes.push_back( { sequentialIndex, value, QDate::currentDate().toString() } );
   }
   emit noteChanged( QVariant::fromValue( m_notes ) );
}

void TableModel::prepareForNewTimetable( int rowCount )
{
   // remove the last table
   clear();

   m_lessons.reserve( rowCount );
   m_totalRows = rowCount;
}

QString TableModel::secondsToString( uint seconds )
{
   return QDateTime::fromTime_t( seconds ).toUTC().toString( "hh:mm:ss" );
}

QVariantList TableModel::getHideLessonList()
{
   QVariantList list;
   list.reserve( m_hideSettings.values().size() );
   for( const auto& it : m_hideSettings )
   {
      list.push_back( QVariant::fromValue( it ) );
   }
   return list;
}

void TableModel::redrawHideLessons()
{
   emit dataChanged(
      createIndex( 0, 0 ), createIndex( rowCount(), columnCount() ), { static_cast<int>( TableRoles::VISIBLE ) } );
}

void TableModel::setHideLessons( const QVariantList& lessons )
{
   for( auto& it : lessons )
   {
      auto casted = qvariant_cast<internal::LessonHide>( it );
      m_hideSettings.insert( casted.subject, casted );
   }
}

QVariant TableModel::hideTablePtr() const
{
   return QVariant::fromValue( reinterpret_cast<uintptr_t>( &m_hideSettings ) );
}

QVariant TableModel::lessonTablePtr() const
{
   return QVariant::fromValue( reinterpret_cast<uintptr_t>( &m_lessons ) );
}

QStringList TableModel::lessonList() const
{
   return QStringList{ m_lessonNames.begin(), m_lessonNames.end() };
}

void TableModel::clear()
{
   beginResetModel();
   m_timetableId = -1;
   m_lessons.clear();
   m_verticalHeaderData.clear();
   m_horizontalHeaderData.clear();
   m_currentLesson = {};
   m_hideSettings.clear();
   endResetModel();
}

QVariantList TableModel::getModel() const
{
   QVariantList list;
   list.reserve( m_lessons.size() );
   for( auto it : m_lessons )
      list.push_back( QVariant::fromValue( it ) );
   return list;
}

int TableModel::currentColumn()
{
   if( m_horizontalHeaderData.empty() )
      return 0;

   int index = std::abs( QDate::currentDate().daysTo( m_horizontalHeaderData[0].date() ) );
   return ( index > m_horizontalHeaderData.size() ) ? m_horizontalHeaderData.size() / 2 : index;
}

QList<QDateTime> TableModel::horizontalHeaderData() const
{
   return m_horizontalHeaderData;
}

QList<QTime> TableModel::verticalHeaderData() const
{
   return m_verticalHeaderData;
}

int TableModel::rowCount( const QModelIndex& ) const
{
   return m_verticalHeaderData.size() + 1;
}

int TableModel::columnCount( const QModelIndex& ) const
{
   return m_horizontalHeaderData.size();
}

QVariant TableModel::data( const QModelIndex& index, int role ) const
{
   if( !index.isValid() || index.row() < 0 || index.row() >= static_cast<int32_t>( m_verticalHeaderData.size() ) )
      return "";
   /*
    * Calculating index
    * It contains of Time (rows headers) and Date (columns header)
    */

   auto iter = m_verticalHeaderData.begin();
   std::advance( iter, index.row() );
   QDateTime modelIndex( m_horizontalHeaderData[index.column()].date(), *iter );

   if( m_lessons.find( modelIndex.toMSecsSinceEpoch() ) == m_lessons.end() )
   {
      return "";
   }

   const internal::Lesson& lesson = m_lessons[modelIndex.toMSecsSinceEpoch()];
   switch( role )
   {
      case TableRoles::DATE:
         return lesson.date.toString( datePatternFull );
      case TableRoles::SUBJECT:
         return lesson.subject;
      case TableRoles::TYPE:
         return lesson.type;
      case TableRoles::GROUPS:
         return lesson.groups;
      case TableRoles::AUDITORY:
         return lesson.auditory;
      case TableRoles::TIME_START:
         return lesson.timeStart.toString( timeFormat );
      case TableRoles::TIME_END:
         return lesson.timeEnd.toString( timeFormat );
      case TableRoles::VISIBLE:
      {
         uint16_t            data = m_hideSettings[lesson.subject].data;
         HideSettingsManager mng( data );
         return mng.isVisible( lesson.type ) && mng.isVisible( detectTimePosition( lesson.timeStart ) );
      }
      case TableRoles::NOTE:
      {
         auto note = std::find_if( m_notes.notes.begin(), m_notes.notes.end(),
            [id = index.column() * rowCount() + index.row()]( const auto& value ) { return value.index == id; } );
         if( note != m_notes.notes.end() )
         {
            return note->note;
         }
         else
            return "";
      }
      default:
         return { "" };
   }
}

QHash<int, QByteArray> TableModel::roleNames() const
{
   return { { TableRoles::DATE, "date" }, { TableRoles::SUBJECT, "subject" }, { TableRoles::TYPE, "type" },
      { TableRoles::GROUPS, "groups" }, { TableRoles::AUDITORY, "auditory" }, { TableRoles::TIME_START, "timeStart" },
      { TableRoles::TIME_END, "timeEnd" }, { TableRoles::VISIBLE, "visible" }, { TableRoles::NOTE, "note" } };
}
} // namespace timetable
