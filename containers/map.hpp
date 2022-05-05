#ifndef MAP_HPP
# define MAP_HPP

#include <iostream>
#include <memory>


# include "../tools/type_traits.hpp"
# include "../red_black_tree/rb_tree.hpp"
# include "../tools/tools.hpp"
# include "../iterators/reverse_iterator.hpp"
# include "../iterators/bidirectional_iterator.hpp"

namespace ft
{
    template < class Key, class T, class Compare = std::less<Key>,
        class Alloc = std::allocator<ft::pair<const Key, T> > >
    class map
    {
        public:
            typedef Key                     key_type;
            typedef T                       mapped_type;
            typedef ft::pair<const Key, T>  value_type;
            typedef size_t                  size_type;
            typedef Alloc                   allocator_type;

            typedef Compare key_compare;

            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;

            class value_compare : public std::binary_function<value_type, value_type, bool>
            {
                friend class map;
                friend class RedBlackTree<value_type, key_compare, value_compare, Alloc>;
                friend class bidirectional_iterator<value_type, false, key_compare, value_compare>;
                friend class bidirectional_iterator<value_type, true, key_compare, value_compare>;

                public:
                    bool operator()(const value_type &x, const value_type &y) const
                    {
                        return (comp(x.first, y.first));
                    }

                protected:
                    Compare comp;

                    value_compare(Compare Comp)
                    {
                        comp = Comp;
                    }
            };

            typedef typename ft::bidirectional_iterator<value_type, false, key_compare, value_compare>  iterator;
            typedef typename ft::bidirectional_iterator<value_type, true, key_compare, value_compare>   const_iterator;
            typedef ft::reverse_iterator<iterator>          reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;

            allocator_type get_allocator() const{ return (_alloc);}

            /// Constructors

