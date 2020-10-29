#include "treemodel.h"
#include "internal/hide_serialize.h"
TreeModel::TreeModel( QObject* parent )
   : QAbstractItemModel( parent )
{
   m_rootNode = new TreeNode( nullptr );
}
TreeModel::~TreeModel()
{
   delete m_rootNode;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
   return m_roles;
}

QVariant TreeModel::data( const QModelIndex& index, int role ) const
{
   if( !index.isValid() )
      return QVariant();

   TreeNode* item = static_cast<TreeNode*>( index.internalPointer() );
   int       itemDepth = item->depth();
   QString   result;
   switch( itemDepth )
   {
      case 0:
      {
         result = std::next( m_hideSettings.begin(), index.row() ).key();
         break;
      }
      case 1:
      {
         switch( index.row() )
         {
            case 0:
               return tr( "Time" );
            case 1:
               return tr( "Type" );
            default:
               return tr( "Undefined" );
         }
         break;
      }
      case 2:
      {
         auto iter = std::next( m_hideSettings.begin(), index.parent().parent().row() );
         if( role == Qt::UserRole + 1 )
         {
            int parentRow = index.parent().parent().row();
            if( auto item = m_tableHidePtr->find( std::next( m_hideSettings.begin(), parentRow ).key() );
                item != m_tableHidePtr->end() )
            {
               switch( index.parent().row() )
               {
                  case 0:
                  {
                     return HideSettingsManager( item->data ).isVisible( index.row() + subject_type_count );
                  }
                  case 1:
                  {
                     HideSettingsManager mng( item->data );
                     return mng.isVisible( *std::next( iter->lessonTypes.begin(), index.row() ) );
                  }
               }
            }
         }
         else
         {
            switch( index.parent().row() )
            {
               case 0:
               {
                  auto timeIter = std::next( iter->lessonTime.begin(), index.row() );
                  result = timeIter->first.toString( "hh:mm" ) + " - " + timeIter->second.toString( "hh:mm" );
                  break;
               }
               case 1:
               {
                  result = *std::next( iter->lessonTypes.begin(), index.row() );
                  break;
               }
            }
         }
         break;
      }
   }

   return result;
}

Qt::ItemFlags TreeModel::flags( const QModelIndex& index ) const
{
   if( !index.isValid() )
      return 0;

   return QAbstractItemModel::flags( index );
}

QModelIndex TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
   if( !hasIndex( row, column, parent ) )
      return QModelIndex();

   TreeNode* parentItem = getNode( parent );
   TreeNode* childItem = parentItem->childNode( row );
   if( childItem )
      return createIndex( row, column, childItem );
   else
      return QModelIndex();
}

QModelIndex TreeModel::parent( const QModelIndex& index ) const
{
   if( !index.isValid() )
      return QModelIndex();

   TreeNode* childItem = static_cast<TreeNode*>( index.internalPointer() );
   TreeNode* parentItem = static_cast<TreeNode*>( childItem->parentNode() );

   if( parentItem == m_rootNode )
      return QModelIndex();

   return createIndex( parentItem->pos(), 0, parentItem );
}

int TreeModel::rowCount( const QModelIndex& parent ) const
{
   if( parent.column() > 0 )
      return 0;
   TreeNode* parentItem = getNode( parent );
   return parentItem->count();
}

int TreeModel::columnCount( const QModelIndex& parent ) const
{
   Q_UNUSED( parent );
   return 1;
}

QQmlListProperty<TreeNode> TreeModel::nodes()
{
   return m_rootNode->nodes();
}

QVariantList TreeModel::roles() const
{
   QVariantList                   list;
   QHashIterator<int, QByteArray> i( m_roles );
   while( i.hasNext() )
   {
      i.next();
      list.append( i.value() );
   }

   return list;
}

void TreeModel::setRoles( const QVariantList& roles )
{
   static int nextRole = Qt::UserRole + 1;
   foreach( auto role, roles )
   {
      m_roles.insert( nextRole, role.toByteArray() );
      nextRole++;
   }
}

QModelIndex TreeModel::getIndexByNode( TreeNode* node )
{
   QVector<int> positions;
   QModelIndex  result;
   if( node )
   {
      do
      {
         int pos = node->pos();
         positions.append( pos );
         node = node->parentNode();
      } while( node != nullptr );

      for( int i = positions.size() - 2; i >= 0; i-- )
      {
         result = index( positions[i], 0, result );
      }
   }
   return result;
}

bool TreeModel::insertNode( TreeNode* childNode, const QModelIndex& parent, int pos )
{
   TreeNode* parentElement = getNode( parent );
   if( pos >= parentElement->count() )
      return false;
   if( pos < 0 )
      pos = parentElement->count();

   childNode->setParentNode( parentElement );
   beginInsertRows( parent, pos, pos );
   bool retValue = parentElement->insertNode( childNode, pos );
   endInsertRows();
   return retValue;
}

TreeNode* TreeModel::getNode( const QModelIndex& index ) const
{
   if( index.isValid() )
      return static_cast<TreeNode*>( index.internalPointer() );
   return m_rootNode;
}
