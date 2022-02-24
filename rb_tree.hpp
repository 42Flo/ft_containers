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
                Node(value_type &val) : left(NULL), right(NULL), parent(NULL),
                    color(BLACK)
                {
                    std::cout << "Node constructor called!" << std::endl;
                }

                value_type  data;
                bool        color;
                Node        *left;
                Node        *right;
                Node        *parent;
        };

        typedef typename Alloc::template rebind<Node>::other node_alloc_type;
        typedef Node*   node_ptr;
        typedef Node&   node_ref;

        RBTree() : _root(NULL), _node_alloc(node_alloc_type()), _alloc(alloc_type())
        {
        }

        node_ptr    createNode(value_type data)
        {
            //TODO create node and call node constructor to init value,
            //allocate node with allocator
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
                std::cout << "rbtree insert here!" << std::endl;
                _root = createNode(data);
            }
        }

    private:
        node_ptr        *_root;
        node_alloc_type _node_alloc;  
        alloc_type      _alloc;
        
};


#endif // RB_TREE_HPP
