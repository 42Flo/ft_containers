#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

# define BLACK 0
# define RED 1

template < class T, class Alloc = std::allocator<T> >
class RBTree
{
    public:
        typedef T       value_type;
        typedef Alloc   alloc_type;
        typedef T*      pointer;
        typedef T&      reference;

        //typedef typename std::allocator_traits<Alloc>::template
        //   rebind_alloc<Node> node_alloc_type;

        class Node
        {
            public:
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

                value_type  data;
                bool        color;
                Node        *left;
                Node        *right;
                Node        *parent;
                alloc_type  alloc;
        };

        typedef typename Alloc::template rebind<Node>::other node_alloc_type;

        RBTree() : _root(NULL), _node_alloc(node_alloc_type()),
            _alloc(alloc_type()){}

        Node    *getRoot() const {return (this->_root);}

        void    insert(value_type data)
        {
            if (this->_root == NULL)
            {
                this->_root = _createNode(data);
                this->_root->color = BLACK;
            }
            else
            {
                Node    *newNode = _createNode(data);
                Node    *tmp = this->_root, *tmp2 = tmp;

                while (tmp != NULL)
                {
                    tmp2 = tmp;
                    if (data > tmp->data)
                        tmp = tmp->right;
                    else
                        tmp = tmp->left;
                }
                tmp2->data < newNode->data ? tmp2->right = newNode : tmp2->left = newNode;
                newNode->parent = tmp2;
                if (newNode->parent->parent != NULL)
                    _insertBalance(newNode);
            }
        }

        void    deleteNode(Node *node)
        {
            Node    *r = _getReplaceNode(node);

            // Case 1: r == NULL, node is leaf
            if (r == NULL)
                _deleteLeaf(node, r);
            // Case 2: node->left == NULL || node->right == NULL, node has 1 child
            else if (node->left == NULL || node->right == NULL)
                _deleteNodeOneChild(node, r);
            // Case 3: else~ node has 2 childs, swap with succesor and recurse
            else
            {
                ft::swap(r->data, node->data);
                this->_node_alloc.destroy(r);
                this->_node_alloc.deallocate(r);
            }
        }

        void    display(Node const *node, std::string indent, bool side) const
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
                std::cout << node->data << "(" << color << ")" << std::endl;
                display(node->left, indent, false);
                display(node->right, indent, true);
            }
        }

        Node    *operator[](value_type data)
        {
            Node    *tmp = this->_root;

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
        Node            *_root;

        Node    *_createNode(value_type &data)
        {
            Node    *newNode = this->_node_alloc.allocate(1);

            this->_node_alloc.construct(newNode, data);
            return (newNode);
        }

        /*Node    *_getSibling(Node *node)
        {
           if (node == node->parent->right) 
               return (node->parent->left);
           else
               return (node->parent->right);
        }

        Node    *_getUncle(Node *node)
        {
            if (node->parent == node->parent->parent->right)
                return (node->parent->parent->left);
            else
                return (node->parent->parent->right);
        }*/

        // Insert helpers functions
        void    _insertBalance(Node *node)
        {
            Node    *uncle;

            while (node->parent->color == RED)
            {
                uncle = node->getUncle();
                if (uncle != NULL && uncle->color == RED)
                    _fixUncleRed(uncle, &node);
                else if (node->parent == node->parent->parent->right)
                    _insertBalanceRight(&node);
                else
                    _insertBalanceLeft(&node);
                if (node == this->_root)
                    break;
            }
            this->_root->color = BLACK;
        }

        void    _fixUncleRed(Node *uncle, Node **node)
        {
            uncle->color = BLACK;
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            *node = (*node)->parent->parent;
        }

        void    _insertBalanceRight(Node **node)
        {
            if (*node == (*node)->parent->left)
            {
                *node = (*node)->parent;
                _rotateRight(*node);
            }
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            _rotateLeft((*node)->parent->parent);
        }

        void    _insertBalanceLeft(Node **node)
        {
            if (*node == (*node)->parent->right)
            {
                *node = (*node)->parent;
                _rotateLeft(*node);
            }
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            _rotateRight((*node)->parent->parent);
        }

        // Delete helpers functions
        Node    *_getSuccessor(Node *node)
        {
            Node    *tmp = node;

            while (tmp->left != NULL)
                tmp = tmp->left;
            return (tmp);
        }

        Node    *_getReplaceNode(Node *node)
        {
            if (node->left != NULL && node->right != NULL)
                return (_getSuccessor(node->right));
            else if (node->left != NULL)
                return (node->left);
            else if (node->right != NULL)
                return (node->right);
            return (NULL);
        }

        void    _fixDoubleBlack(Node *node)
        {
           if (node == this->_root) 
               return;
           Node *sibling = node->getSibling();
           if (sibling == NULL)
               _fixDoubleBlack(node->parent);
           else
           {
                //TODO sibling RED, BLACK cases
           }
        }

        void    _deleteLeaf(Node *node, Node *r)
        {
            if (node == this->_root)
                this->_root = NULL;
            else
            {
                if ((r == NULL || r->color == BLACK) && node->color == BLACK)
                {
                    //TODO fix double black
                }
            }
        }

        void    _deleteNodeOneChild(Node *node, Node *r)
        {
            if (node == this->_root)
            {
                node->data = r->data;
                node->left =  NULL;
                node->right = NULL;
                this->_node_alloc.destroy(r);
                this->_node_alloc.deallocate(r);
            }
        }

        void    _rotateRight(Node *current)
        {
            Node    *x = current->left;

            current->left = x->right;
            if (x->right != NULL)
                x->right->parent = current;
            x->parent = current->parent;
            if (current->parent == NULL)
                this->_root = x;
            else if (current == current->parent->right)
                current->parent->right = x;
            else
                current->parent->left = x;
            x->right = current;
            current->parent = x;
        }

        void    _rotateLeft(Node *current)
        {
            Node    *x = current->right;

            current->right = x->left;
            if (x->left != NULL)
                x->left->parent = current;
            x->parent = current->parent;
            if (current->parent == NULL)
                this->_root = x;
            else if (current == current->parent->left)
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
