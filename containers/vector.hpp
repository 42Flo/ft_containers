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
			typedef T           value_type;
			typedef Alloc       allocator_type;
			typedef size_t      size_type;
            typedef ptrdiff_t   difference_type;

			typedef typename allocator_type::reference	        reference;
			typedef typename allocator_type::const_reference    const_reference;
			typedef typename allocator_type::pointer	        pointer;
			typedef typename allocator_type::const_pointer	    const_pointer;

			typedef ft::random_access_iterator<T, false>	    iterator;
            typedef ft::random_access_iterator<T, true>         const_iterator;
            typedef ft::reverse_iterator<iterator>              reverse_iterator;
            typedef ft::reverse_iterator<const_iterator>        const_reverse_iterator;

            allocator_type	get_allocator() const {return (this->_alloc);}

            /// Constructors

            // default
			explicit vector(const allocator_type &alloc = allocator_type())
				: _vector(NULL), _size(0), _capacity(0), _alloc(alloc){}

            // copy
			vector(const vector &x)
				: _vector(NULL), _size(0), _capacity(0), _alloc(x.get_allocator())
			{
                assign(x.begin(), x.end());
			}

            // fill
			explicit vector(size_type n, const value_type &val = value_type()
				, const allocator_type &alloc = allocator_type())
				: _vector(NULL), _size(0), _capacity(0), _alloc(alloc)
			{
                assign(n, val);
			}

            // in range
			template < class InputIterator >
			vector(InputIterator first, InputIterator last,
                const allocator_type &alloc = allocator_type(),
                typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                InputIterator>::type * = 0)
                : _vector(NULL), _size(0), _capacity(0), _alloc(alloc)
			{
                insert(this->begin(), first, last);
			}

            /// Destructor

            ~vector()
            {
                clear();
                if (_capacity != 0)
                {
                    _alloc.deallocate(_vector, _capacity);
                    _capacity = 0;
                }
            }

            /// Assignation

			ft::vector<T, Alloc>	operator=(const ft::vector<T, Alloc> &x)
			{
                if (&x != this)
                {
                    clear();
                    if (_capacity != 0)
                        _alloc.deallocate(_vector, _capacity);
                    _alloc = x.get_allocator();
                    _size = 0;
                    _capacity = 0;
                    insert(begin(), x.begin(), x.end());
                }
                return (*this);
			}

            /// Element access

            reference	operator[](size_type n)
            {
                if (n >= _size)
                    throw (std::out_of_range("op[]: out of range"));
                return (_vector[n]);
            }

            const_reference	operator[](size_type n) const{ return (_vector[n]);}

            reference	at(size_type n)
            {
                if (n >= _size)
                    throw (std::out_of_range("at: out of range"));
                return (_vector[n]);
            }

            const_reference	at(size_type n) const
            {
                if (n >= _size)
                    throw (std::out_of_range("at: out of range"));
                return (_vector[n]);
            }

            reference	front(){ return (_vector[0]);}

            const_reference	front() const{ return (_vector[0]);}

            reference	back(){ return (_vector[_size - 1]);}

            const_reference	back() const{ return (_vector[_size - 1]);}

            /// Iterators

            iterator    begin(){ return (_vector);}

            const_iterator  begin() const{ return (_vector);}

            reverse_iterator    rbegin(){ return (reverse_iterator(end()));}

            const_reverse_iterator  rbegin() const
            {
                return (const_reverse_iterator(end()));
            }

            iterator    end(){ return (_vector + _size);}

            const_iterator  end() const{ return (_vector + _size);}

            reverse_iterator    rend(){ return (reverse_iterator(begin()));}

            const_reverse_iterator  rend() const
            {
                return (const_reverse_iterator(begin()));
            }

            /// Capacity

			size_type	size() const{ return (_size);}

			size_type	max_size() const{ return (_alloc.max_size());}

			void	resize(size_type n, value_type val = value_type())
			{
                if (n < _size)
                    for (unsigned int i = n ; i < _size ; ++i)
                        _alloc.destroy(&(_vector[i]));
                else
                {
                    if (_capacity < n)
                        reserve(n);
                    for (unsigned int i = _size ; i < n ; ++i)
                        _alloc.construct(&(_vector[i]), val);
                }
                _size = n;
			}

			size_type	capacity() const{ return (_capacity);}
			
			bool    empty() const{ return (_size == 0);}

            void    reserve(size_type n)
            {
                if (n > max_size())
                    throw (std::length_error("reserve: length too high"));
                if (n > _capacity)
                {
                    if (_capacity == 0)
                        _vector = _alloc.allocate(n);
                    else
                    {
                        pointer newVec = _alloc.allocate(n);

                        for (unsigned int i = 0 ; i < _size ; ++i)
                        {
                            _alloc.construct(&newVec[i], _vector[i]);
                            _alloc.destroy(&(_vector[i]));
                        }
                        _alloc.deallocate(_vector, _capacity);
                        _vector = newVec;
                    }
                    _capacity = n;
                }
            }

            /// Modifiers

            void	push_back(const value_type &val)
            {
                reserve(_size + 1);
                _alloc.construct(&(_vector[_size]), val);
                ++_size;
            }

            void	pop_back()
            {
                _alloc.destroy(&(_vector[_size - 1]));
                --_size;
            }

            // single element
            iterator	insert(iterator position, const value_type &val)
            {
                difference_type pos = position - begin();

                insert(position, 1, val);
                return (iterator(_vector + pos));
            }

            // fill
            void	insert(iterator position, size_type n, const value_type &val)
            {
                difference_type pos = position - begin();

                reserve(_size + n);
                _shiftRight(pos, n);
                for (unsigned int i = 0 ; i < n ; ++i)
                    _alloc.construct(&(_vector[pos++]), val);
                _size += n;
            }

            // in range
            template < class InputIterator >
            void	insert(iterator position, InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
            {
                difference_type pos = position - begin();
                size_type   range = 0;
                InputIterator   tmp = first;

                for ( ; tmp != last ; ++tmp)
                    ++range;
                reserve(_size + range);
                _shiftRight(pos, range);
                for ( ; first != last ; ++first)
                {
                    _alloc.construct(&(_vector[pos++]), *first);
                    ++_size;
                }
            }

            // in range
            template < class InputIterator >
            void    assign(InputIterator first, InputIterator last)
            {
                clear();
                insert(begin(), first, last);
            }

            // fill
            void    assign(size_type n, const value_type &val)
            {
                clear();
                insert(begin(), n, val);
            }

            // single element
            iterator	erase(iterator position)
            {
                _alloc.destroy(&(*position));
                _shiftLeft(position - begin(), 1);
                --_size;
                return (position);
            }

            // in range
            iterator	erase(iterator first, iterator last)
            {
                difference_type n = last - first;
                iterator    tmp = first;

                while (first != last)
                {
                    _alloc.destroy(&(*first));
                    first++;
                }
                _shiftLeft(tmp - begin(), n);
                _size -= n;
                return (tmp);
            }

            void	swap(vector &x)
            {
                ft::swap(_vector, x._vector);
                ft::swap(_size, x._size);
                ft::swap(_capacity, x._capacity);
                ft::swap(_alloc, x._alloc);
            }

            void	clear()
            {
                for (unsigned int i = 0 ; i < _size ; ++i)
                    _alloc.destroy(&(_vector[i]));
                _size = 0;
            }

        private:
            pointer _vector;
            size_type	_size;
            size_type	_capacity;
            allocator_type  _alloc;

            void    _shiftRight(difference_type pos, size_type n)
            {
                if (_size == 0 || n == 0)
                    return;
                if ((size_type)pos < _size && _capacity >= _size + n)
                {
                    iterator    toWrite = end() + n - 1;
                    iterator    toDelete = end() - 1;

                    for (unsigned int i = 0 ; i < _size - pos ; ++i)
                    {
                        _alloc.construct(&(*toWrite--), *toDelete);
                        _alloc.destroy(&(*toDelete--));
                    }
                }
            }

            void    _shiftLeft(difference_type pos, size_type n)
            {
                if ((size_type)pos + 1 < _size)
                {
                    for (size_type i = pos ; i + n < _size ; ++i)
                    {
                        _alloc.construct(&(_vector[i]), _vector[i + n]);
                        _alloc.destroy(&(_vector[i + n]));
                    }
                }
            }

            /// Relational operators

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
