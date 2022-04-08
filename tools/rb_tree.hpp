#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

template < class T, class Alloc = std::allocator<T> >
class Node;

template < class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
class RBTree;
//TODO try alloc to node in template

# include "../iterators/bidirectional_iterator.hpp"

# define BLACK 0
# define RED 1

template < class T, class Alloc >
class Node
{
    public:
        typedef T       value_type;
        typedef Alloc   alloc_type;

        Node(const value_type &val) : color(RED), left(NULL), right(NULL), 
            parent(NULL), alloc(alloc_type())
        {
            this->alloc.construct(&(this->data), val);
            std::cout << "node constructed here?" << std::endl;
        }//TODO fix node construction

        Node(const Node &x) : data(x.data.first, x.data.second), color(x.color),
            left(x.left), right(x.right), parent(x.parent), alloc(alloc_type())
        {
        }

        ~Node()
        {
            this->alloc.destroy(&(this->data));
        }

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
        alloc_type  alloc;
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
        typedef std::allocator<Node<T, Alloc>>  node_alloc_type;

        RBTree(const compare &comp = compare())
            : _root(NULL), _lowest(NULL), _highest(NULL), _comp(comp),
            _node_alloc(node_alloc_type()), _alloc(alloc_type()){}

        Node<T, Alloc>  *getRoot() const {return (this->_root);}

        Node<T, Alloc>  *insert(const value_type &data, iterator hint = NULL)
        {
            if (this->_root == NULL)
            {
                this->_root = _createNode(data);
                this->_root->color = BLACK;
                return (this->_root);
            }

            //TODO init and update lowest/highest here for iterators begin() end()

            Node<T, Alloc>  *newNode = _createNode(data);
            Node<T, Alloc>  *tmp = (_checkHint(hint, data) ? hint.getNode() : this->_root);
            Node<T, Alloc>  *tmp2 = tmp;

            while (tmp != NULL)
            {
                tmp2 = tmp;
                //if (this->_comp(data, *hint))
                if (this->_comp(data.first, hint->first))
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

        void    deleteNode(Node<T, Alloc> *node)
        {
            Node<T, Alloc>  *r = _getReplaceNode(node);
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

        void    display(Node<T, Alloc> const *node, std::string indent, bool side) const
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

        Node<T, Alloc> *operator[](value_type data)
        {
            Node<T, Alloc>  *tmp = this->_root;

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
        Node<T, Alloc>  *_root;
        Node<T, Alloc>  *_lowest;
        Node<T, Alloc>  *_highest;
        compare _comp;
        alloc_type  _alloc;
        node_alloc_type _node_alloc;  

        Node<T, Alloc> *_createNode(const value_type &data)
        {
            //Node<T, Alloc>  newNode(data);
            Node<T, Alloc>  *newNode = this->_node_alloc.allocate(1);

            this->_alloc.construct(&(newNode->data), data);
            newNode->right = NULL;
            newNode->left = NULL;
            newNode->parent = NULL;
            return (newNode);
        }

        // Insert helpers functions
        void    _insertBalance(Node<T, Alloc> *node)
        {
            Node<T, Alloc>  *uncle;

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

        void    _fixUncleRed(Node<T, Alloc> *uncle, Node<T, Alloc> **node)
        {
            uncle->color = BLACK;
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            *node = (*node)->parent->parent;
        }

        void    _insertBalanceRight(Node<T, Alloc> **node)
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

        void    _insertBalanceLeft(Node<T, Alloc> **node)
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
        Node<T, Alloc> *_getSuccessor(Node<T, Alloc> *node)
        {
            Node<T, Alloc>  *tmp = node;

            while (tmp->left != NULL)
                tmp = tmp->left;
            return (tmp);
        }

        Node<T, Alloc> *_getReplaceNode(Node<T, Alloc> *node)
        {
            if (node->left != NULL && node->right != NULL)
                return (_getSuccessor(node->right));
            else if (node->left != NULL)
                return (node->left);
            else if (node->right != NULL)
                return (node->right);
            return (NULL);
        }

        void    _siblingRedChild(Node<T, Alloc> *node, Node<T, Alloc> *sibling)
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

        void    _fixDoubleBlack(Node<T, Alloc> *node)
        {
            if (node == this->_root) 
                return;
            Node<T, Alloc>  *sibling = node->getSibling();
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

        bool    _checkDoubleBlack(Node<T, Alloc> *node, Node<T, Alloc> *r)
        {
            return ((r == NULL || r->color == BLACK) && node->color == BLACK);
        }

        void    _deleteLeaf(Node<T, Alloc> *node, bool isDB)
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

        void    _deleteNodeOneChild(Node<T, Alloc> *node, Node<T, Alloc> *r, bool isDB)
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
                Node<T, Alloc>  *parent = node->parent;

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

        void    _rotateRight(Node<T, Alloc> *current)
        {
            Node<T, Alloc>  *x = current->left;

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

        void    _rotateLeft(Node<T, Alloc> *current)
        {
            Node<T, Alloc>  *x = current->right;

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
