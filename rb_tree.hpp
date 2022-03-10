#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

# define BLACK 0
# define RED 1

template < class T, class Alloc = std::allocator<T> >
class Node
{
    public:
        typedef T       value_type;
        typedef Alloc   alloc_type;

        Node(value_type &val) : color(RED), left(NULL), right(NULL),
        parent(NULL), alloc(alloc_type())
        {
            this->alloc.construct(&(this->data), val);
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

template < class T, class Compare, class Alloc = std::allocator<T> >
class RBTree
{
    public:
        typedef T       value_type;
        typedef Compare key_compare;
        typedef Alloc   alloc_type;
        typedef T*      pointer;
        typedef T&      reference;

        //typedef typename std::allocator_traits<Alloc>::template
        //   rebind_alloc<Node> node_alloc_type;


        typedef typename Alloc::template rebind< Node<T, Alloc> >::other node_alloc_type;

        RBTree() : _root(NULL),_comp(key_compare()),
            _node_alloc(node_alloc_type()), _alloc(alloc_type()){}

        Node<T, Alloc> *getRoot() const {return (this->_root);}

        void    insert(value_type data)
        {
            if (this->_root == NULL)
            {
                this->_root = _createNode(data);
                this->_root->color = BLACK;
            }
            else
            {
                Node<T, Alloc>  *newNode = _createNode(data);
                Node<T, Alloc>  *tmp = this->_root, *tmp2 = tmp;

                while (tmp != NULL)
                {
                    tmp2 = tmp;
                    //if (data > tmp->data)
                    if (this->_comp(data.first, tmp->data.first))
                        tmp = tmp->left;
                    else
                        tmp = tmp->right;
                }
                tmp2->data < newNode->data ? tmp2->right = newNode : tmp2->left = newNode;
                newNode->parent = tmp2;
                if (newNode->parent->parent != NULL)
                    _insertBalance(newNode);
            }
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
        Node<T, Alloc> *_root;
        key_compare     _comp;

        Node<T, Alloc> *_createNode(value_type &data)
        {
            Node<T, Alloc>  *newNode = this->_node_alloc.allocate(1);

            this->_node_alloc.construct(newNode, data);
            return (newNode);
        }

        // Insert helpers functions
        void    _insertBalance(Node<T, Alloc> *node)//TODO try recursif
        {
            Node<T, Alloc>*uncle;

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

        node_alloc_type _node_alloc;  
        alloc_type      _alloc;

};


#endif // RB_TREE_HPP