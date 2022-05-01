#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

template < class T >
struct Node;

template < class T, class Compare = std::less<T>, class ValueCompare = Compare,
         class Alloc = std::allocator<T> >
         class RBTree;

# include "../iterators/bidirectional_iterator.hpp"

# define BLACK 0
# define RED 1

template < class T >
struct Node
{
    typedef T       value_type;
    typedef T*      pointer;

    Node    *getSibling() const
    {
        if (this->parent == NULL)
            return (this->leaf);
        else if (this == this->parent->right)
            return (this->parent->left);
        else
            return (this->parent->right);
    }

    Node    *getUncle() const
    {
        if (this->parent == NULL || this->parent->parent == NULL)
            return (this->leaf);
        else if (this->parent == this->parent->parent->right)
            return (this->parent->parent->left);
        else
            return (this->parent->parent->right);
    }

    bool    isRightChild() const
    {
        return (this->parent != NULL && this->parent->right == this);
    }

    bool    isLeftChild() const
    {
        return (this->parent != NULL && this->parent->left == this);
    }

    bool    hasRedChild() const
    {
        return ((this->left != this->leaf && this->left->color == RED) ||
                (this->right != this->leaf && this->right->color == RED));
    }

    //value_type  data;
    pointer     data;
    bool        color;
    Node        *left;
    Node        *right;
    Node        *parent;
    Node        *root;
    Node        *leaf;
};

template < class T, class Compare, class ValueCompare, class Alloc >
class RBTree
{
    public:
        typedef T               value_type;
        typedef Compare         compare;
        typedef ValueCompare    value_compare;
        typedef Alloc           alloc_type;
        typedef T*              pointer;
        typedef T&              reference;

        typedef ft::bidirectional_iterator<T, false, Compare, ValueCompare>  iterator;
        typedef ft::bidirectional_iterator<T, true, Compare, ValueCompare>  const_iterator;
        typedef std::allocator< Node<T> >  node_alloc_type;

        RBTree(const compare &comp = compare())
            : _lowest(NULL), _highest(NULL), _comp(comp),
            _val_comp(comp), _alloc(alloc_type()), _node_alloc(node_alloc_type())
    {
        this->_root = NULL;
        this->_leaf = this->_node_alloc.allocate(1);
        this->_leaf->leaf = this->_leaf;
        this->_leaf->right = NULL;
        this->_leaf->left = NULL;
        this->_leaf->parent = NULL;
        this->_leaf->data = NULL;
        //this->_root = this->_createRootNode();
    }

        /*RBTree(const RBTree &x) : _lowest(NULL), _highest(NULL),
          _comp(x._comp), _val_comp(x._val_comp), _alloc(x._alloc), _node_alloc(x._node_alloc)
          {
          this->_root = NULL;
          this->_leaf = this->_node_alloc.allocate(1);
          this->_leaf->leaf = this->_leaf;
          this->_leaf->right = NULL;
          this->_leaf->left = NULL;
          this->_leaf->parent = NULL;
          this->_leaf->data = NULL;
        //this->_root = this->_createRootNode();
        iterator    it(x._lowest);

        for ( ; it != x._highest->right ; ++it)
        this->insert(*it);
        }*/

        ~RBTree()
        {
            this->_node_alloc.deallocate(this->_leaf, 1);
        }

        Node<T>  *getRoot() const {return (this->_root);}

        Node<T>  *getLowest() const {return (this->_lowest);}

        Node<T>  *getHighest() const {return (this->_highest);}

        Node<T> *getLeaf() const {return (this->_leaf);}

        Node<T>  *insert(const value_type &data, Node<T> *hint = NULL)
        {
            if (this->_root == NULL)
                return (this->_createRootNode(data));

            //this->display(this->_root, ".", false);
            Node<T>  *newNode = this->_createNode(data);
            //Node<T>  *tmp = (_checkHint(hint, data) ? hint.getNode() : this->_root);
            Node<T> *tmp = hint == NULL ? this->_root : hint;
            Node<T>  *tmp2 = tmp;

            while (tmp != this->_leaf)
            {
                tmp2 = tmp;
                if (this->_val_comp(data, *(tmp->data)))
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            }
            if (this->_val_comp(*(tmp2->data), data))
                tmp2->right = newNode;
            else
                tmp2->left = newNode;
            newNode->parent = tmp2;
            if (newNode->parent != NULL && newNode->parent->parent != NULL)
                _insertBalance(newNode);
            //this->display(this->_root, ".", false);
            return (newNode);
        }

