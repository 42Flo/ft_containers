#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

template < class T >
struct Node;

template < class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
class RBTree;
//TODO try alloc to node in template

# include "../iterators/bidirectional_iterator.hpp"

# define BLACK 0
# define RED 1

template < class T >
struct Node
{
    typedef T       value_type;

    Node    *getSibling() const
    {
        if (this->parent == NULL)
            return (NULL);
        else if (this == this->parent->right)
            return (this->parent->left);
        else
            return (this->parent->right);
    }

    Node    *getUncle() const
    {
        if (this->parent == NULL || this->parent->parent == NULL)
            return (NULL);
        else if (this->parent == this->parent->parent->right)
            return (this->parent->parent->left);
        else
            return (this->parent->parent->right);
    }

    bool    isRightChild() const
    {
        return (this->parent->right == this);
    }

    bool    isLeftChild() const
    {
        return (this->parent->left == this);
    }

    bool    hasRedChild() const
    {
        return ((this->left != NULL && this->left->color == RED) ||
                (this->right != NULL && this->right->color == RED));
    }

    value_type  data;
    bool        color;
    Node        *left;
    Node        *right;
    Node        *parent;
};

template < class T, class Compare, class Alloc >
class RBTree
{
    public:
        typedef T           value_type;
        typedef Compare     compare;
        typedef Alloc       alloc_type;
        typedef T*          pointer;
        typedef T&          reference;

        typedef ft::bidirectional_iterator<T, Compare>  iterator;
        typedef ft::bidirectional_iterator<const T, Compare>  const_iterator;
        //typedef typename Alloc::template rebind< Node<T, Alloc> >::other node_alloc_type;
        typedef std::allocator<Node<T>>  node_alloc_type;

        RBTree(const compare &comp = compare())
            : _root(NULL), _lowest(NULL), _highest(NULL), _comp(comp),
            _alloc(alloc_type()), _node_alloc(node_alloc_type()){}

        Node<T>  *getRoot() const {return (this->_root);}

        Node<T>  *getLowest() const {return (this->_lowest);}

        Node<T>  *getHighest() const {return (this->_highest);}

        Node<T>  *insert(const value_type &data, iterator hint = NULL)
        {
            if (this->_root == NULL)
                return (this->_insertRoot(data));

            Node<T>  *newNode = _createNode(data);
            Node<T>  *tmp = (_checkHint(hint, data) ? hint.getNode() : this->_root);
            Node<T>  *tmp2 = tmp;

            while (tmp != NULL)
            {
                tmp2 = tmp;
                //if (this->_comp(data, *hint))
                if (this->_comp(data.first, tmp->data.first))
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            }
            //if (this->_comp(tmp2->data, newNode->data))
            if (this->_comp(tmp2->data.first, newNode->data.first))
                tmp2->right = newNode;
            else
                tmp2->left = newNode;
            newNode->parent = tmp2;
            if (newNode->parent->parent != NULL)
                _insertBalance(newNode);
            return (newNode);
        }

        void    deleteNode(Node<T> *node)
        {
            Node<T>  *r = _getReplaceNode(node);
            bool            isDB = _checkDoubleBlack(node, r);

            if (r == NULL)
                _deleteLeaf(node, isDB);
            else if (node->left == NULL || node->right == NULL)
                _deleteNodeOneChild(node, r, isDB);
            else
            {
                ft::swap(r->data, node->data);
                deleteNode(r);
            }
        }

