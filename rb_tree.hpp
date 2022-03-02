#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

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

        Node const  *getRoot() const {return (this->_root);}

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
                Node    *tmp = this->_root;
                Node    *tmp2 = tmp;

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

    private:
        Node            *_root;

        Node    *_createNode(value_type &data)
        {
            Node    *newNode = this->_node_alloc.allocate(1);

            this->_node_alloc.construct(newNode, data);
            return (newNode);
        }

        void    _insertBalance(Node *node)
        {
            while (node->parent->color == RED)
            {
                if (node->parent == node->parent->parent->right)
                    _insertBalanceRight(&node);
                else
                    _insertBalanceLeft(&node);
                if (node == this->_root)
                    break;
            }
            this->_root->color = BLACK;
        }

        void    _insertUncleRedFix(Node *uncle, Node **node)
        {
            uncle->color = BLACK;
            (*node)->parent->color = BLACK;
            (*node)->parent->parent->color = RED;
            *node = (*node)->parent->parent;
        }

        bool    _isUncleRed(Node *uncle)
        {
            if (uncle != NULL)
                if (uncle->color == RED)
                    return (true);
            return (false);
        }

        void    _insertBalanceRight(Node **node)
        {
            if (_isUncleRed((*node)->parent->parent->left) == true)
                _insertUncleRedFix((*node)->parent->parent->left, node);
            else
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
        }

        void    _insertBalanceLeft(Node **node)
        {
            if (_isUncleRed((*node)->parent->parent->right) == true)
                _insertUncleRedFix((*node)->parent->parent->right, node);
            else
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
