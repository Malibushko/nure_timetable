#pragma once
#include <QtWidgets/QtWidgets>
#include <QtQuick>
#include "treenode.h"
#include "internal/api_structs.h"
#include "internal/hide_serialize.h"
class TreeModel : public QAbstractItemModel
{
   Q_OBJECT
   QHash<qint64, timetable::internal::Lesson>*           m_modelData{ nullptr };
   QMap<QString, timetable::internal::TableHideSettings> m_hideSettings;
   QMap<QString, timetable::internal::LessonHide>*       m_tableHidePtr;

public:
   Q_PROPERTY( QQmlListProperty<TreeNode> nodes READ nodes )
   Q_PROPERTY( QVariantList roles READ roles WRITE setRoles NOTIFY rolesChanged )
   Q_CLASSINFO( "DefaultProperty", "nodes" )

   TreeModel( QObject* parent = Q_NULLPTR );
   ~TreeModel();

   Q_INVOKABLE void setModel( QVariant lessonsPointer, QVariant tablePointer )
   {
      m_modelData = reinterpret_cast<decltype( m_modelData )>( qvariant_cast<uintptr_t>( lessonsPointer ) );
      m_tableHidePtr = reinterpret_cast<decltype( m_tableHidePtr )>( qvariant_cast<uintptr_t>( tablePointer ) );
      m_hideSettings.clear();
      delete m_rootNode;
      m_rootNode = new TreeNode( this );
   }
   Q_INVOKABLE void changeVisible( const QModelIndex& index, bool visible )
   {
      auto iter = std::next( m_hideSettings.begin(), index.parent().parent().row() );

      int parentRow = index.parent().parent().row();
      if( auto item = m_tableHidePtr->find( std::next( m_hideSettings.begin(), parentRow ).key() );
          item != m_tableHidePtr->end() )
      {
         switch( index.parent().row() )
         {
            case 0:
            {
               HideSettingsManager mng( item->data );
               mng.setVisible(
                  detectTimePosition( std::next( iter->lessonTime.begin(), index.row() )->first ), visible );
               break;
            }
            case 1:
            {
               HideSettingsManager mng( item->data );
               mng.setVisible( detectSubjectType( *std::next( iter->lessonTypes.begin(), index.row() ) ), visible );
               break;
            }
         }
      }
   }
   Q_INVOKABLE void generate()
   {
      if( m_modelData )
      {
         beginResetModel();
         for( const auto& lesson : *m_modelData )
         {
            m_hideSettings[lesson.subject].lessonTime.insert( { lesson.timeStart, lesson.timeEnd } );
            m_hideSettings[lesson.subject].lessonTypes.insert( lesson.type );
         }
         for( const auto& lesson : m_hideSettings )
         {
            TreeNode* node = new TreeNode( this );
            TreeNode *types = new TreeNode( this ), *time = new TreeNode( this );
            node->setParentNode( m_rootNode );
            types->setParentNode( node );
            time->setParentNode( node );
            for( std::size_t i = 0; i < lesson.lessonTime.size(); ++i )
            {
               TreeNode* timeSubNode = new TreeNode( this );
               timeSubNode->setParentNode( time );
               time->insertNode( timeSubNode );
            }
            for( std::size_t i = 0; i < lesson.lessonTypes.size(); ++i )
            {
               TreeNode* typeSubNode = new TreeNode( this );
               typeSubNode->setParentNode( types );
               types->insertNode( typeSubNode );
            }
            node->insertNode( time );
            node->insertNode( types );
            insertNode( node );
         }
         endResetModel();
      }
      else
         qDebug() << "[WARNING] Calling TreeModel::generate with an empty model.";
   }
   QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
   QVariant               data( const QModelIndex& index, int role ) const Q_DECL_OVERRIDE;
   Qt::ItemFlags          flags( const QModelIndex& index ) const Q_DECL_OVERRIDE;
   QModelIndex            index( int row, int column, const QModelIndex& parent = QModelIndex() ) const Q_DECL_OVERRIDE;
   QModelIndex            parent( const QModelIndex& index ) const Q_DECL_OVERRIDE;
   int                    rowCount( const QModelIndex& parent = QModelIndex() ) const Q_DECL_OVERRIDE;
   int                    columnCount( const QModelIndex& parent = QModelIndex() ) const Q_DECL_OVERRIDE;
   QQmlListProperty<TreeNode> nodes();

   QVariantList roles() const;
   void         setRoles( const QVariantList& roles );

   Q_INVOKABLE TreeNode*   getNodeByIndex( const QModelIndex& index );
   Q_INVOKABLE QModelIndex getIndexByNode( TreeNode* node );
   Q_INVOKABLE bool insertNode( TreeNode* childNode, const QModelIndex& parent = QModelIndex(), int pos = ( -1 ) );

protected:
   TreeNode* getNode( const QModelIndex& index ) const;

private:
   TreeNode*              m_rootNode;
   QHash<int, QByteArray> m_roles =
      QHash<int, QByteArray>{ { Qt::UserRole, "subject" }, { Qt::UserRole + 1, "checked" } };
signals:
   void rolesChanged();
};
