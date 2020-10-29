#pragma once
#include <QtWidgets/QtWidgets>
#include <QtQuick>

class TreeNode : public QObject
{
   Q_OBJECT
public:
   Q_PROPERTY( QQmlListProperty<TreeNode> nodes READ nodes )
   Q_CLASSINFO( "DefaultProperty", "nodes" )
   TreeNode( QObject* parent = Q_NULLPTR );

   void        setParentNode( TreeNode* parent );
   Q_INVOKABLE TreeNode*      parentNode() const;
   bool                       insertNode( TreeNode* node, int pos = ( -1 ) );
   QQmlListProperty<TreeNode> nodes();
   int                        depth()
   {
      TreeNode* ptr = parentNode();
      int       i = 0;
      while( ptr && ( ptr = ptr->parentNode() ) )
         ++i;
      return i;
   }
   TreeNode* childNode( int index ) const;
   void      clear();

   Q_INVOKABLE int pos() const;
   Q_INVOKABLE int count() const;

private:
   QList<TreeNode*> m_nodes;
   TreeNode*        m_parentNode;
};