        void    deleteNode(Node<T> *node)
        {
            std::cout << "Delete Start " << std::endl;
            this->display(this->_root, ".", false);
            Node<T>  *r = this->_getReplaceNode(node);
            bool    isDB = this->_checkDoubleBlack(node, r);

            this->_deleteUpdateLowHigh(node);
            if (r == this->_leaf)
            {
                std::cout << " 1 " <<  std::endl;
                this->_deleteLeaf(node, isDB);
            }
            else if ((node->left == this->_leaf || node->right == this->_leaf))
            {

                std::cout << " 2 " <<  std::endl;
                this->_deleteNodeOneChild(node, r, isDB);
            }
            else
            {

                std::cout << "before swap  = " <<  r->data->first << std::endl;
                ft::swap(r->data, node->data);
                std::cout << "after swap  = " <<  r->data->first << std::endl;
                this->deleteNode(r);
            }

            std::cout << "Delete edn " << std::endl;
            this->display(this->_root, ".", false);
        }



        /*        void    deleteNode(Node<T> *v)
                  {
                  Node<T> *u = _getReplaceNode(v);
                  bool uvBlack = _checkDoubleBlack(v, u);
                  Node<T> *parent = v->parent;

                  if (u == _leaf)
                  {
                  if (v == _root)
                  {
                  _root = NULL;
                  }
                  else
                  {
                  if (uvBlack)
                  {
                  _fixDoubleBlack(v);
                  }
                  else
                  {
                  if (v->getSibling() != _leaf)
                  v->getSibling()->color = RED;
                  }

                  if (v->isLeftChild())
                  parent->left = _leaf;
                  else
                  parent->right = _leaf;
                  }
                  _freeNode(v);
                  return;
                  }


                  if (v->left == _leaf || v->right == _leaf)
                  {
                  if (v == _root)
                  {
                  ft::swap(v->data, u->data);
                  v->left = v->right = _leaf;
                  _freeNode(u);
                  }
                  else
                  {
                  if (v->isLeftChild())
                  parent->left = u;
                  else
                  parent->right = u;

                  _freeNode(v);
                  u->parent = parent;
                  if (uvBlack)
                  _fixDoubleBlack(u);
                  else
                  u->color = BLACK;
                  }
                  return ;
                  }

                  ft::swap(u->data,v->data);
                  _freeNode(u);
                  }*/

        void    display(Node<T> const *node, std::string indent, bool side) const
        {
            if (node->data != NULL)
            {
                std::cout << indent;
                if (side)
                {
                    std::cout << "R----";
                    indent += "   ";
                }
                else
                {
                    std::cout << "L----";
                    indent += "|  ";
                }
                std::string color = node->color ? "RED" : "BLACK";
                std::cout << node->data->first << ", " << node->data->second <<
                    "(" << color << ")" << std::endl;
                display(node->left, indent, false);
                display(node->right, indent, true);
            }
        }

        /*Node<T> *operator[](value_type data)
          {
          Node<T>  *tmp = this->_root;

          while (tmp != NULL && tmp->data != data)
          {
          if (data > tmp->data)
          tmp = tmp->right;
          else
          tmp = tmp->left;
          }
          return (tmp);
          }*/

    private:
        Node<T> *_root;
        Node<T> *_lowest;
        Node<T> *_highest;
        Node<T> *_leaf;
        compare _comp;
        value_compare   _val_comp;
        alloc_type  _alloc;
        node_alloc_type _node_alloc;  

        /*Node<T> *_createNode(const value_type &data)
          {
        //Node<T, Alloc>  newNode(data);
        Node<T>  *newNode = this->_node_alloc.allocate(1);

        newNode->data = this->_alloc.allocate(1);
        this->_alloc.construct(newNode->data, data);
        newNode->right = NULL;
        newNode->left = NULL;
        newNode->parent = NULL;
        newNode->color = RED;
        this->_insertUpdateLowHigh(newNode);
        return (newNode);
        }*/

        Node<T> *_createNode(const value_type &data)
        {
            Node<T> *newNode = this->_node_alloc.allocate(1);

            newNode->data = this->_alloc.allocate(1);
            this->_alloc.construct(newNode->data, data);
            newNode->right = this->_leaf;
            newNode->left = this->_leaf;
            newNode->leaf = this->_leaf;
            newNode->parent = NULL;
            newNode->color = RED;
            this->_insertUpdateLowHigh(newNode);
            return (newNode);
        }

        Node<T> *_createRootNode(const value_type &data)
        {
            this->_root = this->_createNode(data);
            this->_root->color = BLACK;
            return (this->_root);
        }

