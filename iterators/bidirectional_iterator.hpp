#ifndef BIDIRECTIONAL_ITERATOR_HPP
# define BIDIRECTIONAL_ITERATOR_HPP

# include <iostream>
# include <memory>

namespace ft
{
    template < class T, class Compare = std::less<T>, class ValueCompare = Compare>
    class bidirectional_iterator;
}
# include "../tools/rb_tree.hpp"
# include "../containers/map.hpp"

namespace ft
{
    template < class T, class Compare, class ValueCompare >
    class bidirectional_iterator
    {
        public:
            typedef T               value_type;
            typedef Compare         compare;
            typedef ValueCompare    value_compare;
            typedef T*              pointer;
            typedef T&              reference;

            /// Constructors

            // default
            bidirectional_iterator(const compare &comp = compare())
                : _cur(NULL), _comp(comp), _val_comp(comp){}

            // by Node
            bidirectional_iterator(Node<value_type> *src, const compare &comp = compare())
                : _cur(src), _comp(comp), _val_comp(comp){}

            // copy
            bidirectional_iterator(bidirectional_iterator const &src)
                : _cur(src.getNode()), _comp(src.comp()), _val_comp(src.comp()){}

            /// Assignation operator

            bidirectional_iterator  &operator=(bidirectional_iterator const &r)
            {
                if (this != &r)
                    this->_cur = r.getNode();
                return (*this);
            }

            Node<value_type> *getNode() const{ return (this->_cur);}

            compare comp() const{ return (this->_comp);}

            /// Referencing

            reference   operator*() const{ return (*(this->_cur->data));}
            
            pointer operator->() const{ return (this->_cur->data);}

            /// Increment / Decrement

            bidirectional_iterator  &operator++()
            {
                bidirectional_iterator  p = this->_cur->parent;
                bidirectional_iterator  gp = (p != NULL) ? this->_cur->parent->parent : NULL;

                if (this->_cur->right == NULL && p != NULL &&
                        this->_val_comp(*(*this), *p))
                    this->_cur = this->_cur->parent;
                else if (this->_cur->right == NULL && p != NULL && gp != NULL &&
                        this->_val_comp(*(*this), *gp))
                    this->_cur = this->_cur->parent->parent;
                else if (this->_cur->right != NULL)
                {
                    this->_cur = this->_cur->right;
                    while (this->_cur != NULL && this->_cur->left != NULL)
                        this->_cur = this->_cur->left;
                }
                else
                    this->_cur = NULL;
                return (*this);
            }

            bidirectional_iterator  operator++(int)
            {
                bidirectional_iterator tmp(this->_cur);

                ++(*this);
                return (tmp);
            }

            bidirectional_iterator  &operator--()
            {
                bidirectional_iterator  p = this->_cur->parent;
                bidirectional_iterator  gp = (p != NULL) ? this->_cur->parent->parent : NULL;

                if (this->_cur->left == NULL && p != NULL &&
                        !this->_val_comp(*(*this), *p))
                    this->_cur = this->_cur->parent;
                else if (this->_cur->left == NULL && p != NULL && gp != NULL &&
                        !this->_comp(*(*this), *gp))
                    this->_cur = this->_cur->parent->parent;
                else if (this->_cur->left != NULL)
                {
                    this->_cur = this->_cur->left;
                    while (this->_cur != NULL && this->_cur->right != NULL)
                        this->_cur = this->_cur->right;
                }
                return (*this);
            }

            bidirectional_iterator  operator--(int)
            {
                bidirectional_iterator  tmp(this->_cur);

                --(*this);
                return (tmp);
            }

        private:
            Node<value_type>    *_cur;
            compare             _comp;
            value_compare       _val_comp;

            /// Relational operators

            friend bool operator==(const bidirectional_iterator &l,
                    const bidirectional_iterator &r)
            {
                return (l.getNode() == r.getNode());
            }

            friend bool operator!=(const bidirectional_iterator &l,
                    const bidirectional_iterator &r)
            {
                return (!(l == r));
            }
    };
}

#endif // BIDIRECTIONAL_ITERATOR_HPP
