#ifndef SET_HPP
# define SET_HPP

# include <iostream>
# include <memory>

# include "../tools/rb_tree.hpp"
# include "../tools/tools.hpp"

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

            /// Size / Capacity

            size_type   size() const {return (this->_size);}

            bool    empty() const {return (this->_size == 0);}

            size_type   max_size() const
            {
                return (this->_alloc.max_size);
            }

            //TODO define iterators

        private:
            size_type   _size;
    };
}

#endif // SET_HPP
