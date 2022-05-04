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
            typedef ptrdiff_t   difference_type;


		    typedef typename ft::bidirectional_iterator<value_type, false, key_compare, value_compare> iterator;
    		typedef typename ft::bidirectional_iterator<value_type, true, key_compare, value_compare> const_iterator;
    		typedef ft::reverse_iterator<iterator> reverse_iterator;
    		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

            /// Constructors

            // default
            explicit set(const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _tree(), _comp(comp), _size(0), _alloc(alloc){}

            // copy
            set(const set &x) : _alloc(x._alloc), _tree(x._tree), _size(x._size)
            {
                //TODO test copy
            }

            // fill in range
            template < class InputIterator >
            set(InputIterator first, InputIterator last,
                    const key_compare &comp = key_compare(),
                    const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc), _size(0)
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
                return (iterator(NULL));
            }

            const_iterator  end() const
            {
                return (const_iterator(NULL));
            }

            reverse_iterator    rbegin()
            {
                return (reverse_iterator(this->begin()));
            }

            reverse_iterator    rend()
            {
                return (reverse_iterator(this->end()));
            }

            /// Size / Capacity

            size_type   size() const {return (this->_size);}

            bool    empty() const {return (this->_size == 0);}

            size_type   max_size() const
            {
                return (this->_alloc.max_size);
            }

            /// Modifiers

            // insert(): single element
            ft::pair<iterator, bool>    insert(const value_type &val)
            {
                iterator    it = this->find(val);

                if (it.getNode() != NULL && it != this->end())
                    return (ft::make_pair(it, false));
                ++this->_size;
                return (ft::make_pair(iterator(this->_tree.insert(val)), true));
            }

            // insert(): with hint
            iterator    insert(iterator position, const value_type &val)
            {
                if (iterator it = this->find(val->first) != this->end())
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
                    this->insert(*first);
            }

            // erase(): by iterator
            void    erase(iterator position)
            {
                this->_tree.deleteNode(position.getNode());
            }

            // erase(): by value
            size_type   erase(const value_type &val)
            {
                iterator    it = this->find(val);

                if (it != NULL)
                {
                    this->_tree.deleteNode(it.getNode());
                    --this->_size;
                    return (1);
                }
                return (0);
            }

            void    swap(set &x)
            {
                ft::swap(this->_tree, x._tree);
                ft::swap(this->_comp, x._comp);
                ft::swap(this->_size, x._size);
                ft::swap(this->_alloc, x._alloc);
            }

            void    clear()
            {
                iterator    it = this->begin();
                iterator    tmp;

                while (it != this->end())
                {
                    tmp = it;
                    ++it;
                    this->_tree.deleteNode(tmp.getNode());
                }
                this->_size = 0;
            }

            /// Observers
            
            key_compare key_comp() const {return (this->_comp);}

            value_compare   value_comp() const {return (this->_comp);}

            /// Allocator

            allocator_type  get_allocator() const {return (this->_alloc);}

            /// Operations

            iterator    find(const value_type &val) const
            {
                iterator    it = this->begin();

                while (it != this->end())
                {
                    if (!this->_comp(*it, val) && !this->_comp(val, *it))
                        break;
                    ++it;
                } 
                return (it);
            }

            iterator    lower_bound(const value_type &val)
            {
                iterator    it(this->begin());

                while (this->_comp(*it, val))
                    ++it;
                return (it);
            }

            iterator    upper_bound(const value_type &val)
            {
                iterator    it(this->begin());

                while (!this->_comp(val, *it))
                    ++it;
                return (it);
            }

            ft::pair<iterator, iterator>    equal_range(const value_type &val)
            {
                return (ft::make_pair<iterator, iterator>(lower_bound(val), upper_bound(val)));
            }

        private:
            RBTree<value_type, key_compare, value_compare, Alloc>  _tree;
            key_compare _comp;
            size_type   _size;
            allocator_type  _alloc;
    };
}

#endif // SET_HPP
