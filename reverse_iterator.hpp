#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft
{
	template < class Iterator >
		class	reverse_iterator
		{
			public:
				typedef Iterator	iterator_type;
				typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;
				typedef typename ft::iterator_traits<Iterator>::value_type	value_type;
				typedef typename ft::iterator_traits<Iterator>::difference_type	difference_type;
				typedef typename ft::iterator_traits<Iterator>::pointer	pointer;
				typedef typename ft::iterator_traits<Iterator>::reference	reference;

				reverse_iterator() : _base(0){}
				explicit reverse_iterator(iterator_type it) : _base(it){}
				template < class Iter >
				reverse_iterator(reverse_iterator<Iter> const &rev_it)
					: _base(rev_it.base()){}

				iterator_type	base() const{ return (this->_base);}

				// ********** Operator overloads **********

				// Referencing
				reference	operator*(){ return (*(this->_base.getCurrent()));}
				pointer	operator->(){ return(this->_base.getCurrent());}
				reference	operator[](int r)
				{
                    return (this->_base + r);
					//return (*(this->_base.getCurrent() + r));
				}

				// Increment / Decrease
				reverse_iterator	&operator++()
				{
                    --this->base;
					//--(this->_base.getCurrent());
					return (*this);
				}
				reverse_iterator	operator++(int)
				{
					reverse_iterator	tmp(*this);

					++(*this);
					return (tmp);
				}
				reverse_iterator	&operator--()
				{
                    ++this->base;
					//++(this->_base.getCurrent());
					return (*this);
				}
				reverse_iterator	operator--(int)
				{
					reverse_iterator	tmp(*this);

					--(*this);
					return (tmp);
				}

				// Arithmetic
				reverse_iterator	operator+(difference_type n) const
				{
                    return (this->_base - n);
					//return (this->_base.GetCurrent() - n);
				}

				reverse_iterator	operator-(difference_type n) const
				{
                    return (this->_base + n);
					//return (this->_base.GetCurrent() + n);
				}

				//Compound assignment
				reverse_iterator	operator+=(difference_type n) const
				{
					//this->_base.getCurrent() -= n;
					this->_base -= n;
					return (*this);
				}

				reverse_iterator	operator-=(difference_type n) const
				{
					//this->_base.getCurrent()  += n;
					this->_base += n;
					return (*this);
				}

			private:
				iterator_type	_base;

				// ********** Non-member operator overloads **********

				// Relational operators
				friend bool operator==(const reverse_iterator &l,
						const reverse_iterator &r)
				{
					//return (l.base().getCurrent() == r.base().getCurrent());
					return (l.base == r.base);
				}
				friend bool	operator!=(const reverse_iterator &l,
						const reverse_iterator &r)
				{
					//return (l.base().getCurrent() != r.base().getCurrent());
					return (l.base != r.base);
				}
				friend bool	operator<(const reverse_iterator &l,
						const reverse_iterator &r)
				{
					//return (l.base().getCurrent() < r.base().getCurrent());
					return (l.base < r.base);
				}
				friend bool	operator<=(const reverse_iterator &l,
						const reverse_iterator &r)
				{
					//return (l.base().getCurrent() <= r.base().getCurrent());
					return (l.base <= r.base);
				}
				friend bool	operator>(const reverse_iterator<Iterator> &l,
						const reverse_iterator<Iterator> &r)
				{
					//return (l.base().getCurrent() > r.base().getCurrent());
					return (l.base > r.base);
				}
				friend bool	operator>=(const reverse_iterator<Iterator> &l,
						const reverse_iterator<Iterator> &r)
				{
					return (l.base >= r.base);
				}

				//TODO operator+ operator-
		};

}

#endif // REVERSE_ITERATOR_HPP
