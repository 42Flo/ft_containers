#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>
# include <stdexcept>
# include <cstddef>

# include "../iterators/reverse_iterator.hpp"
# include "../tools/type_traits.hpp"
# include "../tools/tools.hpp"
# include "../iterators/random_access_iterator.hpp"

namespace ft
{
    template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T	                                        value_type;
			typedef Alloc	                                    allocator_type;
			typedef typename allocator_type::reference	        reference;
			typedef typename allocator_type::const_reference    const_reference;
			typedef typename allocator_type::pointer	        pointer;
			typedef typename allocator_type::const_pointer	    const_pointer;
			typedef ft::random_access_iterator<T>	            iterator;
            typedef ft::random_access_iterator<const T>         const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
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
                InputIterator>::type * = 0)
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
                return (*this);
			}

            // Iterators
            iterator    begin()
            { 
                return (iterator(&(this->_vector[0])));
            }

            const_iterator  begin() const
            {
                return (const_iterator(&(this->_vector[0])));
            }

            reverse_iterator    rbegin()
            {
                //return (reverse_iterator(&(this->_vector[this->_size])));
                return (reverse_iterator(this->end()));
            }

            iterator    end()
            { 
                return (iterator(&(this->_vector[this->_size])));
            }

            const_iterator  end() const
            {
                return (const_iterator(&(this->_vector[this->_size])));
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
                        this->_alloc.destroy(&(this->_vector[i]));
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

/*            void    reserve(size_type n)
            {
                if (n > this->max_size())
                    throw (std::length_error("reserve: length too high"));
                if (n <= this->_capacity)
                    return;
                if (this->_capacity == 0)
                {
                    this->_vector = this->_alloc.allocate(n);
                    this->_capacity = n;
                }
                else
                {
                    size_type   newCapacity = this->_capacity;

                    while (newCapacity < n)
                        newCapacity *= 2;

                    pointer newVec = this->_alloc.allocate(newCapacity);

                    for (unsigned int i = 0 ; i < this->_size ; ++i)
                    {
                        this->_alloc.construct(&newVec[i], this->_vector[i]);
                        this->_alloc.destroy(&(this->_vector[i]));
                    }
                    this->_alloc.deallocate(this->_vector, this->_capacity);
                    this->_vector = newVec;
                    this->_capacity = newCapacity;
                }
            }
*/

            //TODO fix capacity to be incremented has capacity *= 2

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
            // insert(): in range
            template < class InputIterator >
                void	insert(iterator position, InputIterator first, InputIterator last,
                        typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
                {
                    difference_type pos = position - this->begin();

                    this->reserve(this->_size + (last - first));
                    this->_shiftRight(pos, last - first);
                    for ( ; first != last ; ++first)
                    {
                        this->_alloc.construct(&(this->_vector[pos++]), *first);
                        ++this->_size;
                    }
                }

            // assign(): in range
            template < class InputIterator >
                void    assign(InputIterator first, InputIterator last)
                {
                    this->clear();
                    this->insert(this->begin(), first, last);
                }

            // assign(): fill
            void    assign(size_type n, const value_type &val)
            {
                this->clear();
                this->insert(this->begin(), n, val);
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
                        //*toDelete = 0;
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
                        //*toDelete = 0;
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

            friend void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
            {
                x.swap(y);
            }
    };
}

#endif // VECTOR_HPP
