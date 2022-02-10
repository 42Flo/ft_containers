#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft
{
    template < class T, class Container = ft::vector<T> >
    class stack
    {
        public:
            typedef T   value_type;
            typedef Container   container_type;
            typedef size_t  size_type;

            // Default constructor
            explicit stack(const container_type &ctnr = container_type())
                : _ctnr(ctnr){}

            bool    empty() const{ return (this->_ctnr.empty());}

            size_type   size() const{ return (this->_ctnr.size());}

            value_type  &top() { return (this->ctnr.back());}
            const value_type    &top() const { return (this->ctnr.back());}

            void    push(const value_type &val) { this->ctnr.push_back(val);}

            void    pop() { this->ctnr.pop_back();}

        private:
            const container_type    _ctnr;

            friend bool operator==(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l == r);
            }
            friend bool operator!=(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l != r);
            }
            friend bool operator<(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l < r);
            }
            friend bool operator<=(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l <= r);
            }
            friend bool operator>(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l > r);
            }
            friend bool operator>=(const stack<T, Container> &l,
                    const stack<T, Container> &r)
            {
                return (l >= r);
            }
    };
}

#endif // STACK_HPP