        /*Node<T> *_initNodeData(Node<T> *current, const value_type &data)
          {
          std::cout << "init node data" << std::endl;
          this->_createLeafNode(current);
          current->data = this->_alloc.allocate(1);
          this->_alloc.construct(current->data, data);
          if (current != this->_root)
          current->color = RED;
          this->_insertUpdateLowHigh(current);
          return (current);
          }

          Node<T> *_createRootNode()
          {
          this->_root = this->_node_alloc.allocate(1);
          this->_root->parent = NULL;
          this->_root->data = NULL;
        //this->_root->root = this->_root;
        //this->_initNodeData(this->_root, data);
        this->_root->color = BLACK;
        this->_createLeafNode(this->_root);
        this->_insertUpdateLowHigh(this->_root);
        return (this->_root);
        }

        void    _createLeafNode(Node<T> *node)
        {
        node->right = this->_node_alloc.allocate(1);
        node->right->data = NULL;
        node->right->parent = node;
        node->right->root = this->_root;
        node->left = this->_node_alloc.allocate(1);
        node->left->data = NULL;
        node->left->parent = node;
        node->left->root = this->_root;
        node->right = this->_leaf;
        node->left = this->_leaf;
        }*/

        /// Insert helpers functions

        Node<T>  *_insertRoot(const value_type &data)
        {
            this->_root = this->_createNode(data);//TODO insert root node
            this->_root->parent = NULL;
            this->_root->color = BLACK;
            return (this->_root);
        }

        void    _insertUpdateLowHigh(Node<T> *node)
        {
            if (this->_lowest == NULL ||
                    this->_val_comp(*(node->data), *(this->_lowest->data)))
                this->_lowest = node;
            if (this->_highest == NULL ||
                    this->_val_comp(*(this->_highest->data), *(node->data)))
            {
                this->_highest = node;
                this->_leaf->parent = node;
            }
        }

        void    _insertBalance(Node<T> *node)
        {
            Node<T>  *uncle;

            while (node->parent->color == RED)
            {
                uncle = node->getUncle();
                if (uncle != this->_leaf && uncle->color == RED)
                    this->_fixUncleRed(uncle, &node);
                else if (node->parent->isRightChild())
                    this->_insertBalanceRight(&node);
                else
                    this->_insertBalanceLeft(&node);
                if (node == this->_root)
                    break;
            }
            this->_root->color = BLACK;
        }

        void    _fixUncleRed(Node<T> *uncle, Node<T> **node)
        {
            uncle->color = BLACK;
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            *node = (*node)->parent->parent;
        }

        void    _insertBalanceRight(Node<T> **node)
        {
            if ((*node)->isLeftChild())
            {
                *node = (*node)->parent;
                _rotateRight(*node);
            }
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            _rotateLeft((*node)->parent->parent);
        }

        void    _insertBalanceLeft(Node<T> **node)
        {
            if ((*node)->isRightChild())
            {
                *node = (*node)->parent;
                _rotateLeft(*node);
            }
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            _rotateRight((*node)->parent->parent);
        }

        /// Delete helpers functions

        void    _deleteUpdateLowHigh(Node<T> *node)
        {
            if (node == this->_highest)
            {
                this->_highest = node->parent;
                this->_leaf->parent = node->parent;
            }
            if (node == this->_lowest)
                this->_lowest = node->parent;
        }

        Node<T> *_getSuccessor(Node<T> *node)
        {
            Node<T>  *tmp = node;

            while (tmp->left != this->_leaf)
                tmp = tmp->left;
            return (tmp);
        }

        Node<T> *_getReplaceNode(Node<T> *node)
        {
            if (node->left != this->_leaf && node->right != this->_leaf)
                return (_getSuccessor(node->right));
            else if (node->left != this->_leaf)
                return (node->left);
            else if (node->right != this->_leaf)
                return (node->right);
            return (this->_leaf);
        }

        void    _siblingRedChild(Node<T> *sibling, Node<T> *parent)
        {
            if (sibling->left != this->_leaf && sibling->left->color == RED)
            {
                if (sibling->isLeftChild())
                {
                    sibling->left->color = sibling->color;
                    sibling->color = parent->color;
                    _rotateRight(parent);
                }
                else
                {
                    sibling->left->color = parent->color;
                    _rotateRight(sibling);
                    _rotateLeft(parent);
                }
            }
            else
            {
                if (sibling->isLeftChild())
                {
                    sibling->right->color = parent->color;
                    _rotateLeft(sibling);
                    _rotateRight(parent);
                }
                else
                {
                    sibling->right->color = sibling->color;
                    sibling->color = parent->color;
                    _rotateLeft(parent);
                }
            }
            parent->color = BLACK;
        }

