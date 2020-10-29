#pragma once
#include <QList>
#include <QDate>
#include <QString>
#include <bitset>
constexpr inline std::size_t subject_type_count = 7;
constexpr inline std::size_t subject_bits_total = subject_type_count + 9;
enum subject_t
{
   lecture = 0,
   practice,
   exam,
   laboratory,
   consultation,
   credit,
   course_work,
   undefined = -1
};
inline int detectTimePosition( const QTime& time )
{
   static const QTime start_time{ 7, 45 };
   return subject_type_count + time.secsTo( start_time ) / 6300;
}
inline subject_t detectSubjectType( const QString& subject )
{
   if( subject.contains( "Лк", Qt::CaseInsensitive ) )
      return subject_t::lecture;
   else if( subject.contains( "Пз", Qt::CaseInsensitive ) )
      return subject_t::practice;
   else if( subject.contains( "Екз", Qt::CaseInsensitive ) )
      return subject_t::exam;
   else if( subject.contains( "Лб", Qt::CaseInsensitive ) )
      return subject_t::laboratory;
   else if( subject.contains( "Конс", Qt::CaseInsensitive ) )
      return subject_t::consultation;
   else if( subject.contains( "Зал", Qt::CaseInsensitive ) )
      return subject_t::credit;
   else if( subject.contains( "КП", Qt::CaseInsensitive ) || subject.contains( "КР", Qt::CaseInsensitive ) )
      return subject_t::course_work;
   else
      return subject_t::undefined;
}
class HideSettingsManager
{
   uint16_t& m_bits;

public:
   explicit HideSettingsManager( uint16_t& bits )
      : m_bits{ bits }
   {
   }

   bool isVisible( const QString& lesson ) const
   {
      int lessonIndex = detectSubjectType( lesson );
      if( lessonIndex < 0 )
         return false;
      return isVisible( lessonIndex );
   }
   bool isVisible( int pos ) const
   {
      return m_bits & ( 1 << pos );
   }
   void setVisible( int pos, bool visible )
   {
      if( visible )
      {
         m_bits |= 1UL << pos;
      }
      else
      {
         m_bits &= ~( 1UL << pos );
      }
   }
};
