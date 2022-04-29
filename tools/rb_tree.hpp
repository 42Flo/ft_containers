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
        return (this->parent != NULL && this->parent->right == this);
    }

    bool    isLeftChild() const
    {
        return (this->parent != NULL && this->parent->left == this);
    }

    bool    hasRedChild() const
    {
        return ((this->left->data != NULL && this->left->color == RED) ||
                (this->right->data != NULL && this->right->color == RED));
    }

    //value_type  data;
    pointer     data;
    bool        color;
    Node        *left;
    Node        *right;
    Node        *parent;
    Node        *root;
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
            this->_root = this->_createRootNode();
        }

        RBTree(const RBTree &x) : _lowest(NULL), _highest(NULL),
            _comp(x._comp), _val_comp(x._val_comp), _alloc(x._alloc), _node_alloc(x._node_alloc)
        {
            this->_root = this->_createRootNode();
            iterator    it(x._lowest);

            for ( ; it != x._highest->right ; ++it)
                this->insert(*it);
        }

        Node<T>  *getRoot() const {return (this->_root);}

        Node<T>  *getLowest() const {return (this->_lowest);}
        
        Node<T>  *getHighest() const {return (this->_highest);}

        Node<T>  *insert(const value_type &data, iterator hint = NULL)
        {
            //if (this->_root == NULL)
            //    return (this->_createRootNode(data));

            //this->display(this->_root, ".", false);
            Node<T>  *newNode;
            //Node<T>  *tmp = (_checkHint(hint, data) ? hint.getNode() : this->_root);
            Node<T> *tmp = this->_root;
            Node<T>  *tmp2 = tmp;

            while (tmp->data != NULL)
            {
                std::cout << "bst loop" << std::endl;
                tmp2 = tmp;
                //if (this->_comp(data.first, tmp->data->first))
                if (this->_val_comp(data, *(tmp->data)))
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            }
            //if (this->_comp(tmp2->data->first, newNode->data->first))
            if (tmp == this->_root)
            {
                std::cout << "root!" << std::endl;
                newNode = this->_initNodeData(this->_root, data);
            }
            else if (this->_val_comp(*(tmp2->data), data))
                newNode = this->_initNodeData(tmp2->right, data);
                //tmp2->right = newNode;
            else
                newNode = this->_initNodeData(tmp2->left, data);
                //tmp2->left = newNode;
            //newNode->parent = tmp2;
            if (newNode->parent != NULL && newNode->parent->parent != NULL)
                _insertBalance(newNode);
            this->display(this->_root, ".", false);
            return (newNode);
        }

        void    deleteNode(Node<T> *node)
        {
            Node<T>  *r = this->_getReplaceNode(node);
            bool    isDB = this->_checkDoubleBlack(node, r);

            this->_deleteUpdateLowHigh(node);
            if (r == NULL)
                this->_deleteLeaf(node, isDB);
            else if (node->left->data == NULL || node->right->data == NULL)
                this->_deleteNodeOneChild(node, r, isDB);
            else
            {
                ft::swap(r->data, node->data);
                this->deleteNode(r);
            }
        }

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

        Node<T> *_initNodeData(Node<T> *current, const value_type &data)
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
        }

        /// Insert helpers functions

        Node<T>  *_insertRoot(const value_type &data)
        {
            this->_root = this->_createNode(data);//TODO insert root node
            this->_root->color = BLACK;
            return (this->_root);
        }

        void    _insertUpdateLowHigh(Node<T> *node)
        {
            if (this->_lowest == NULL ||
                    this->_val_comp(*(node->data), *(this->_lowest->data)))
                    //this->_comp(node->data->first, this->_lowest->data->first))
                this->_lowest = node;
            if (this->_highest == NULL ||
                    this->_val_comp(*(this->_highest->data), *(node->data)))
                    //this->_comp(this->_highest->data->first, node->data->first))
                this->_highest = node;
        }

        void    _insertBalance(Node<T> *node)
        {
            Node<T>  *uncle;

            while (node->parent->color == RED)
            {
                uncle = node->getUncle();
                if (uncle->data != NULL && uncle->color == RED)
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

        bool    _checkHint(iterator hint, const value_type &val)
        {
            if (hint == NULL)
                return (false);
            //if (this->_comp(val.first, this->_root->data->first))
            if (this->_val_comp(val, *(this->_root->data)))
            {
                //if (!this->_comp(hint->first, this->_root->data->first) ||
                if (!this->_val_comp(*hint, *(this->_root->data)) ||
                        hint.getNode()->isRightChild() ||
                        (hint.getNode()->parent != NULL &&
                         hint.getNode()->parent->isRightChild()))
                    return (false);
            }
            else
            {
                //if (this->_comp(*hint, this->_root->data) ||
                //if (this->_comp(hint->first, this->_root->data->first) ||
                if (this->_val_comp(*hint, *(this->_root->data)) ||
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

        /// Delete helpers functions

        void    _deleteUpdateLowHigh(Node<T> *node)
        {
            if (node == this->_highest)
                if (node != this->_root)
                    this->_highest = node->parent;
            if (node == this->_lowest)
                if (node != this->_root)
                    this->_lowest = node->parent;
        }

        Node<T> *_getSuccessor(Node<T> *node)
        {
            Node<T>  *tmp = node;

            while (tmp->left->data != NULL)
                tmp = tmp->left;
            return (tmp);
        }

        Node<T> *_getReplaceNode(Node<T> *node)
        {
            if (node->left->data != NULL && node->right->data != NULL)
                return (_getSuccessor(node->right));
            else if (node->left->data != NULL)
                return (node->left);
            else if (node->right->data != NULL)
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
            else if (node->parent != NULL)
                node->parent->right = NULL;
            this->_freeNode(node);
        }

        void    _deleteNodeOneChild(Node<T> *node, Node<T> *r, bool isDB)
        {
            if (node == this->_root)
            {
                this->_root = r;
                r->parent = NULL;
                r->left = NULL;
                r->right = NULL;
                this->_freeNode(node);
            }
            else
            {
                Node<T>  *parent = node->parent;

                if (node->isLeftChild())
                    parent->left = r;
                else
                    parent->right = r;
                this->_freeNode(node);
                r->parent = parent;
                if (isDB)
                    _fixDoubleBlack(r);
                else
                    r->color = BLACK;
            }
        }

        void    _freeNode(Node<T> *node)
        {
            this->_node_alloc.deallocate(node->right, 1);
            this->_node_alloc.deallocate(node->left, 1);
            this->_alloc.destroy(node->data);
            this->_alloc.deallocate(node->data, 1);
            this->_node_alloc.deallocate(node, 1);
        }

        /*void    _rotateRight(Node<T> *current)
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
        }*/

        void    _rotateRight(Node<T> *current)
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
        }


        /*void    _rotateLeft(Node<T> *current)
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
        }*/
        
        void    _rotateLeft(Node<T> *current)
        {
            /*Node<T>  *x = current->right;

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
            current->parent->data = x->data;*/
        }

};


#endif // RB_TREE_HPP
