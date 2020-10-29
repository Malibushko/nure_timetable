#include "treenode.h"
#include "treemodel.h"
void append_element( QQmlListProperty<TreeNode>* property, TreeNode* value )
{
   TreeNode* parent = ( qobject_cast<TreeNode*>( property->object ) );
   value->setParentNode( parent );
   parent->insertNode( value, -1 );
}

int count_element( QQmlListProperty<TreeNode>* property )
{
   TreeNode* parent = ( qobject_cast<TreeNode*>( property->object ) );
   return parent->count();
}

TreeNode* at_element( QQmlListProperty<TreeNode>* property, int index )
{
   TreeNode* parent = ( qobject_cast<TreeNode*>( property->object ) );
   if( index < 0 || index >= parent->count() )
      return nullptr;
   return parent->childNode( index );
}

void clear_element( QQmlListProperty<TreeNode>* property )
{
   TreeNode* parent = ( qobject_cast<TreeNode*>( property->object ) );
   parent->clear();
}

TreeNode::TreeNode( QObject* parent )
   : QObject( parent )
   , m_parentNode( nullptr )
{
}

void TreeNode::setParentNode( TreeNode* parent )
{
   m_parentNode = parent;
}

TreeNode* TreeNode::parentNode() const
{
   return m_parentNode;
}

QQmlListProperty<TreeNode> TreeNode::nodes()
{
   QQmlListProperty<TreeNode> list( this, 0, &append_element, &count_element, &at_element, &clear_element );
   return list;
}

TreeNode* TreeNode::childNode( int index ) const
{
   if( index < 0 || index >= m_nodes.length() )
      return nullptr;
   return m_nodes.at( index );
}

void TreeNode::clear()
{
   qDeleteAll( m_nodes );
   m_nodes.clear();
}

bool TreeNode::insertNode( TreeNode* node, int pos )
{
   if( pos > m_nodes.count() )
      return false;
   if( pos < 0 )
      pos = m_nodes.count();
   m_nodes.insert( pos, node );
   return true;
}

int TreeNode::pos() const
{
   TreeNode* parent = parentNode();
   if( parent )
      return parent->m_nodes.indexOf( const_cast<TreeNode*>( this ) );
   return 0;
}

int TreeNode::count() const
{
   return m_nodes.size();
}

TreeNode* TreeModel::getNodeByIndex( const QModelIndex& index )
{
   if( !index.isValid() )
      return nullptr;
   return static_cast<TreeNode*>( index.internalPointer() );
}
