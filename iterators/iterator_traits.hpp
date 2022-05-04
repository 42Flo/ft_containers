#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

namespace ft
{
	template < class Iterator >
	class iterator_traits
	{
		public:
			//typedef typename Iterator::difference_type	    difference_type;
			typedef typename Iterator::value_type	        value_type;
			typedef typename Iterator::pointer	            pointer;
			typedef typename Iterator::reference	        reference;
			//typedef typename Iterator::iterator_category    iterator_category;
	};
    //template < class T > class iterator_traits<T*>;
    //template < class T > class iterator_traits<const T*>;
}

#endif // ITERATOR_TRAITS_HPP
