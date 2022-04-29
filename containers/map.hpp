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
            typedef std::ptrdiff_t   difference_type;

            class value_compare : public std::binary_function<value_type, value_type, bool>
            {
                friend class map;
                friend class RBTree<value_type, key_compare, value_compare, Alloc>;
                friend class bidirectional_iterator<value_type, false, key_compare, value_compare>;
                friend class bidirectional_iterator<value_type, true, key_compare, value_compare>;

                protected:
                    Compare comp;
                    value_compare(Compare c) : comp(c){}

                public:
                    bool    operator()(const value_type &x, const value_type &y) const
                    {
                        return (comp(x.first, y.first));
                    }
            };

            typedef typename
                RBTree<value_type, key_compare, value_compare, Alloc>::iterator iterator;
            typedef typename
                RBTree<value_type, key_compare, value_compare, Alloc>::const_iterator const_iterator;
            typedef ft::reverse_iterator<iterator>  reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;

            /// Constructors

            // default
            explicit map(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _tree(), _comp(comp), _size(0), _alloc(alloc)
            {
            }

            // copy
            map(const map &x) : _comp(x._comp), _size(0), _alloc(x._alloc)
            {
                this->insert(x.begin(), x.end());
            }

            // fill in range
            template < class InputIterator >
            map(InputIterator first, InputIterator last,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _comp(comp), _size(0), _alloc(alloc)
            {
                this->insert(first, last);
            }

            /// Destructor

            ~map()
            {
                this->clear();
            }

            /// Assignation operator

            map &operator=(const map &x)
            {
                if (this != &x)
                {
                    this->clear();
                    this->_comp = x._comp;
                    this->_alloc = x._alloc;
                    this->insert(x.begin(), x.end());
                }
                return (*this);
            }

            /// Iterators

            iterator    begin()
            {
                return (iterator(this->_tree.getLowest()));
            }

            const_iterator  begin() const
            {
                return (const_iterator(this->_tree.getLowest()));
            }

            iterator    end()
            {
                if (this->_size == 0)
                    return (this->begin());
                return (iterator(this->_tree.getHighest()->right));
            }

            const_iterator  end() const
            {
                if (this->_size == 0)
                    return (this->begin());
                return (const_iterator(this->_tree.getHighest()->right));
            }

            reverse_iterator    rbegin()
            {
                return (reverse_iterator(this->end()));
            }

            reverse_iterator    rend()
            {
                return (reverse_iterator(this->begin()));
            }

            /// Size / Capacity

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
                return (this->insert(ft::make_pair(k, mapped_type())).first->second);
                //return (this->insert(ft::make_pair(k, mapped_type())).second);
            }

            /// Modifiers

            // insert(): single element
            ft::pair<iterator, bool>    insert(const value_type &val)
            {
                iterator    it = this->find(val.first);

                if (it != this->end())
                    return (ft::make_pair(it, false));
                ++this->_size;
                return (ft::make_pair(iterator(this->_tree.insert(val)), true));
            }

            // insert(): with hint
            iterator    insert(iterator position, const value_type &val)
            {
                iterator    it = this->find(val.first);

                if (it != this->end())
                    return (it);
                ++this->_size;
                return (this->_tree.insert(val, position));
            }

            // insert(): in range
            template < class InputIterator >
            void    insert(InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                    InputIterator>::type * = 0)
            {
                for ( ; first != last ; ++first)
                {
                    std::cout << "insert loop!" << std::endl;
                    this->insert(*first);
                }
            }

            // erase(): by iterator
            void    erase(iterator position)
            {
                if (position == this->end())
                    return;
                this->_tree.deleteNode(position.getNode());
                --this->_size;
            }

            // erase(): by key
            size_type   erase(const key_type &k)
            {
                iterator    it = this->find(k);

                if (it != NULL)
                {
                    this->_tree.deleteNode(it.getNode());
                    --this->_size;
                    return (1);
                }
                return (0);
            }

            // erase(): in range
            void    erase(iterator first, iterator last)
            {
                iterator    tmp;

                while (first != last)
                {
                    tmp = first;
                    ++first;
                    this->_tree.deleteNode(tmp.getNode());
                    --this->_size;
                }
            }

            void    swap(map &x)
            {
                ft::swap(this->_tree, x._tree);
                ft::swap(this->_comp, x._comp);
                ft::swap(this->_size, x._size);
                ft::swap(this->_alloc, x._alloc);
            }

            void    clear()
            {
                iterator    it = this->begin();
                iterator    ite = this->end();
                iterator    tmp;

                while (it != ite)
                {
                    tmp = it;
                    ++it;
                    this->_tree.deleteNode(tmp.getNode());
                    --this->_size;
                }
            }

            /// Observers

            key_compare key_comp() const {return (this->_comp);}

            value_compare   value_comp() const {return (value_compare(this->_comp));}

            /// Allocator

            allocator_type  get_allocator() const {return (this->_alloc);}

            /// Operations

            iterator        find(const key_type &k)
            {
                iterator    it = this->begin();

                while (it != this->end())
                {
                    std::cout << "find loop!" << std::endl;
                    std::cout << this->_tree.getHighest()->data->first << std::endl;
                    std::cout << ( it == this->_tree.getHighest()->right) << std::endl;
                    //std::cout << it->first << std::endl;
                    if (!this->_comp(it->first, k) && !this->_comp(k, it->first))
                        break;
                    ++it;
                }
                return (it);
            }

            const_iterator  find(const key_type &k) const
            {
                const_iterator  it = this->begin();

                while (it != this->end())
                {
                    if (!this->_comp(it->first, k) && !this->_comp(k, it->first))
                        break;
                    ++it;
                }
                return (it);
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

                while (it != this->end() && this->_comp(it->first, k))
                    ++it;
                return (it);
            }

            const_iterator  lower_bound(const key_type &k) const
            {
                const_iterator  it(this->begin());

                while (it != this->end() && this->_comp(it->first, k))
                    ++it;
                return (it);
            }

            iterator    upper_bound(const key_type &k)
            {
                return (this->lower_bound(k) == this->end() ? this->end() : ++this->lower_bound(k));
            }

            const_iterator  upper_bound(const key_type &k) const
            {
                return (this->lower_bound(k) == this->end() ? this->end() : ++this->lower_bound(k));
                //return (++this->lower_bound(k));
            }

            ft::pair<iterator, iterator>    equal_range(const key_type &k)
            {
                return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
            }

            ft::pair<const_iterator, const_iterator>    equal_range(const key_type &k) const
            {
                return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
            }

        private:

            RBTree<value_type, key_compare, value_compare, Alloc>  _tree;
            key_compare _comp;
            size_type   _size;
            allocator_type  _alloc;
            Node<value_type>    *_end_node;

            friend bool operator==(const map &l, const map &r)
            {
                if (l.size() == r.size())
                {
                    const_iterator    itr = r.begin();
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
                return (ft::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end(), l.value_comp()));
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

    template < class Key, class T, class Compare, class Alloc >
    void    swap(ft::map<Key, T, Compare, Alloc> &x,
            ft::map<Key, T, Compare, Alloc> &y)
    {
        x.swap(y);
    }
}

#endif // MAP_HPP
