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

            //TODO const iterators

            class value_compare : public std::binary_function<value_type, value_type, bool>
            {
                friend class map;

                protected:
                    Compare comp;
                    value_compare(Compare c) : comp(c){}

                public:
                    bool    operator()(const value_type &x, const value_type &y) const
                    {
                        return (x.first, y.first);
                    }
            };

            typedef typename RBTree<value_type, Compare, value_compare, Alloc>::
                bidirectional_iterator    iterator;

            // Default
            explicit map(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc), _tree(value_comp())
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

            size_type   max_size() const
            {
                return (this->_alloc.max_size);//test that
            }

            // Element access

            /*mapped_type &operator[](const key_type &k)
              {
              }*/

            // Modifiers
            // insert(): single element
            ft::pair<iterator, bool>    insert(const value_type &val)
            {
            }

            // insert(); with hint
            iterator    insert(iterator position, const value_type &val)
            {

            }

            // insert(): in range
            template < class InputIterator >
                void    insert(InputIterator first, InputIterator last)
                {

                }

            // Observers
            key_compare key_comp() const {return (this->_comp);}


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

            // Operations

            iterator        find(const key_type &k)
            {
                iterator    it(this->_tree.getRoot());

                while (it != NULL)
                {
                    if (!this->_comp(it->first, k) && !this->_comp(k, it->first))
                        return (it);
                    else if (this->_comp(it->first, k))
                        *it = (*it)->right;
                    else
                        *it = (*it)->left;
                }
                return (this->end());
            }

            //TODO find with const iterator

            size_type       count(const key_type &k) const
            {
                if (this->find(k) != this->end())
                    return (1);
                return (0);
            }

            iterator        lower_bound(const key_type &k)
            {
                iterator    it(this->begin());

                while (this->_comp(it->first, k))
                    ++it;
                return (it);
            }

            //TODO upper/ lower bound with const iterator

            iterator        upper_bound(const key_type &k)
            {
                iterator    it(this->begin());

                while (!this->_comp(k, it->first))
                    ++it;
                return (it);
            }

            ft::pair<iterator, iterator>    equal_range(const key_type &k)
            {
                return (ft::pair<iterator, iterator>
                        (lower_bound(k), upper_bound(k)));
            }

            //TODO equal_range with const iterator

        private:
            RBTree<value_type, Compare, value_compare, Alloc> _tree;
            key_compare                             _comp;
            size_type                               _size;
            allocator_type                          _alloc;
    };

}

#endif
