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
			//typedef ft::random_access_iterator<T, false>	    iterator;
			typedef ft::random_access_iterator<T, false>	        iterator;
            typedef ft::random_access_iterator<T, true>         const_iterator;
            //typedef ft::random_access_iterator<T, true>         const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;
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

            // Destructor

            ~vector()
            {
                this->clear();
                if (this->_capacity != 0)
                    this->_alloc.deallocate(this->_vector, this->_capacity);
            }

			// Assignation operator
			ft::vector<T, Alloc>	operator=(const ft::vector<T, Alloc> &x)
			{
                if (&x != this)
                {
                    this->clear();
                    if (this->_capacity != 0)
                        this->_alloc.deallocate(this->_vector, this->_capacity);
                    this->_alloc = x.get_allocator();
                    this->_size = 0;
                    this->_capacity = 0;
                    this->insert(this->begin(), x.begin(), x.end());
                }
                return (*this);
			}

            // Iterators
            iterator    begin()
            { 
                //return (iterator(&(this->_vector[0])));
                return (iterator(this->_vector));
            }

            const_iterator  begin() const
            {
                return (const_iterator(this->_vector));
                //return (const_iterator(&(this->_vector[0])));
            }

            reverse_iterator    rbegin()
            {
                //return (reverse_iterator(&(this->_vector[this->_size])));
                return (reverse_iterator(this->end()));
            }

            const_reverse_iterator  rbegin() const
            {
                return (const_reverse_iterator(this->end()));
            }

            iterator    end()
            { 
                return (iterator(this->_vector + this->_size));
                //return (iterator(&(this->_vector[this->_size])));
            }

            const_iterator  end() const
            {
                return (const_iterator(this->_vector + this->_size));
                //return (const_iterator(&(this->_vector[this->_size])));
            }

            reverse_iterator    rend()
            {
                return (reverse_iterator(this->begin()));
            }

            const_reverse_iterator  rend() const
            {
                return (const_reverse_iterator(this->begin()));
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
                difference_type pos = position - this->begin();

                this->insert(position, 1, val);
                return (iterator(this->_vector + pos));
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
                size_type   range = 0;
                InputIterator   tmp = first;

                for ( ; tmp != last ; ++tmp)
                    ++range;
                this->reserve(this->_size + range);
                this->_shiftRight(pos, range);
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
                iterator    tmp = first;

                for ( ; first != last ; ++first)
                    this->_alloc.destroy(&(*first));
                this->_shiftLeft(tmp - this->begin(), n);
                this->_size -= n;
                return (tmp);
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
                    this->_alloc.destroy(&(this->_vector[i]));
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
                if (n > this->_size)
                    throw (std::out_of_range("at: out of range"));
                return (this->_vector[n]);
            }
            // const at()
            const_reference	at(size_type n) const
            {
                if (n > this->_size)
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
                        this->_alloc.destroy(&(*toDelete--));
                    }
                }
            }

            void    _shiftLeft(difference_type pos, size_type n)
            {
                if ((size_type)pos + 1 < this->_size)
                {
                    for (size_type i = pos ; i + n < this->_size ; ++i)
                    {
                        this->_alloc.construct(&(this->_vector[i]), this->_vector[i + n]);
                        this->_alloc.destroy(&(this->_vector[i + n]));
                    }
                }
            }

        private:
            pointer _vector;
            size_type	_size;
            size_type	_capacity;
            allocator_type  _alloc;

            // Relational operators
            friend bool operator==(const vector &l,
                    const vector &r)
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

            friend bool operator!=(const vector &l, const vector &r)
            {
                return (!(l == r));
            }

            friend bool operator<(const vector &l, const vector &r)
            {
                return (ft::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()));
            }

            friend bool operator<=(const vector &l, const vector &r)
            {
                return (!(r < l));
            }

            friend bool operator>(const vector &l, const vector &r)
            {
                return (r < l);
            }

            friend bool operator>=(const vector &l, const vector &r)
            {
                return (!(l < r));
            }
    };

    template < class T, class Alloc >
    void    swap(ft::vector<T, Alloc> &x, ft::vector<T, Alloc> &y)
    {
        x.swap(y);
    }
}

#endif // VECTOR_HPP