        void    display(Node<T> const *node, std::string indent, bool side) const
        {
            if (node != NULL)
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
                std::cout << node->data.first << ", " << node->data.second <<
                    "(" << color << ")" << std::endl;
                display(node->left, indent, false);
                display(node->right, indent, true);
            }
        }

        Node<T> *operator[](value_type data)
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
        }

    private:
        Node<T>  *_root;
        Node<T>  *_lowest;
        Node<T>  *_highest;
        compare _comp;
        alloc_type  _alloc;
        node_alloc_type _node_alloc;  

        Node<T> *_createNode(const value_type &data)
        {
            //Node<T, Alloc>  newNode(data);
            Node<T>  *newNode = this->_node_alloc.allocate(1);

            this->_alloc.construct(&(newNode->data), data);
            newNode->right = NULL;
            newNode->left = NULL;
            newNode->parent = NULL;
            this->_updateLowHigh(newNode);
            return (newNode);
        }

        // Insert helpers functions

        Node<T>  *_insertRoot(const value_type &data)
        {
            this->_root = this->_createNode(data);
            this->_root->color = BLACK;
            return (this->_root);
        }

        void    _updateLowHigh(Node<T> *node)
        {
            if (this->_lowest == NULL ||
                    this->_comp(node->data.first, this->_lowest->data.first))
                this->_lowest = node;
            if (this->_highest == NULL ||
                    this->_comp(this->_highest->data.first, node->data.first))
                this->_highest = node;
        }

        void    _insertBalance(Node<T> *node)
        {
            Node<T>  *uncle;

            while (node->parent->color == RED)
            {
                uncle = node->getUncle();
                if (uncle != NULL && uncle->color == RED)
                    _fixUncleRed(uncle, &node);
                else if (node->parent->isRightChild())
                    _insertBalanceRight(&node);
                else
                    _insertBalanceLeft(&node);
                if (node == this->_root)
                    break;
            }
            this->_root->color = BLACK;
        }

        bool    _checkHint(iterator hint, const value_type &val)
        {
            if (hint == NULL)
                return (false);
            //if (this->_comp(val, this->_root->data))
            if (this->_comp(val.first, this->_root->data.first))
            {
                //if (!this->_comp(*hint, this->_root->data) ||
                if (!this->_comp(hint->first, this->_root->data.first) ||
                        hint.getNode()->isRightChild() ||
                        (hint.getNode()->parent != NULL &&
                         hint.getNode()->parent->isRightChild()))
                    return (false);
            }
            else
            {
                //if (this->_comp(*hint, this->_root->data) ||
                if (this->_comp(hint->first, this->_root->data.first) ||
                        hint.getNode()->isLeftChild() ||
                        (hint.getNode()->parent != NULL &&
                         hint.getNode()->parent->isLeftChild()))
                    return (false);
            }
            return (true);
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

        // Delete helpers functions
        Node<T> *_getSuccessor(Node<T> *node)
        {
            Node<T>  *tmp = node;

            while (tmp->left != NULL)
                tmp = tmp->left;
            return (tmp);
        }

        Node<T> *_getReplaceNode(Node<T> *node)
        {
            if (node->left != NULL && node->right != NULL)
                return (_getSuccessor(node->right));
            else if (node->left != NULL)
                return (node->left);
            else if (node->right != NULL)
                return (node->right);
            return (NULL);
        }

        void    _siblingRedChild(Node<T> *node, Node<T> *sibling)
        {
            if (sibling->left != NULL && sibling->left->color == RED)
            {
                if (sibling->isLeftChild())
                {
                    sibling->left->color = sibling->color;
                    sibling->color = node->parent->color;
                    _rotateRight(node->parent);
                }
                else
                {
                    sibling->left->color = node->parent->color;
                    _rotateRight(sibling);
                    _rotateLeft(node->parent);
                }
            }
            else
            {
                if (sibling->isLeftChild())
                {
                    sibling->right->color = node->parent->color;
                    _rotateLeft(sibling);
                    _rotateRight(node->parent);
                }
                else
                {
                    sibling->right->color = sibling->color;
                    sibling->color = node->parent->color;
                    _rotateLeft(node->parent);
                }
            }
            node->parent->color = BLACK;
        }

        void    _fixDoubleBlack(Node<T> *node)
        {
            if (node == this->_root) 
                return;
            Node<T>  *sibling = node->getSibling();
            if (sibling == NULL)
                _fixDoubleBlack(node->parent);
            else
            {
                if (sibling->color == RED)
                {
                    node->parent->color = RED;
                    sibling->color = BLACK;
                    if (sibling->isLeftChild())
                        _rotateRight(node->parent);
                    else
                        _rotateLeft(node->parent);
                    _fixDoubleBlack(node);
                }
                else
                {
                    if (sibling->hasRedChild())
                        _siblingRedChild(node, sibling);
                    else
                    {
                        sibling->color = RED;
                        if (node->parent->color == BLACK)
                            _fixDoubleBlack(node->parent);
                        else
                            node->parent->color = BLACK;
                    }
                }
            }
        }

        bool    _checkDoubleBlack(Node<T> *node, Node<T> *r)
        {
            return ((r == NULL || r->color == BLACK) && node->color == BLACK);
        }

        void    _deleteLeaf(Node<T> *node, bool isDB)
        {
            if (node == this->_root)
                this->_root = NULL;
            else
            {
                if (isDB)
                    _fixDoubleBlack(node);
                else if (node->getSibling() != NULL)
                    node->getSibling()->color = RED;
            }
            if (node->isLeftChild())
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
            this->_node_alloc.destroy(node);
            this->_node_alloc.deallocate(node, 1);
        }

        void    _deleteNodeOneChild(Node<T> *node, Node<T> *r, bool isDB)
        {
            if (node == this->_root)
            {
                node->data = r->data;
                node->left =  NULL;
                node->right = NULL;
                this->_node_alloc.destroy(r);
                this->_node_alloc.deallocate(r, 1);
            }
            else
            {
                Node<T>  *parent = node->parent;

                if (node->isLeftChild())
                    parent->left = r;
                else
                    parent->right = r;
                this->_node_alloc.destroy(node);
                this->_node_alloc.deallocate(node, 1);
                r->parent = parent;
                if (isDB)
                    _fixDoubleBlack(r);
                else
                    r->color = BLACK;
            }
        }

        void    _rotateRight(Node<T> *current)
        {
            Node<T>  *x = current->left;

            current->left = x->right;
            if (x->right != NULL)
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

        void    _rotateLeft(Node<T> *current)
        {
            Node<T>  *x = current->right;

            current->right = x->left;
            if (x->left != NULL)
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
};


#endif // RB_TREE_HPP