        void    _fixDoubleBlack(Node<T> *node)
        {
            if (node == this->_root) 
                return;
            Node<T>  *sibling = node->getSibling();
            Node<T>  *parent = node->parent;
            if (sibling == this->_leaf)
                _fixDoubleBlack(parent);
            else
            {
                if (sibling->color == RED)
                {
                    parent->color = RED;
                    sibling->color = BLACK;
                    if (sibling->isLeftChild())
                        _rotateRight(parent);
                    else
                        _rotateLeft(parent);
                    _fixDoubleBlack(node);
                }
                else
                {
                    if (sibling->hasRedChild())
                        _siblingRedChild(sibling, parent);
                    else
                    {
                        sibling->color = RED;
                        if (parent->color == BLACK)
                            _fixDoubleBlack(parent);
                        else
                            parent->color = BLACK;
                    }
                }
            }
        }

        bool    _checkDoubleBlack(Node<T> *node, Node<T> *r)
        {
            return ((r == this->_leaf || r->color == BLACK) && node->color == BLACK);
        }

        void    _deleteLeaf(Node<T> *node, bool isDB)
        {
            if (node == this->_root)
                this->_root = NULL;//TODO leak here
            else
            {
                if (isDB)
                    _fixDoubleBlack(node);
                else if (node->getSibling() != this->_leaf)
                    node->getSibling()->color = RED;
            }
            if (node->isLeftChild())
                node->parent->left = this->_leaf;
            else
                node->parent->right = this->_leaf;
            std::cout << "Node of parent = " << node->parent->data->first << std::endl;
            std::cout << "Node of = " << node->data->first << std::endl;
            this->_freeNode(node);
        }

        void    _deleteNodeOneChild(Node<T> *node, Node<T> *r, bool isDB)
        {
            if (node == this->_root)
            {
                std::cout << "node = root" << std::endl;
                //ft::swap(node->data, r->data);
                //node->data = r->data;
                this->_root = r;
                r->parent = NULL;
                r->left = this->_leaf;
                r->right = this->_leaf;
                r->color = BLACK;
                //node->left = this->_leaf;
                //node->right = this->_leaf;
                //r->parent = NULL;
                this->_freeNode(node);
                //this->_freeNode(r);
            }
            else
            {
                Node<T>  *parent = node->parent;

                std::cout << "Delete one child parent val = " << node->data->first << std::endl;
                std::cout << "Delete one child parent val = " << r->data->first << std::endl;
                std::cout << "Delete one child parent val = " << parent->data->first << std::endl;

                if (node->isLeftChild())
                {
                    parent->left = r;
                }
                else
                {
                    parent->right = r;
                }
                r->parent = parent;
                r->left = _leaf;
                r->right = _leaf;
                this->_freeNode(node);

                if (isDB)
                    _fixDoubleBlack(r);
                else
                    r->color = BLACK;
            }
        }

        void    _freeNode(Node<T> *node)
        {
            //this->_node_alloc.deallocate(node->right, 1);
            //this->_node_alloc.deallocate(node->left, 1);
            this->_alloc.destroy(node->data);
            this->_alloc.deallocate(node->data, 1);
            this->_node_alloc.deallocate(node, 1);
        }

        void    _rotateRight(Node<T> *current)
        {
            Node<T>  *x = current->left;

            current->left = x->right;
            if (x->right != this->_leaf)
                x->right->parent = current;
            x->parent = current->parent;
            if (current->parent == NULL)
                this->_root = x;
            else if (current->isRightChild())
                current->parent->right = x;
            else
                current->parent->left = x;
            x->right = current;
            current->parent = x;
        }

        /*void    _rotateRight(Node<T> *current)
          {
          Node<T>  *x = current->left;

          current->left->data = x->right->data;
          if (x->right->data != NULL)
          x->right->parent->data = current->data;
          x->parent->data = current->parent->data;
          if (current->parent->data == NULL)
          this->_root->data = x->data;
          else if (current->isRightChild())
          current->parent->right->data = x->data;
          else
          current->parent->left->data = x->data;
          x->right->data = current->data;
          current->parent->data = x->data;
          }*/


        void    _rotateLeft(Node<T> *current)
        {
            Node<T>  *x = current->right;

            current->right = x->left;
            if (x->left != this->_leaf)
                x->left->parent = current;
            x->parent = current->parent;
            if (current->parent == NULL)
                this->_root = x;
            else if (current->isLeftChild())
                current->parent->left = x;
            else
                current->parent->right = x;
            x->left = current;
            current->parent = x;
        }

        /*void    _rotateLeft(Node<T> *current)
          {
          Node<T>  *x = current->right;

          std::cout << "rotate left current: " << current->data->first << std::endl;
          current->right->data = x->left->data;
          if (x->left->data != NULL)
          x->left->parent->data = current->data;
          x->parent->data = current->parent->data;
          if (current->parent == NULL)
          this->_root->data = x->data;
          else if (current->isLeftChild())
          current->parent->left->data = x->data;
          else
          current->parent->right->data = x->data;
          x->left->data = current->data;
          current->parent->data = x->data;
          }*/

};


#endif // RB_TREE_HPP
