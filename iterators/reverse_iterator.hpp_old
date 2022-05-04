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
            //typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;
            typedef typename ft::iterator_traits<Iterator>::value_type	value_type;
            typedef typename ft::iterator_traits<Iterator>::difference_type	difference_type;
            typedef typename ft::iterator_traits<Iterator>::pointer	pointer;
            typedef typename ft::iterator_traits<Iterator>::reference	reference;

            reverse_iterator() : _base(0){}

            explicit reverse_iterator(iterator_type it) : _base(it){}

            template < class Iter >
            reverse_iterator(reverse_iterator<Iter> const &rev_it)
                : _base(rev_it.base()){}

            template < class Rit >
            reverse_iterator    &operator=(const reverse_iterator<Rit> &other)
            {
                this->_base = other.base();
                return (*this);
            }

            iterator_type	base() const{ return (this->_base);}

            // ********** Operator overloads **********

            // Referencing

            reference   operator*() const
            {
                return (*(this->_base - 1));
            }

            pointer	operator->() const { return (this->_base.operator->() - 1);}

            reference	operator[](difference_type r) const
            {
                return (this->_base[-r - 1]);
            }

            // Increment / Decrease
            reverse_iterator	&operator++()
            {
                --this->_base;
                return (*this);
            }

            reverse_iterator	operator++(int)
            {
                reverse_iterator	tmp(*this);

                --this->_base;
                return (tmp);
            }

            reverse_iterator	&operator--()
            {
                ++this->_base;
                //++this->_base;
                return (*this);
            }

            reverse_iterator	operator--(int)
            {
                reverse_iterator	tmp(*this);

                ++this->_base;
                return (tmp);
            }

            // Arithmetic
            reverse_iterator	operator+(difference_type n) const
            {
                return (reverse_iterator(this->_base - n));
                //return (this->_base.GetCurrent() - n);
            }

            reverse_iterator	operator-(difference_type n) const
            {
                return (reverse_iterator(this->_base + n));
            }

            difference_type     operator-(reverse_iterator const &r) const
            {
                return (r.base() - this->_base);
            }

            //Compound assignment
            reverse_iterator	operator+=(difference_type n)
            {
                this->_base -= n;
                return (*this);
            }

            reverse_iterator	operator-=(difference_type n)
            {
                this->_base += n;
                return (*this);
            }

        private:
            iterator_type	_base;

            // ********** Non-member operator overloads **********

            /*friend reverse_iterator operator+(difference_type l, const reverse_iterator &r)
            {
                return (reverse_iterator(r.base() - l));
            }

            friend difference_type  operator-(const reverse_iterator &l,
                    const reverse_iterator &r)
            {
                return (r.base() - l.base());
            }*/

            //TODO non member op-

            // Relational operators
            /*friend bool operator==(const reverse_iterator &l,
                    const reverse_iterator &r)
            {
                //return (l.base().getCurrent() == r.base().getCurrent());
                return (l.base() == r.base());
            }*/
            /*
            friend bool	operator!=(const reverse_iterator &l,
                    const reverse_iterator &r)
            {
                //return (l.base().getCurrent() != r.base().getCurrent());
                return (l.base() != r.base());
            }
            friend bool	operator<(const reverse_iterator &l,
                    const reverse_iterator &r)
            {
                //return (l.base().getCurrent() < r.base().getCurrent());
                return (l.base() < r.base());
            }
            friend bool	operator<=(const reverse_iterator &l,
                    const reverse_iterator &r)
            {
                //return (l.base().getCurrent() <= r.base().getCurrent());
                return (l.base() <= r.base());
            }
            friend bool	operator>(const reverse_iterator<Iterator> &l,
                    const reverse_iterator<Iterator> &r)
            {
                //return (l.base().getCurrent() > r.base().getCurrent());
                return (l.base() > r.base());
            }
            friend bool	operator>=(const reverse_iterator<Iterator> &l,
                    const reverse_iterator<Iterator> &r)
            {
                return (l.base() >= r.base());
            }*/
    };

    template < class It1, class It2 >
    bool    operator==(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        return (l.base() == r.base());
    }

    template < class It1, class It2 >
    bool    operator!=(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        return (l.base() != r.base());
    }

    template < class It1, class It2 >
    bool    operator<(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        //return (l.base() < r.base());
        return (l.base() > r.base());
    }

    template < class It1, class It2 >
    bool    operator<=(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        //return (l.base() <= r.base());
        return (l.base() >= r.base());
    }

    template < class It1, class It2 >
    bool    operator>(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        //return (l.base() > r.base());
        return (l.base() < r.base());
    }

    template < class It1, class It2 >
    bool    operator>=(const ft::reverse_iterator<It1> &l, const ft::reverse_iterator<It2> &r)
    {
        //return (l.base() >= r.base());
        return (l.base() <= r.base());
    }

    template < class It >
    ft::reverse_iterator<It>    operator+(typename ft::reverse_iterator<It>::difference_type n,
            const ft::reverse_iterator<It> &it)
    {
        return (ft::reverse_iterator<It>(it.base() - n));
    }

    template < class It >
    typename ft::reverse_iterator<It>::difference_type  operator-(const ft::reverse_iterator<It> &l,
            const ft::reverse_iterator<It> &r)
    {
        return (r.base() - l.base());
    }
}

#endif // REVERSE_ITERATOR_HPP
