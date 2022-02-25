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
                    std::cout << "Node constructor called!" << std::endl;
                }

                value_type  data;
                bool        color;
                Node        *left;
                Node        *right;
                Node        *parent;
                alloc_type  alloc;
        };

        typedef typename Alloc::template rebind<Node>::other node_alloc_type;

        RBTree() : _root(NULL), _node_alloc(node_alloc_type()), _alloc(alloc_type())
        {
        }

        Node    *createNode(value_type &data)
        {
            Node    *newNode = this->_node_alloc.allocate(1);

            this->_node_alloc.construct(newNode, data);
            return (newNode);
        }

        void    rotateLeft(Node *current)
        {
            Node    *x = current->right;
            Node    *y = x->left;

            x->left = current;
            current->right = y;
            current->parent = x;
            if (y != NULL)
                y->parent = current;
        }

        void    rotateRight(Node *current)
        {
            Node    *x = current->left;
            Node    *y = x->right;

            x->right = current;
            current->left = y;
            current->parent = x;
            if (y != NULL)
                y->parent = current;
        }

        void    insert(value_type data)
        {
            if (this->_root == NULL)
            {
                this->_root = createNode(data);
                this->_root->color = BLACK;
            }
            else
            {
                Node    *newNode = createNode(data);
                Node    *tmp = this->_root;
                Node    *tmp2;

                while (tmp != NULL)
                {
                    tmp2 = tmp;
                    if (data > tmp->data)
                        tmp = tmp->right;
                    else
                        tmp = tmp->left;
                }
                tmp = newNode;
                newNode->parent = tmp2;
                if (newNode->parent->parent == NULL)
                    return ;
                //TODO insert fix
            }
        }

    private:
        Node            *_root;
        node_alloc_type _node_alloc;  
        alloc_type      _alloc;
        
};


#endif // RB_TREE_HPP
