#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>
# include <stdexcept>
# include <cstddef>

# include "reverse_iterator.hpp"
# include "type_traits.hpp"
# include "tools.hpp"

namespace ft
{
    template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		private:
			class random_access_iterator
			{
				public:
					typedef T	            value_type;
					typedef std::ptrdiff_t	difference_type;
					typedef T&	            reference;
					typedef T*	            pointer;

                    // Default constructor
					random_access_iterator(pointer val = 0) : _current(val){}

                    // Copy constructor
					random_access_iterator(random_access_iterator const &src)
						: _current(src.getCurrent()){}

                    // Destructor
					~random_access_iterator(){}

					pointer	getCurrent() const {return (this->_current);}

					// Assignation
					random_access_iterator	operator=(random_access_iterator &r)
					{
						if (this != &r)
							this->_current = r.getCurrent();
						return (*this);
					}

					// Referencing
					reference	operator*(){ return (*(this->_current));}
					pointer		operator->(){ return (this->_current);}//TODO test that
					reference	operator[](int r)
					{
						return (*(this->_current + r));
					}

					// Increment / Decrease
					random_access_iterator	&operator++()
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
				    random_access_iterator  operator+(int r) const
					{
						return (this->_current + r);
					}
					random_access_iterator	operator-(int r) const
					{
						return (this->_current - r);
					}
					difference_type operator-(random_access_iterator const &r)
					{
                        return (this->_current - r.getCurrent());
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
			typedef T	                                        value_type;
			typedef Alloc	                                    allocator_type;
			typedef typename allocator_type::reference	        reference;
			typedef typename allocator_type::const_reference    const_reference;
			typedef typename allocator_type::pointer	        pointer;
			typedef typename allocator_type::const_pointer	    const_pointer;
			typedef random_access_iterator	                    iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            //TODO const iterators
			typedef size_t                                      size_type;
            typedef ptrdiff_t                                   difference_type;

			// Default constructor
			explicit vector(const allocator_type &alloc = allocator_type())
				: _size(0), _capacity(0), _alloc(alloc){}

			// Copy constructor
			vector(const vector &x)
				: _size(0), _capacity(0), _alloc(x.get_allocator())
			{
                this->insert(this->begin(), x.begin(), x.end());
			}

			// Fill constructor
			explicit vector(size_type n, const value_type &val = value_type()
				, const allocator_type &alloc = allocator_type())
				: _size(0), _capacity(0), _alloc(alloc)
			{
                this->insert(this->begin(), n, val);
			}

			// Range constructor
			template < class InputIterator >
			vector(InputIterator first, InputIterator last,
                const allocator_type &alloc = allocator_type(),
                typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                InputIterator>::type = 0)
                : _size(0), _capacity(0), _alloc(alloc)
			{
                this->insert(this->begin(), first, last);
			}

            //TODO destructor

			// Assignation operator
			ft::vector<T, Alloc>	operator=(const ft::vector<T, Alloc> &x)
			{
                if (&x != this)
                {
                    this->_size = x.size();
                    this->_capacity = x.capacity();
                    this->_alloc = x.get_allocator();
                    this->_vector = this->_alloc.allocate(this->_capacity);
                    for (unsigned int i = 0 ; i < this->_size ; ++i)//TODO use iterator
                        this->_alloc.construct(&(this->_vector[i]), x[i]);
                }
			}

            // Iterators
            iterator    begin()
            { 
                return (iterator(&(this->_vector[0])));
            }

            reverse_iterator    rbegin()
            {
                return (reverse_iterator(&(this->_vector[this->_size])));
            }//TODO FIX REVERSE ITERATORS

            iterator    end()
            { 
                return (iterator(&(this->_vector[this->_size])));
            }

            reverse_iterator    rend()
            {
                return (reverse_iterator(&(this->_vectr[0])));
            }

			// Capacity / size
			size_type	size() const { return (this->_size);}

			size_type	max_size() const 
			{
				return (this->_alloc.max_size());
			}

			void	resize(size_type n, value_type val = value_type())
			{
                if (n < this->_size)
                    for (unsigned int i = n ; i < this->_size ; ++i)
                        this->_alloc.destroy(this->_vector[i]);
                else
                {
                    if (this->_capacity < n)
                        this->reserve(n);
                    for (unsigned int i = this->_size ; i < n ; ++i)
                        this->_alloc.construct(&(this->_vector[i]), val);
                }
                this->_size = n;
			}

			size_type	capacity() const { return (this->_capacity);}
			
			bool    empty() const { return (this->_size == 0);}

            void    reserve(size_type n)
            {
                if (n > this->max_size())
                    throw (std::length_error("reserve: length too high"));
                if (n > this->_capacity)
                {
                    if (this->_capacity == 0)
                        this->_vector = this->_alloc.allocate(n);
                    else
                    {
                        pointer newVec = this->_alloc.allocate(n);

                        for (unsigned int i = 0 ; i < this->_size ; ++i)
                        {
                            this->_alloc.construct(&newVec[i], this->_vector[i]);
                            this->_alloc.destroy(&(this->_vector[i]));
                        }
                        this->_alloc.deallocate(this->_vector, this->_capacity);
                        this->_vector = newVec;
                    }
                    this->_capacity = n;
                }
            }

            // Modifiers
            void	push_back(const value_type &val)
            {
                this->reserve(this->_size + 1);
                this->_alloc.construct(&(this->_vector[this->_size]), val);
                ++this->_size;
            }

            void	pop_back()
            {
                this->_alloc.destroy(&(this->_vector[this->_size - 1]));
                --this->_size;
            }

            // insert(): single element
            iterator	insert(iterator position, const value_type &val)
            {
                this->insert(position, 1, val);
                return (position);
            }
            // insert(): fill
            void	insert(iterator position, size_type n, const value_type &val)
            {
                difference_type pos = position - this->begin();

                this->reserve(this->_size + n);
                this->_shiftRight(pos, n);
                for (unsigned int i = 0 ; i < n ; ++i)
                    this->_alloc.construct(&(this->_vector[pos++]), val);
                this->_size += n;
            }
            // insert(): fill in range
            template < class InputIterator >
            void	insert(iterator position, InputIterator first, InputIterator last,
                typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                InputIterator>::type = 0)
            {
                difference_type pos = position - this->begin();

                this->reserve(this->_size + (last - first));
                this->_shitftRight(pos, last - first);
                for ( ; first != last ; ++first)
                {
                    this->_alloc.construct(&(this->_vector[pos++]), *first);
                    ++this->_size;
                }
            }

            // erase(): single element
            iterator	erase(iterator position)
            {
                this->_alloc.destroy(&(*position));
                this->_shiftLeft(position - this->begin(), 1);//TODO check with iterator NULL
                --this->_size;
                return (position);
            }
            // erase(): in range
            iterator	erase(iterator first, iterator last)
            {
                difference_type n = last - first;

                for ( ; first != last ; ++first)
                    this->_alloc.destroy(&(*first));//TODO same here
                this->_shiftLeft(last - this->begin() - 1, n);
                this->_size -= n;
                return (last);
            }

            void	swap(vector &x)
            {
                ft::swap(this->_vector, x._vector);
                ft::swap(this->_size, x._size);
                ft::swap(this->_capacity, x._capacity);
                ft::swap(this->_alloc, x._alloc);
            }

            void	clear()
            {
                for (unsigned int i = 0 ; i < this->_size ; ++i)
                    this->_alloc.destroy(&(this->_vector[i]));//TODO use iterator
                this->_size = 0;
            }

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
                if (n < this->_size)
                    throw (std::out_of_range("at: out of range"));
                return (this->_vector[n]);
            }
            // const at()
            const_reference	at(size_type n) const
            {
                if (n < this->_size)
                    throw (std::out_of_range("at: out of range"));
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

            void    _shiftRight(difference_type pos, size_type n)
            {
                if ((size_type)pos < this->_size && this->_capacity >= this->_size + n)
                {
                    iterator    toWrite = this->end() + n - 1;
                    iterator    toDelete = this->end() - 1;

                    for (unsigned int i = 0 ; i < this->_size - pos ; ++i)
                    {
                        this->_alloc.construct(&(*toWrite--), *toDelete);
                        *toDelete = 0; //maybe not needed
                        this->_alloc.destroy(&(*toDelete--));
                    }
                }
            }

            void    _shiftLeft(difference_type pos, size_type n)
            {
                if ((size_type)pos < this->_size)
                {
                    iterator    toWrite = this->begin() + pos - n + 1;
                    iterator    toDelete = this->begin() + pos + 1;

                    for (unsigned int i = 0 ; i < this->_size - pos ; ++i)
                    {
                        this->_alloc.construct(&(*toWrite++), *toDelete);
                        *toDelete = 0; //same
                        this->_alloc.destroy(&(*toDelete++));
                    }
                }
            }

            pointer _vector;
            size_type	_size;
            size_type	_capacity;
            allocator_type  _alloc;

            // Relational operators
            friend bool operator==(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                if (l.size() == r.size())
                {
                    for (unsigned int i = 0 ; i < l.size() ; ++i)
                        if (r[i] != l[i])
                            return (false);
                    return (true);
                }
                return (false);
            }

            friend bool operator!=(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                return (!(l == r));
            }

            friend bool operator<(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                return (lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()));
            }

            friend bool operator<=(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                return (!(r < l));
            }

            friend bool operator>(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                return (r < l);
            }

            friend bool operator>=(const vector<T, Alloc> &l,
                const vector<T, Alloc> &r)
            {
                return (!(l < r));
            }

            //TODO non-member swap
    };
}

#endif // VECTOR_HPP
