#ifndef BIDIRECTIONAL_ITERATOR_HPP
# define BIDIRECTIONAL_ITERATOR_HPP

# include "../red_black_tree/rb_tree.hpp"
# include "../tools/type_traits.hpp"

namespace ft
{
    template < class T, bool B, class Compare = std::less<T>, class ValueCompare = Compare >
    class bidirectional_iterator
    {
        public:
            typedef bidirectional_iterator  iterator_category;
            typedef typename ft::conditional<B, const T, T >::type value_type;
            typedef value_type*	    pointer;
            typedef value_type&		reference;
            typedef Compare			compare;
            typedef ValueCompare	value_compare;
            typedef Node<T>*		node_pointer;
            typedef std::ptrdiff_t  difference_type;

            /// Constructors

            bidirectional_iterator(const compare &comp = compare())
                : _cur(NULL), _comp(comp), _valComp(comp){}

            bidirectional_iterator(node_pointer src, const compare &comp = compare())
                : _cur(src), _comp(comp), _valComp(comp){}

            bidirectional_iterator(bidirectional_iterator<T, false, Compare, ValueCompare> const &x)
                : _cur(x.getNode()), _comp(x.getComp()), _valComp(x.getComp()){}

            bidirectional_iterator  &operator=(bidirectional_iterator const &rhs)
            {
                if (this != &rhs)
                    _cur = rhs.getNode();
                return (*this);
            }

            ~bidirectional_iterator(){}

            /// Getters

            node_pointer    getNode() const{ return (_cur);}

            compare     getComp() const{ return (_comp);}

            /// Referencing

            reference   operator*() const{ return (*(_cur->data));}

            pointer     operator->() const{ return (_cur->data);}

            /// Incrementation

            bidirectional_iterator  &operator++()
            {
                if (_cur->right != _cur->leaf)
                {
                    _cur = _cur->right;
                    while (_cur->left != _cur->leaf)
                        _cur = _cur->left;
                }
                else
                {
                    Node<T> *tmp = _cur->parent;
                    while (tmp != NULL && _valComp(*(tmp->data), *(_cur->data)))
                        tmp = tmp->parent;
                    if (tmp == NULL)
                        _cur = _cur->leaf;
                    else
                        _cur = tmp;
                }

                return (*this);
            }

            bidirectional_iterator  &operator--()
            {
                if (_cur == _cur->leaf)
                    _cur = _cur->parent;
                else if (_cur->left != _cur->leaf)
                {
                    _cur = _cur->left;
                    while (_cur->right != _cur->leaf)
                        _cur = _cur->right;
                }
                else
                {
                    Node<T> *tmp = _cur->parent;
                    while (tmp != NULL && _valComp(*(_cur->data), *(tmp->data)))
                        tmp = tmp->parent;
                    if (tmp == NULL)
                        _cur = _cur->leaf;
                    else
                        _cur = tmp;
                }
                return (*this);
            }

            bidirectional_iterator  operator++(int)
            {
                bidirectional_iterator tmp(_cur);

                ++(*this);
                return (tmp);
            }

            bidirectional_iterator  operator--(int)
            {
                bidirectional_iterator  tmp(_cur);

                --(*this);
                return (tmp);
            }

        private:
            node_pointer		_cur;
            compare 			_comp;
            value_compare		_valComp;

            /// Relational operators

            friend bool operator==(const bidirectional_iterator &lhs,
                    const bidirectional_iterator &rhs)
            {
                if (lhs.operator->() == rhs.operator->())
                    return (true);
                if (lhs.operator->() == NULL || rhs.operator->() == NULL)
                    return (false);
                return (!lhs._valComp(*lhs, *rhs) && !rhs._valComp(*rhs, *lhs));
            }

            friend bool operator!=(const bidirectional_iterator &lhs,
                    const bidirectional_iterator &rhs)
            {
                return (!(lhs == rhs));
            }
    };
}

#endif // BIDIRECTIONAL_ITERATOR_HPP