            // default
            explicit map(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _alloc(alloc), _tree(), _size(0), _comp(comp){}

            // copy
            map(const map &x) : _alloc(x._alloc), _tree(x._tree),
                _size(x._size),_comp(x._comp){}

            // in range
            template<class InputIterator>
            map(InputIterator first, InputIterator last,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _alloc(alloc), _size(0), _comp(comp)
            {
                insert(first, last);
            }

            /// Destructor

            ~map()
            {
                this->clear();
            }

            /// Assignation

            map &operator=(const map &x)
            {
                if (this != &x)
                {
                    clear();
                    _tree = x._tree;
                    _size = x._size;
                    _alloc = x._alloc;
                    _comp = x._comp;
                }
                return (*this);
            }

            /// Element access

/*          // c++11
            T &at(const Key &key)
            {
                iterator tmp = find(key);

                if (tmp == end())
                    throw (std::out_of_range("Key not found"));
                return (tmp->second);
            }

            const T &at(const Key &key) const
            {
                const_iterator tmp = find(key);

                if (tmp == end())
                    throw (std::out_of_range("Key not found"));
                return (tmp->second);
            }
*/

            T &operator[](const Key &key)
            {
                value_type tmp = ft::make_pair(key, mapped_type());
                Node<value_type> *found = _tree.searchByData(tmp);

                if (_size == 0 || found->data->first != key)
                {
                    found = _tree.insert(tmp).first;
                    ++_size;
                }
                return (found->data->second);
            }

            /// Iterators

            iterator    begin(){ return (_tree.getLowest());}

            const_iterator  begin() const{ return (_tree.getLowest());}

            reverse_iterator    rbegin(){ return (reverse_iterator(end()));}

            const_reverse_iterator  rbegin() const
            {
                return (const_reverse_iterator(end()));
            }

            iterator    end(){ return (_tree.getLeaf());}

            const_iterator  end() const{ return (_tree.getLeaf());}

            reverse_iterator    rend(){ return (reverse_iterator(begin()));}

            const_reverse_iterator  rend() const
            {
                return (const_reverse_iterator(begin()));
            }

            /// Capacity

            size_type size() const{ return (_size);}

            bool empty() const{ return (_size == 0);}

            size_type max_size() const{ return (_alloc.max_size());}

            /// Modifiers

            void clear()
            {
                _tree.clear();
                _size = 0;
            }

            // single element
            ft::pair<iterator, bool> insert(const value_type &value)
            {
                ft::pair<iterator, bool> ret = _tree.insert(value);

                if (ret.second == true)
                    ++_size;
                return (ret);
            }

            // with hint
            iterator insert(iterator hint, const value_type &val)
            {
                if (hint != begin() && hint != end())
                {
                    if (hint == --lower_bound(val.first))
                    {
                        ft::pair<iterator, bool> ret = _tree.insert(val, hint.getNode());
                        if (ret.second == true)
                            ++_size;
                        else
                            return (end());
                    }
                }
                return (insert(val).first);
            }

            // in range
            template < class InputIterator >
            void insert(InputIterator first, InputIterator last)
            {
                while (first != last)
                {
                    insert(*first);
                    first++;
                }
            }

            // single element by iterator
            void erase(iterator position)
            {
                erase(position->first);
            }

            // single element by key
            size_type erase(const Key &key)
            {
                value_type tmp = ft::make_pair(key, mapped_type());

                if (_tree.erase(tmp) == true)
                {
                    --_size;
                    return (1);
                }
                return (0);
            }

            // in range
            void erase(iterator first, iterator last)
            {
                key_type arr[_size];
                int i = 0, j = 0;

                while (first != last)
                {
                    arr[i] = first->first;
                    ++first;
                    i++;
                }
                while (j < i)
                {
                    erase(arr[j]);
                    j++;
                }
            }

            void swap(map &x)
            {
                _tree.swap(x._tree);
                ft::swap(_alloc, x._alloc);
                ft::swap(_size, x._size);
                ft::swap(_comp, x._comp);
            }

            /// Lookup
            
            size_type count(const Key &key) const
            {
                if (find(key) != end())
                    return (1);
                return (0);
            }

            iterator find(const Key &key)
            {
                value_type toFind = ft::make_pair(key, mapped_type());
                Node<value_type> *found = _tree.searchByData(toFind);

                if (found->data->first != key)
                    return (end());
                return (found);
            }

            const_iterator find(const Key &key) const
            {
                value_type toFind = ft::make_pair(key, mapped_type());
                Node<value_type> *found = _tree.searchByData(toFind);

                if (found->data->first != key)
                    return (end());
                return (found);
            }

            ft::pair<iterator, iterator> equal_range(const Key &key)
            {
                return (ft::make_pair(lower_bound(key), upper_bound(key)));
            }

            ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const
            {
                return (ft::make_pair(lower_bound(key), upper_bound(key)));
            }

            iterator lower_bound(const key_type &key)
            {
                iterator it(begin());

                while (it != end() && _comp(it->first, key))
                    ++it;
                return (it);
            }

            const_iterator lower_bound(const key_type &key) const
            {
                const_iterator it(begin());

                while (it != end() && _comp(it->first, key))
                    ++it;
                return (it);
            }

            iterator upper_bound(const key_type &key)
            {
                iterator it(begin());

                while (it != end() && !_comp(key, it->first))
                    ++it;
                return (it);
            }

            const_iterator upper_bound(const key_type &key) const
            {
                const_iterator it(begin());

                while (it != end() && !_comp(key, it->first))
                    ++it;
                return (it);
            }

            /// Observers

            key_compare key_comp() const{ return (_comp);}

            value_compare value_comp() const{ return (_comp);}

        private:
            allocator_type _alloc;
            RedBlackTree<value_type, key_compare, value_compare, Alloc> _tree;
            size_type _size;
            key_compare _comp;

            /// Relational operators

            friend bool operator==(const map &l, const map &r)
            {
                if (l.size() == r.size())
                {
                    const_iterator itr = r.begin();
                    for (const_iterator it = l.begin() ; it != l.end() ; ++it)
                    {
                        if (it != itr)
                            return (false);
                        ++itr;
                    }
                    return (true);
                }
                return (false);
            }

            friend bool operator!=(const map &l, const map &r)
            {
                return (!(l == r));
            }

            friend bool operator<(const map &l, const map &r)
            {
                return (ft::lexicographical_compare(l.begin(), l.end(), r.begin(),
                            r.end()));
            }

            friend bool operator<=(const map &l, const map &r)
            {
                return (!(r < l));
            }

            friend bool operator>(const map &l, const map &r)
            {
                return (r < l);
            }

            friend bool operator>=(const map &l, const map &r)
            {
                return (!(l < r));
            }
    };

    // non-member swap
    template<class Key, class T, class Compare, class Alloc>
    void swap(ft::map<Key, T, Compare, Alloc> &x, ft::map<Key, T, Compare, Alloc> &y)
    {
        x.swap(y);
    }
}

#endif // MAP_HPP
