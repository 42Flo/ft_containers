#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include <memory>

# include "type_traits.hpp"
# include "rb_tree.hpp"
# include "tools.hpp"

namespace ft
{
    template < class Key, class T, class Compare = std::less<Key>,
             class Alloc = std::allocator< ft::pair<const Key, T> > >
    class map
    {
        public:
            typedef Key                                         key_type;
            typedef T                                           mapped_type;
            typedef ft::pair<const key_type, mapped_type>       value_type;
            typedef Compare                                     key_compare;
            typedef Alloc                                       allocator_type;
            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            typedef size_t                                      size_type;
            typedef ptrdiff_t                                   difference_type;

        private:
            class bidirectional_iterator
            {
                public:
                    // Default
                    bidirectional_iterator(Node<value_type, Alloc> *src = 0,
                        const key_compare &comp = key_compare())
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

                    Node<value_type, Alloc> *getCurrent() const{ return (this->_cur);}

                    key_compare key_comp() const{ return (this->_comp);}

                    // Referencing
                    //bidirectional_iterator  operator*(){ return (*(this->_cur));}
                    Node<value_type, Alloc>  operator*(){ return (*(this->_cur));}//not sure about that

                    pointer operator->(){ return (&(this->_cur->data));}

                    // Increment / Decrement
                    bidirectional_iterator  &operator++()
                    {
                        bidirectional_iterator  parent = this->_cur->parent;

                        if (this->_cur->right == NULL && parent != NULL &&
                            !this->_comp((*this)->first, parent->first))
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
                            this->_comp((*this)->first, parent->first))
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

                private:
                    Node<value_type, Alloc> *_cur;
                    key_compare             _comp;

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

            class value_compare : std::binary_function<value_type, value_type, bool>
            {
                public:
                    Compare comp;
                    value_compare(Compare c) : comp(c){}

                    bool    operator()(const value_type &x, const value_type &y) const
                    {
                        return (comp(x.first, y.first));
                    }
            };

        public:
            typedef bidirectional_iterator                      iterator;
            //TODO const iterators

            // Default
            explicit map(const key_compare &comp = key_compare(),
                const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc)
            {
                //TODO
            }

            // Copy
            map(const map &x)
            {
                //TODO
            }

            // Fill in range
            template < class InputIterator >
                map(InputIterator first, InputIterator last,
                const key_compare &comp = key_compare(),
                const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc)
            {
                    //TODO
            }

            // Destructor
            ~map()
            {
                //TODO
            }

            // Capacity / size
            size_type   size() const { return (this->_size);}

            bool    empty() const { return ((this->_size == 0) ? true : false);}

            // Element access

            /*mapped_type &operator[](const key_type &k)
            {
            }*/

            // Observers
            key_compare key_comp() const { return (this->_comp);}


            value_compare   value_comp() const {return (value_compare(this->_comp));}

            // Iterators
            iterator    begin()
            {
                Node<value_type, Alloc> *node = this->_tree.getRoot();

                while (node->left != NULL && !value_comp()(node->data, node->left->data))
                    node = node->left;
                return (iterator(node));
            }

            iterator    end()
            {
                Node<value_type, Alloc>  *node = this->_tree.getRoot();

                while (node->right != NULL && value_comp()(node->data, node->right->data))
                    node = node->right;
                return (iterator(node));
            }

            // Allocator
            allocator_type  get_allocator() const { return (this->_alloc);}

        private:
            RBTree<value_type, Compare, Alloc>    _tree;
            key_compare         _comp;
            size_type           _size;
            allocator_type      _alloc;
    };
}

#endif
