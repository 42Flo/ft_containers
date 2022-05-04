//
// Created by Segal Codin on 23/04/2022.
//

#ifndef _BIDIRECTIONAL_ITERATOR_HPP
#define _BIDIRECTIONAL_ITERATOR_HPP

# include "../tools/rb_tree.hpp"
# include "../tools/type_traits.hpp"

namespace ft
{
	template < class T, bool B ,class Compare = std::less<T>, class ValueCompare = Compare>
	class bidirectional_iterator
	{
	public:

		typedef typename ft::conditional<B, const T, T > ::type value_type;
		typedef value_type*				pointer;
		typedef value_type&				reference;
		typedef Compare					compare;
		typedef ValueCompare			value_compare;
		typedef Node<T>*				nodePtr;

		bidirectional_iterator(const compare &comp = compare()) :
				_comp(comp),
				_valComp(comp)
		{
			_cur = NULL;
		}

		bidirectional_iterator(nodePtr src, const compare &comp = compare()) :
		_comp(comp),
		_valComp(comp)
		{
			_cur = src;
		}

		bidirectional_iterator(bidirectional_iterator<T, false, Compare, ValueCompare> const &other) :
		_comp(other.GetCompare()),
		_valComp(other.GetCompare())
		{
			_cur = other.GetNode();
		}

		nodePtr GetNode() const
		{
			return (this->_cur);
		}

		compare GetCompare() const
		{
			return (this->_comp);
		}

		reference operator*() const
		{
			return (*(this->_cur->data));
		}

		pointer operator->() const
		{
			return (this->_cur->data);
		}


		bidirectional_iterator &operator=(bidirectional_iterator const &rhs)
		{
			if (this != &rhs)
				this->_cur = rhs.GetNode();
			return (*this);
		}

		bidirectional_iterator  &operator++()
		{
			if (this->_cur->right != this->_cur->leaf)
			{
				this->_cur = this->_cur->right;
				while (this->_cur->left != this->_cur->leaf)
					this->_cur = this->_cur->left;
			}
			else
			{
				Node<T> *tmp = this->_cur->parent;
				while (tmp != NULL && this->_valComp(*(tmp->data), *(this->_cur->data)))
					tmp = tmp->parent;
				if (tmp == NULL)
					this->_cur = this->_cur->leaf;
				else
					this->_cur = tmp;
			}

			return (*this);
		}

		bidirectional_iterator  &operator--()
		{
			if (this->_cur == this->_cur->leaf)
				this->_cur = this->_cur->parent;
			else if (this->_cur->left != this->_cur->leaf)
			{
				this->_cur = this->_cur->left;
				while (this->_cur->right != this->_cur->leaf)
					this->_cur = this->_cur->right;
			}
			else
			{
				Node<T> *tmp = this->_cur->parent;
				while (tmp != NULL && this->_valComp(*(this->_cur->data), *(tmp->data)))
					tmp = tmp->parent;
				if (tmp == NULL)
					this->_cur = this->_cur->leaf;
				else
					this->_cur = tmp;
			}
			return (*this);
		}

		bidirectional_iterator  operator++(int)
		{
			bidirectional_iterator tmp(this->_cur);
			++(*this);
			return (tmp);
		}

		bidirectional_iterator  operator--(int)
		{
			bidirectional_iterator  tmp(this->_cur);

			--(*this);
			return (tmp);
		}
#pragma endregion Operator

	private:
		nodePtr				_cur;
		compare 			_comp;
		value_compare		_valComp;

		friend bool operator==(const bidirectional_iterator &lhs,
				const bidirectional_iterator &rhs)
		{
			//return (lhs.GetNode() == rhs.GetNode());
            if (lhs.operator->() == rhs.operator->())
                return (true);
            if (lhs.operator->() == NULL || rhs.operator->() == NULL)
                return (false);
            return (!lhs._valComp(*lhs, *rhs) && !rhs._valComp(*rhs, *lhs));
//            return (*lhs == *rhs);
		}

		friend bool operator!=(const bidirectional_iterator &lhs,
				const bidirectional_iterator &rhs)
		{
			return (!(lhs == rhs));
		}
	};
}

#endif //_BIDIRECTIONAL_ITERATOR_HPP
