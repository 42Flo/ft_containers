#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

# include <cstddef>
# include "../iterators/random_access_iterator.hpp"

namespace ft
{
    // traits for iterator
	template < class Iterator >
	struct iterator_traits
	{
        typedef typename Iterator::difference_type	    difference_type;
        typedef typename Iterator::value_type	        value_type;
        typedef typename Iterator::pointer	            pointer;
        typedef typename Iterator::reference	        reference;
        typedef typename Iterator::iterator_category    iterator_category;
    };

    // traits for T = value_type
    template < class T >
    struct iterator_traits<T*>
    {
        typedef ft::random_access_iterator<T, false>  iterator_category;
        typedef std::ptrdiff_t  difference_type;
        typedef T               value_type;
        typedef T*              pointer;
        typedef T&              reference;
    };

    // traits for T = const value_type
    template < class T>
    struct iterator_traits<const T*>
    {
        typedef ft::random_access_iterator<T, true>  iterator_category;
        typedef std::ptrdiff_t  difference_type;
        typedef T               value_type;
        typedef const T*        pointer;
        typedef const T&        reference;
    };
}

#endif // ITERATOR_TRAITS_HPP
