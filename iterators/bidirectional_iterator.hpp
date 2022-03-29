#ifndef BIDIRECTIONAL_ITERATOR_HPP
# define BIDIRECTIONAL_ITERATOR_HPP

# include "../tools/rb_tree.hpp"

namespace ft
{
    template < class T, class Compare = std::less<T>, class KeyCompare = Compare,
         class Alloc = std::allocator<T> >
    class bidirectional_iterator
    {
        public:
            typedef T           value_type;
            typedef Compare     compare;
            typedef KeyCompare  key_compare;
            typedef Alloc       alloc_type;
            typedef T*          pointer;

            // Default
            bidirectional_iterator(Node<value_type, Alloc> src = 0,//TODO compile error!!!! TO FIX!
                    const compare &comp = key_compare()) : _cur(src), _comp(comp){}

            // Copy
            bidirectional_iterator(bidirectional_iterator const &src)
                : _cur(src.getCurrent()), _comp(src.key_comp()){}

            // Assignation
            bidirectional_iterator  operator=(bidirectional_iterator &r)
            {
                if (this != &r)
                    this->_current = r.getCurrent();
                return (*this);
            }

            Node<value_type, Alloc> *getNode() const{ return (this->_cur);}

            compare comp() const{ return (this->_comp);}

            // Referencing
            //bidirectional_iterator  operator*(){ return (*(this->_cur));}
            value_type  *operator*(){ return (this->_cur->data);}//not sure about that

            pointer operator->(){ return (&(this->_cur->data));}

            // Increment / Decrement
            bidirectional_iterator  &operator++()
            {
                bidirectional_iterator  parent = this->_cur->parent;

                if (this->_cur->right == NULL && parent != NULL &&
                        this->_comp((*this)->first, parent->first))
                    this->_cur = this->_cur->parent;
                else if (this->_cur->right)
                {
                    this->_cur = this->_cur->right;
                    while (this->_cur->left)
                        this->_cur = this->_cur->left;
                }
                return (*this);
            }

            bidirectional_iterator  operator++(int)
            {
                bidirectional_iterator tmp(this->_cur);

                ++this;
                return (tmp);
            }

            bidirectional_iterator  &operator--()
            {
                bidirectional_iterator  parent = this->_cur->parent;

                if (this->_cur->left == NULL && parent != NULL &&
                        !this->_comp((*this)->first, parent->first))//TODO change compare function
                    this->_cur = this->_cur->parent;
                else if (this->_cur->left)
                {
                    this->_cur = this->_cur->left;
                    while (this->_cur->right)
                        this->_cur = this->_cur->right;
                }
                return (*this);
            }

            bidirectional_iterator  operator--(int)
            {
                bidirectional_iterator  tmp(this->_cur);

                --this;
                return (tmp);
            }

            key_compare key_comp() const {return (this->_comp);}

        private:
            Node<value_type, Alloc> *_cur;
            compare             _comp;

            // Relational operators
            friend bool operator==(const bidirectional_iterator &l,
                    const bidirectional_iterator &r)
            {
                return (l.getCurrent() == r.getCurrent());
            }

            friend bool operator!=(const bidirectional_iterator &l,
                    const bidirectional_iterator &r)
            {
                return (!(l == r));
            }
    };
}

#endif // BIDIRECTIONAL_ITERATOR_HPP
