#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

# include <cstddef>
# include <iostream>
# include "../tools/type_traits.hpp"

namespace ft
{
    template < class T, bool B >
    class random_access_iterator
    {
        public:
            typedef random_access_iterator  iterator_category;
            typedef typename ft::conditional<B, const T, T>::type value_type;
            typedef std::ptrdiff_t	        difference_type;
            typedef value_type&             reference;
            typedef value_type*             pointer;

            /// Constructors

            random_access_iterator(pointer val = 0) : _current(val){}

            random_access_iterator(random_access_iterator<T, false> const &src)
                : _current(src.operator->()){}

            random_access_iterator	&operator=(random_access_iterator const &r)
            {
                if (this != &r)
                    this->_current = r.operator->();
                return (*this);
            }

            ~random_access_iterator(){}

            /// Referencing

            reference	operator*() const{ return (*(this->_current));}

            pointer operator->(){ return (this->_current);}

            pointer operator->() const{return (this->_current);}

            reference	operator[](difference_type r) const
            {
                return (*(this->_current + r));
            }

            /// Incrementation

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

            /// Arithmetic

            random_access_iterator  operator+(int r) const
            {
                return (this->_current + r);
            }

            random_access_iterator	operator-(int r) const
            {
                return (this->_current - r);
            }

            difference_type operator-(random_access_iterator const &r) const
            {
                return (this->_current - r.operator->());
            }

            /// Compound assignment

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

            // non-member operator+
            friend random_access_iterator   operator+(int r, random_access_iterator const &l)
            {
                return (l + r);
            }

            /// Relational operators

            friend bool	operator==(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (l.operator->() == r.operator->());
            }

            friend bool	operator!=(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (!(l == r));
            }

            friend bool	operator<(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (l.operator->() < r.operator->());
            }

            friend bool	operator<=(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (l.operator->() <= r.operator->());
            }

            friend bool	operator>(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (l.operator->() > r.operator->());
            }

            friend bool	operator>=(const random_access_iterator &l,
                    const random_access_iterator &r)
            {
                return (l.operator->() >= r.operator->());
            }
    };
}

#endif // RANDOM_ACCESS_ITERATOR_HPP
