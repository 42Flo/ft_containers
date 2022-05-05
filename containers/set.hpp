#ifndef SET_HPP
# define SET_HPP

# include <iostream>
# include <memory>

# include "../tools/rb_tree.hpp"
# include "../tools/tools.hpp"
# include "../iterators/reverse_iterator.hpp"
# include "../iterators/bidirectional_iterator.hpp"

namespace ft
{
    template < class T, class Compare = std::less<T>,
             class Alloc = std::allocator<T> >
    class set
    {
        public:
            typedef T   key_type;
            typedef T   value_type;
            typedef Compare key_compare;
            typedef Compare value_compare;
            typedef Alloc   allocator_type;
            typedef typename allocator_type::reference  reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer    pointer;
            typedef typename allocator_type::const_pointer  const_pointer;
            typedef size_t  size_type;
            //typedef ptrdiff_t   difference_type;


		    typedef typename ft::bidirectional_iterator<value_type, true, key_compare, value_compare>  iterator;
    		typedef typename ft::bidirectional_iterator<value_type, true, key_compare, value_compare>   const_iterator;
    		typedef ft::reverse_iterator<iterator> reverse_iterator;
    		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

            allocator_type  get_allocator() const {return (_alloc);}

            /// Constructors

            // default
            explicit set(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _tree(), _comp(comp), _size(0), _alloc(alloc){}

            // copy
            set(const set &x)
                : _tree(x._tree), _comp(x._comp), _size(x._size), _alloc(x._alloc)
            {
                //TODO test copy
            }

            // fill in range
            template < class InputIterator >
            set(InputIterator first, InputIterator last,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _comp(comp), _size(0), _alloc(alloc)
            {
                insert(first, last);
            }

            /// Destructor

            ~set()
            {
                this->clear();
            }

            /// Assignation operator

            set &operator=(const set &x)
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

            /// Iterators

            iterator    begin(){ return (_tree.getLowest());}

            const_iterator  begin() const{ return (_tree.getLowest());}

            reverse_iterator    rbegin(){ return (reverse_iterator(end()));}

            const_reverse_iterator  rbegin() const
            {
                return (const_reverse_iterator(begin()));
            }

            iterator    end(){ return (_tree.getLeaf());}

            const_iterator  end() const { return (_tree.getLeaf());}

            reverse_iterator    rend(){ return (reverse_iterator(begin()));}

            const_reverse_iterator   rend() const
            {
                return (const_reverse_iterator(begin()));
            }

            /// Capacity

            size_type   size() const {return (_size);}

            bool    empty() const {return (_size == 0);}

            size_type   max_size() const{return (_alloc.max_size());}

            /// Modifiers

            void    clear()
            {
                _tree.clear();
                _size = 0;
            }

            // single element
            ft::pair<iterator, bool>    insert(const value_type &val)
            {
                ft::pair<iterator, bool> ret = _tree.insert(val);

                if (ret.second == true)
                    ++_size;
                return (ret);
            }

            // with hint
            iterator    insert(iterator hint, const value_type &val)
            {
                (void)hint;//TODO manage hint
                return (insert(val).first);
            }

            // in range
            template < class InputIterator >
            void    insert(InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                    InputIterator>::type * = 0)
            {
                while (first != last)
                {
                    insert(*first);
                    first++;
                }
            }

            // single element by iterator
            void    erase(iterator position)
            {
                erase(*position);
            }

            // single element by value
            size_type   erase(const value_type &val)
            {
                //value_type tmp = ft::make_pair(key, mapped_type());

                if (_tree.deleteByData(val) == true)
                {
                    --_size;
                    return (1);
                }
                return (0);
            }

            // in range
            void    erase(iterator first, iterator last)
            {
                value_type  arr[_size];
                int i = 0, j = 0;

                while (first != last)
                {
                    arr[i] = *first;
                    ++first;
                    i++;
                }
                while (j < i)
                {
                    erase(arr[j]);
                    j++;
                }
            }

            void    swap(set &x)
            {
                _tree.swap(x._tree);
                ft::swap(_comp, x._comp);
                ft::swap(_size, x._size);
                ft::swap(_alloc, x._alloc);
            }

            /// Observers
            
            key_compare key_comp() const {return (this->_comp);}

            value_compare   value_comp() const {return (this->_comp);}


            /// Lookup

            size_type   count(const value_type &val) const
            {
                if (find(val) != end())
                    return (1);
                return (0);
            }

            iterator    find(const value_type &val)
            {
                //value_type toFind = ft::make_pair(key, mapped_type());
                Node<value_type> *found = _tree.searchByData(val);

                if (*(found->data) != val)
                    return (end());
                return (iterator(found));
            }

            const_iterator    find(const value_type &val) const
            {
                //value_type toFind = ft::make_pair(key, mapped_type());
                Node<value_type> *found = _tree.searchByData(val);

                if (*(found->data) != val)
                    return (end());
                return (iterator(found));
            }


            ft::pair<iterator, iterator> equal_range(const value_type &val)
            {
                return (ft::make_pair(lower_bound(val), upper_bound(val)));
            }

            ft::pair<const_iterator, const_iterator> equal_range(const value_type &val) const
            {
                return (ft::make_pair(lower_bound(val), upper_bound(val)));
            }

            iterator    lower_bound(const value_type &val)
            {
                iterator    it(begin());

                while (it != end() && _comp(*it, val))
                    ++it;
                return (it);
            }

            const_iterator    lower_bound(const value_type &val) const
            {
                const_iterator    it(begin());

                while (it != end() && _comp(*it, val))
                    ++it;
                return (it);
            }

            iterator    upper_bound(const value_type &val)
            {
                iterator    it(begin());

                while (it != end() && !_comp(val, *it))
                    ++it;
                return (it);
            }

            const_iterator    upper_bound(const value_type &val) const
            {
                const_iterator    it(begin());

                while (it != end() && !_comp(val, *it))
                    ++it;
                return (it);
            }

        private:
            RedBlackTree<value_type, key_compare, value_compare, Alloc>  _tree;
            key_compare _comp;
            size_type   _size;
            allocator_type  _alloc;

            /// Relational operators

            friend bool operator==(const set &l, const set &r)
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

            friend bool operator!=(const set &l, const set &r)
            {
                return (!(l == r));
            }

            friend bool operator<(const set &l, const set &r)
            {
                return (ft::lexicographical_compare(l.begin(), l.end(), r.begin(),
                            r.end()));
            }

            friend bool operator<=(const set &l, const set &r)
            {
                return (!(r < l));
            }

            friend bool operator>(const set &l, const set &r)
            {
                return (r < l);
            }

            friend bool operator>=(const set &l, const set &r)
            {
                return (!(l < r));
            }
    };

    // non-member swap
    template < class Key, class Compare, class Alloc >
    void swap(ft::set<Key, Compare, Alloc> &x, ft::set<Key, Compare, Alloc> &y)
    {
        x.swap(y);
    }
}

#endif // SET_HPP
