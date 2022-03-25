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

template < class T, class Compare = std::less<T>, class KeyCompare = Compare,
         class Alloc = std::allocator<T> >
class RBTree
{
    public:
        typedef T           value_type;
        typedef Compare     compare;
        typedef KeyCompare  key_compare;
        typedef Alloc       alloc_type;
        typedef T*          pointer;
        typedef T&          reference;

    private:
        class bidirectional_iterator
        {
            public:
                // Default
                bidirectional_iterator(Node<value_type, Alloc> *src = 0,
                        const compare &comp = key_compare())
                    : _cur(src), _comp(comp){}

                // Copy
                bidirectional_iterator(bidirectional_iterator const &src)
                    : _cur(src.getCurrent()), _comp(src.key_comp()){}

                // Assignation
                bidirectional_iterator  operator=(bidirectional_iterator &r)
                {
                    if (this != &r)
                        this->_current = r.getCurrent();
                    return (*this);
                }

                Node<value_type, Alloc> *getNode() const{ return (this->_cur);}

                compare comp() const{ return (this->_comp);}

                // Referencing
                //bidirectional_iterator  operator*(){ return (*(this->_cur));}
                value_type  *operator*(){ return (this->_cur->data);}//not sure about that

                pointer operator->(){ return (&(this->_cur->data));}

                // Increment / Decrement
                bidirectional_iterator  &operator++()
                {
                    bidirectional_iterator  parent = this->_cur->parent;

                    if (this->_cur->right == NULL && parent != NULL &&
                            this->_comp((*this)->first, parent->first))
                        this->_cur = this->_cur->parent;
                    else if (this->_cur->right)
                    {
                        this->_cur = this->_cur->right;
                        while (this->_cur->left)
                            this->_cur = this->_cur->left;
                    }
                    return (*this);
                }

                bidirectional_iterator  operator++(int)
                {
                    bidirectional_iterator tmp(this->_cur);

                    ++this;
                    return (tmp);
                }

                bidirectional_iterator  &operator--()
                {
                    bidirectional_iterator  parent = this->_cur->parent;

                    if (this->_cur->left == NULL && parent != NULL &&
                            !this->_comp((*this)->first, parent->first))//TODO change compare function
                        this->_cur = this->_cur->parent;
                    else if (this->_cur->left)
                    {
                        this->_cur = this->_cur->left;
                        while (this->_cur->right)
                            this->_cur = this->_cur->right;
                    }
                    return (*this);
                }

                bidirectional_iterator  operator--(int)
                {
                    bidirectional_iterator  tmp(this->_cur);

                    --this;
                    return (tmp);
                }
                
                key_compare key_comp() const {return (this->_comp);}

            private:
                Node<value_type, Alloc> *_cur;
                compare             _comp;

                // Relational operators
                friend bool operator==(const bidirectional_iterator &l,
                        const bidirectional_iterator &r)
                {
                    return (l.getCurrent() == r.getCurrent());
                }

                friend bool operator!=(const bidirectional_iterator &l,
                        const bidirectional_iterator &r)
                {
                    return (!(l == r));
                }
        };

    public:
        typedef bidirectional_iterator  iterator;
        typedef typename Alloc::template rebind< Node<T, Alloc> >::other node_alloc_type;

        RBTree(const key_compare &key_comp = key_compare())
            : _root(NULL), _comp(compare()), _key_comp(key_comp),
            _node_alloc(node_alloc_type()), _alloc(alloc_type()){}

        Node<T, Alloc> *getRoot() const {return (this->_root);}

        iterator    insert(value_type &data, iterator hint = NULL)
        {//TODO return iterator to data inserted, manage hint
            if (this->_root == NULL)
            {
                this->_root = _createNode(data);
                this->_root->color = BLACK;
                return (this->_root);
            }
            else
            {
                Node<T, Alloc>  *newNode = _createNode(data);
                Node<T, Alloc>  *tmp = (_checkHint() ? hint.getCurrent() : this->_root);
                Node<T, Alloc>  *tmp2 = tmp;

                while (hint != NULL)
                {
                    tmp2 = tmp;
                    if (this->_comp(data, *hint))
                        tmp = tmp->left;
                    else
                        tmp = tmp->right;
                }
                if (this->_comp(tmp2->data, newNode->data))
                    tmp2->right = newNode;
                else
                    tmp2->left = newNode;
                newNode->parent = tmp2;
                if (newNode->parent->parent != NULL)
                    _insertBalance(newNode);
                return (newNode);
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
        compare     _comp;
        key_compare _key_comp;

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

        bool    _checkHint(iterator hint, const value_type &val)
        {
            if (hint == NULL)
                return (false);
            if (this->_comp(val, this->_root->data))
            {
                if (!this->_comp(*hint, this->_root->data) ||
                        hint.getCurrent()->isRightChild() ||
                        (hint.getCurrent()->parent != NULL &&
                        hint.getCurrent()->parent->isRightChild()))
                    return (false);
            }
            else
            {
                if (this->_comp(*hint, this->_root->data) ||
                        hint.getCurrent()->isLeftChild() ||
                        (hint.getCurrent()->parent != NULL &&
                        hint.getCurrent()->parent->isLeftChild()))
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

        node_alloc_type _node_alloc;  
        alloc_type      _alloc;

};


#endif // RB_TREE_HPP
