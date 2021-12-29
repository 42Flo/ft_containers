#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>

# include "reverse_iterator.hpp"

namespace ft
{
    template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		private:
			// ********** Random access iterator **********

			class random_access_iterator
			{
				public:
					typedef T	value_type;
					typedef std::ptrdiff_t	difference_type;
					typedef T&	reference;
					typedef T*	pointer;

					random_access_iterator(pointer val = 0) : _current(val){}
					random_access_iterator(random_access_iterator const &src)
						: _current(src.getCurrent()){}
					~random_access_iterator(){}

					pointer	getCurrent() const {return (this->_current);}

					// ********** Operator overloads **********

					// Assignation
					random_access_iterator	operator=(random_access_iterator &r)
					{
						if (this != &r)
							this->_current = r.getCurrent();
						return (*this);
					}

					// Referencing
					reference	operator*(){ return (*(this->_current));}
					pointer		operator->(){ return (this->_current);}
					reference	operator[](int r)
					{
						return (*(this->_current + r));
					}

					// Increment / Decrease
					random_access_iterator	&operator++() //TODO try with ++(*this)
					{
						++(this->_current);
						return (*this);
					}
					random_access_iterator	operator++(int)
					{
						random_access_iterator	tmp(*this);

						++(this->_current);
						return (tmp);
					}
					random_access_iterator	&operator--()
					{
						--(this->_current);
						return (*this);
					}
					random_access_iterator	operator--(int)
					{
						random_access_iterator	tmp(*this);

						--(this->_current);
						return (tmp);
					}

					// Arithmetic
					random_access_iterator	operator+(int r) const //TODO test a + n AND n + a
					{
						return (this->_current + r);
					}
					random_access_iterator	operator-(int r) const
					{
						return (this->_current - r);
					}
					random_access_iterator	operator-(random_access_iterator &r)
					{
						//TODO substracting an iterator from another
					}

					// Compound assignment
					random_access_iterator	operator+=(int r)
					{
						this->_current += r;
						return (*this);
					}
					random_access_iterator	operator-=(int r)
					{
						this->_current -= r;
						return (*this);
					}

				private:
					pointer	_current;

					// ********** Non-member opeator overloads **********

					// Relational operators
					friend bool	operator==(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() == r.getCurrent());
					}
					friend bool	operator!=(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() != r.getCurrent());
					}
					friend bool	operator<(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() < r.getCurrent());
					}
					friend bool	operator<=(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() <= r.getCurrent());
					}
					friend bool	operator>(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() > r.getCurrent());
					}
					friend bool	operator>=(const random_access_iterator &l,
							const random_access_iterator &r)
					{
						return (l.getCurrent() >= r.getCurrent());
					}
			};

		public:
			typedef T	value_type;
			typedef Alloc	allocator_type;
			typedef typename allocator_type::reference	reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer	pointer;
			typedef typename allocator_type::const_pointer	const_pointer;
			typedef random_access_iterator	iterator;
            typedef ft::reverse_iterator<iterator>    reverse_iterator;
            //TODO const iterators

			typedef size_t	size_type;

			//TODO iterator typedef and implementation
			
			// Constructors

			// Default constructor
			explicit vector(const allocator_type &alloc = allocator_type())
				: _size(0), _capacity(0), _alloc(alloc){}

			// Copy constructor
			vector(const ft::vector<T, Alloc> &x)
				: _size(x.size()), _capacity(x.capacity), _alloc(x.get_allocator)
			{
				this->_vector = this->_alloc.allocate(this->_capacity);
			}

			// Fill constructor
			explicit vector(size_type n, const value_type &val = value_type()
					, const allocator_type &alloc = allocator_type())
				: _size(n), _capacity(n), _alloc(alloc)
			{
				this->_vector = this->_alloc.allocate(this->_capacity);
				for (unsigned int i = 0 ; i < this->_capacity ; ++i)
					this->_vector[i] = val; //This may not work
			}

			// Range constructor
			/*template < class InputIterator >
			vector(InputIterator first, InputIterator last
					, const allocator_type &alloc = allocator_type())
			{
                
			}*/

			// Assignation operator
			ft::vector<T, Alloc>	operator=(const ft::vector<T, Alloc> &x)
			{
                if (&x != this)
                {
                    this->_size = x.size();
                    this->_capacity = x.capacity();
                    this->_alloc = x.get_allocator();
                    this->_vector = this->_alloc.allocate(this->_capacity);
                    for (unsigned int i ; i < this->_size ; ++i)
                        this->_vector[i] = x[i];
                }
			}
		
            // Iterators
            iterator    begin()
            { 
                return (iterator(&(this->_vector[0])));
            }
            //TODO const begin
            reverse_iterator    rbegin()
            {
                return (reverse_iterator(&(this->_vector[this->_size])));
            }
            iterator    end()
            { 
                return (iterator(&(this->_vector[this->_size])));
            }
            //TODO const last
            reverse_iterator    rend()
            {
                return (reverse_iterator(&(this->_vectr[0])));
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
					this->_vector[this->_size - 1] = val;
                else
                {
                    reallocVector();
                    this->_vector[this->_size] = val;
                }
				this->_size++;
			}
			void	pop_back()
			{
				this->_vector[this->_size] = 0;
				this->_size--;
			}

			// insert(): single element
			iterator	insert(iterator position, const value_type &val)
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

			// erase(): single element
			iterator	erase(iterator position)
			{

			}
			// erase(): in range
			iterator	erase(iterator first, iterator last)
			{

			}

			void	swap(ft::vector<T, Alloc>)
			{

			}

			void	clear()
			{
				for (value_type i ; i < this->_size ; ++i)
					this->_vector[i] = 0;
				this->_size = 0;
			}


			// Element access

			// operator []
			reference	operator[](size_type n)
			{ 
				return (this->_vector[n]);
			}
			// const operator []
			const_reference	operator[](size_type n) const
			{
				return (this->_vector[n]);
			}

			// at()
			reference	at(size_type n)
			{
				//TODO throw out_of_range exception if n > size
				return (this->_vector[n]);
			}
			// const at()
			const_reference	at(size_type n) const
			{
				//TODO throw out_of_range exception if n > size
				return (this->_vector[n]);
			}

			// front()
			reference	front()
			{
				return (this->_vector[0]);
			}
			// const front()
			const_reference	front() const
			{
				return (this->_vector[0]);
			}

			// back()
			reference	back()
			{
				return (this->_vector[this->_size - 1]);
			}
			// const back()
			const_reference	back() const
			{
				return (this->_vector[this->_size - 1]);
			}


			allocator_type	get_allocator() const {return (this->_alloc);}

		private:

            void    reallocVector()
            {
                pointer newVector = this->_alloc.allocate(this->_size + 1);

                for (unsigned int i = 0 ; i < this->_size ; ++i)
                {
                    this->_alloc.construct(&newVector[i], this->_vector[i]);
                    this->_alloc.destroy(&this->_vector[i]);
                }
                this->_alloc.deallocate(this->_vector, this->_capacity);
                this->_vector = newVector;
                ++this->_capacity;
            }

			value_type	*_vector;
			size_type	_size;
			size_type	_capacity;
			allocator_type  _alloc;
	};
}

#endif // VECTOR_HPP
