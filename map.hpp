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
            typedef std::pair<const key_type, mapped_type>      value_type;
            typedef Compare                                     key_compare;
            typedef Alloc                                       allocator_type;
            typedef typename allocator_type::reference          reference;
            typedef typename allocator_type::const_reference    const_reference;
            typedef typename allocator_type::pointer            pointer;
            typedef typename allocator_type::const_pointer      const_pointer;
            //TODO iterators
            typedef size_t                                      size_type;
            typedef ptrdiff_t                                   difference_type;

            //using node_alloc = typename std::allocator_traits<Alloc>::template
            //    rebind_alloc<Node>;

        private:
            RBTree<T, Alloc>    _map;
            key_compare         _comp;
            size_type           _size;
            allocator_type      _alloc;
            //node_alloc      _node_alloc;

        public:
            //typedef typename std::allocator_traits<Alloc>::template
            //    rebind_alloc<Node> node_alloc;

            // Default constructor
            explicit map(const key_compare &comp = key_compare(),
                const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc)
            {
                //TODO
            }

            // Range constructor
            template < class InputIterator >
            map(InputIterator first, InputIterator last,
                const key_compare &comp = key_compare(),
                const allocator_type &alloc = allocator_type())
                : _comp(comp), _alloc(alloc)
            {
                //TODO
            }

            // Copy constructor
            map(const map &x)
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

            mapped_type &operator[](const key_type &k)
            {
                //TODO
            }

            // Observers
            key_compare key_comp() const { return (this->_comp);}

            class value_compare : std::binary_function<value_type, value_type, bool>
            {
                private:
                    Compare comp;
                    value_compare(Compare c) : comp(c){}

                public:
                    bool    operator()(const value_type &x, const value_type &y) const
                    {
                        return (comp(x.first, y.first));
                    }
            };

            // Allocator
            allocator_type  get_allocator() const { return (this->_alloc);}

        private:
    };
}

#endif
