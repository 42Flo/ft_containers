#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include <memory>

# include "../tools/type_traits.hpp"
# include "../tools/rb_tree.hpp"
# include "../tools/tools.hpp"
# include "../iterators/reverse_iterator.hpp"
# include "../iterators/bidirectional_iterator.hpp"

namespace ft
{
    template < class Key, class T, class Compare = std::less<Key>,
             class Alloc = std::allocator< ft::pair<const Key, T> > >
    class map
    {
        public:
            typedef Key key_type;
            typedef T   mapped_type;
            typedef ft::pair<const key_type, mapped_type>   value_type;
            typedef Compare key_compare;
            typedef Alloc   allocator_type;
            typedef typename allocator_type::reference  reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer    pointer;
            typedef typename allocator_type::const_pointer  const_pointer;
            typedef size_t  size_type;
            typedef ptrdiff_t   difference_type;

            class value_compare : public std::binary_function<value_type, value_type, bool>
            {
                friend class map;
                friend class RBTree<value_type, value_compare, Alloc>;
                friend class bidirectional_iterator<value_type, value_compare, Alloc>;

                protected:
                    Compare comp;
                    value_compare(Compare c = Compare()) : comp(c){}

                public:
                    bool    operator()(const value_type &x, const value_type &y) const
                    {
                        return (comp(x.first, y.first));
                    }
            };

            typedef typename RBTree<value_type, value_compare, Alloc>::iterator  iterator;
            typedef typename RBTree<value_type, value_compare, Alloc>::const_iterator  const_iterator;
            typedef ft::reverse_iterator<iterator>  reverse_iterator;

            // Default
            explicit map(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _tree(value_comp()), _comp(comp), _alloc(alloc)
            {
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

            // Size / Capacity
            size_type   size() const {return (this->_size);}

            bool    empty() const {return (this->_size == 0);}

            size_type   max_size() const
            {
                return (this->_alloc.max_size());
            }

            // Element access

            mapped_type &operator[](const key_type &k)
            {
                //return (*((this->insert(ft::make_pair(k, mapped_type()))).first).second);
                return (this->insert(ft::make_pair(k, mapped_type()))->second);
            }

            // Modifiers
            // insert(): single element
            ft::pair<iterator, bool>    insert(const value_type &val)
            {
                iterator    it = this->find(val.first);

                if (it != this->end())
                    return (ft::make_pair(it, false));
                ++this->_size;
                return (ft::make_pair(iterator(this->_tree.insert(val), value_comp()), true));
            }

            // insert(): with hint
            iterator    insert(iterator position, const value_type &val)
            {
                if (iterator it = find(val->first) != end())
                    return (it);
                ++this->_size;
                return (this->_tree.insert(val, position));
            }

            // insert(): in range
            template < class InputIterator >
            void    insert(InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                    InputIterator>::type = 0)
            {
                for ( ; first != last ; ++first)
                    this->insert(*first);
            }

            // erase(): with iterator
            void    erase(iterator position)
            {
                this->_tree.deleteNode(position.getCurrent());
            }

            // erase(): with key
            size_type   erase(const key_type &k)
            {
                this->_tree.deleteNode(*(this->find(k)));
                return (1);
            }

            // erase(): in range
            void    erase(iterator first, iterator last)
            {
                for ( ; first != last ; ++first)
                    this->_tree.deleteNode(first.getCurrent());
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
                return (iterator(node, value_comp()));
            }

            iterator    end()
            {
                Node<value_type, Alloc>  *node = this->_tree.getRoot();

                while (node->right != NULL && value_comp()(node->data, node->right->data))
                    node = node->right;
                return (iterator(node, value_comp()));
            }

            reverse_iterator    rbegin()
            {
                return (reverse_iterator(this->end()));
            }

            reverse_iterator    rend()
            {
                return (reverse_iterator(this->begin()));
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
                        ++it;
                        //*it = (*it)->right;
                    else
                        --it;
                        //*it = (*it)->left;
                }
                return (this->end());
            }

            const_iterator  find(const key_type &k) const
            {
                const_iterator  it(this->_tree.getRoot());

                while (it != NULL)
                {
                    if (!this->_comp(it->first, k) && !this->_comp(k, it->first))
                        return (it);
                    else if (this->_comp(it->first, k))
                        ++it;
                        //*it = (*it)->right;
                    else
                        --it;
                        //*it = (*it)->left;
                }
                return (this->end());
            }

            size_type   count(const key_type &k) const
            {
                if (this->find(k) != this->end())
                    return (1);
                return (0);
            }

            iterator    lower_bound(const key_type &k)
            {
                iterator    it(this->begin());

                while (this->_comp(it->first, k))
                    ++it;
                return (it);
            }

            const_iterator  lower_bound(const key_type &k) const
            {
                const_iterator  it(this->begin());

                while (this->_comp(it->first, k))
                    ++it;
                return (it);
            }

            iterator    upper_bound(const key_type &k)
            {
                iterator    it(this->begin());

                while (!this->_comp(k, it->first))
                    ++it;
                return (it);
            }

            const_iterator  upper_bound(const key_type &k) const
            {
                const_iterator  it(this->begin());

                while (!this->_comp(k, it->first))
                    ++it;
                return (it);
            }


            ft::pair<iterator, iterator>    equal_range(const key_type &k)
            {
                return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
            }

            ft::pair<const_iterator, const_iterator>    equal_range(const key_type &k) const
            {
                return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
            }

            void    swap(map &x)
            {
                ft::swap(this->_tree, x._tree);
                ft::swap(this->_comp, x._comp);
                ft::swap(this->_size, x._size);
                ft::swap(this->_alloc, x._alloc);
            }

        private:
            RBTree<value_type, value_compare, Alloc> _tree;
            key_compare                             _comp;
            size_type                               _size;
            allocator_type                          _alloc;
    };

}

#endif // MAP_HPP
