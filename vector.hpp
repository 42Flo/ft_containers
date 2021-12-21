#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>

namespace ft
{
	template < class T, class Alloc = std::allocator<T> >
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

			//TODO iterator typedef and implementation
			
			// Constructors

			// Default constructor
			explicit vector(const allocator_type &alloc = allocator_type())
				: _size(0), _capacity(0), _alloc(alloc){}

			// Copy constructor
			vector(const ft::vector &x)
				: _size(x.size()), _capacity(x.capacity), _alloc(x.get_allocator)
			{
				//TODO find a way to deeply copy vector pointer
			}

			// Fill constructor
			explicit vector(size_type n, const value_type &val = value_type()
					, const allocator_type &alloc = allocator_type())
				: _size(n), _capacity(n)
			{
				this->_vector = this->_alloc.allocate(this->_capacity);
				for (unsigned int i = 0 ; i < this->_capacity ; ++i)
					this->_vector[i] = val; //This may not work
			}

			// Range constructor
			template < class InputIterator >
			vector(InputIterator first, InputIterator last
					, const allocator_type &alloc = allocator_type())
			{

			}
		

			// Capacity / size

			size_type	size() const { return (this->_size);}
			
			size_type	max_size() const 
			{
				//return ((((size_type) - 1) / sizeof(T)) / 2);
				return (this->_alloc.max_size());
			}
			
			void	resize(size_type n , value_type val = value_type())
			{

			}

			size_type	capacity() const { return (this->_capacity);}
			
			//TODO empty()
			
			//TODO reserve()


			// Modifiers

			void	push_back(const value_type &val)
			{
				if (this->_size < this->_capacity)
					this->_vector[this->size] = val;
				//TODO else realloc : find the best way to realloc
				this->_size++;
			}

			void	pop_back()
			{
				this->_vector[this->size] = 0;
				this->_size--;
			}

			// insert(): single element
			iterator	insert(iterator position, const_value &val)
			{

			}
			// insert(): fill
			void	insert(iterator position, size_type n, const value_type &val)
			{

			}
			// insert(): fill in range
			template < class InputIterator >
			void	insert(iterator position, InputIterator first, InputIterator last)
			{

			}

			//erase(): single element
			iterator	erase(iterator position)
			{

			}
			//erase(): in range
			iterator	erase(iterator first, iterator last)
			{

			}


			allocator_type	get_allocator() const {return (this->_alloc);}

		private:
			value_type	*_vector;
			size_type	_size;
			size_type	_capacity;
			allocator_type const	&_alloc;
	};
}

#endif // VECTOR_HPP
