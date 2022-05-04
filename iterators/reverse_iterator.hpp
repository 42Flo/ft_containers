//
// Created by Segal Codin on 17/03/2022.
//

#ifndef _VECTOR_REVERSE_ITERATOR_HPP
#define _VECTOR_REVERSE_ITERATOR_HPP

#include <memory>
# include "iterator_traits.hpp"

namespace ft
{
	template<typename Iterator>
	class reverse_iterator
	{
	public:

#pragma region TypeDefines
		typedef Iterator iterator_type;
		typedef typename ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename ft::iterator_traits<Iterator>::reference			reference;
		//typedef typename ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef std::ptrdiff_t difference_type;
#pragma endregion TypeDefines

#pragma region Constructors&Deconstruct

		//Constructors
		reverse_iterator() : _ptr(NULL)
		{};

		explicit reverse_iterator(iterator_type ptr) : _ptr(ptr)
		{};

		template<class Iter>
		reverse_iterator(reverse_iterator<Iter> const &src) : _ptr(src.base())
		{}

		//Deconstruct
		~reverse_iterator()
		{
		};

		//Equal Operator
		template < class Rit >
		reverse_iterator &operator=(reverse_iterator<Rit> const &src)
		{
			_ptr = src.base();
			return (*this);
		}

		iterator_type	base() const
		{
			return _ptr;
		}

#pragma endregion Constructors&Deconstruct

#pragma region Operator

		//Bool

		//Arithmetic
		reverse_iterator operator+(difference_type n) const
		{
			return reverse_iterator(_ptr - n);
		}

		reverse_iterator operator-(difference_type n) const
		{
			return reverse_iterator(_ptr + n);
		}

		difference_type operator-(reverse_iterator const &other) const
		{
			return (other.base() - _ptr);
		}

		//Compound Assignments
		reverse_iterator &operator+=(difference_type n)
		{
			_ptr -= n;
			return (*this);
		};

		reverse_iterator &operator-=(difference_type n)
		{
			_ptr += n;
			return (*this);
		};

		//Incremental
		//Pre
		reverse_iterator &operator++()
		{
			--_ptr;
			return (*this);
		}

		reverse_iterator &operator--()
		{
			++_ptr;
			return (*this);
		}

		//Post
		reverse_iterator operator++(int)
		{
			reverse_iterator tmp = *this;
			--_ptr;
			return tmp;
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator tmp = *this;
			++_ptr;
			return tmp;
		}

		//Index Operator
		reference operator[](difference_type index) const
		{
			return base()[-index - 1];
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		reference operator*() const
		{
			Iterator tmp = _ptr; return *--tmp;
		}

#pragma endregion Operator

	private:
		iterator_type _ptr;

	};
}


template <class Iter>
ft::reverse_iterator<Iter>
operator +(typename ft::reverse_iterator<Iter>::difference_type n, const ft::reverse_iterator<Iter> &it)
{
	return (ft::reverse_iterator<Iter>(it.base() - n));
}

template <class Iter>
typename ft::reverse_iterator<Iter>::difference_type
operator -(const ft::reverse_iterator<Iter> &lhs, const ft::reverse_iterator<Iter> &rhs)
{
	return (lhs.base() - rhs.base());
}

template < class Iterator1, class Iterator2 >
		bool operator==(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() == rhs.base());
}

template < class Iterator1, class Iterator2 >
bool operator!=(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() != rhs.base());
}

template < class Iterator1, class Iterator2 >
bool operator>(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() < rhs.base());
}

template < class Iterator1, class Iterator2 >
bool operator<(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() > rhs.base());
}

template < class Iterator1, class Iterator2 >
bool operator<=(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() >= rhs.base());
}

template < class Iterator1, class Iterator2 >
bool operator>=(ft::reverse_iterator<Iterator1> const &lhs, ft::reverse_iterator<Iterator2>  const &rhs)
{
	return (lhs.base() <= rhs.base());
}

#endif //_VECTOR_REVERSE_ITERATOR_HPP
