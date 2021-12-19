#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>

namespace ft
{
	template <class T, class Alloc = std::allocator<T>>
	class vector
	{
		public:

			// Member types

			typedef T	value_type;
			typedef Alloc	allocator_type;
			typedef typename allocator_type::reference	reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer	pointer;
			typedef typename allocator_type::const_pointer	const_pointer;
			typedef size_t	size_type;

			
			// Constructors

			// Default constructor
			explicit vector(const allocator_type &alloc = allocator_type())
			{

			}

			//Copy constructor
			vector(const vector &x)
			{

			}

			// Fill constructor
			explicit vector(size_type n, const value_type &val = value_type()
					, const allocator_type &alloc = allocator_type())
			{

			}

			// Range constructor
			template < class InputIterator >
			vector(InputIterator first, InputIterator last
					, const allocator_type &alloc = allocator_type())
			{

			}
		
			size_type	size() const { return (this->_size);}
			size_type	max_size() const 
			{
				return ((((size_type) - 1) / sizeof(T)) / 2);
			}
			//TODO resize()
			size_type	capacity() const { return (this->_capacity);}
			//TODO empty()
			//TODO reserve()

		private:
			size_type	_size;
			size_type	_capacity;
			allocator_type	_alloc;
	};
}

#endif // VECTOR_HPP
