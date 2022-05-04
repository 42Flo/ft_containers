#ifndef NODE_HPP
# define NODE_HPP

# define BLACK 0
# define RED 1

# include <iostream>
# include <functional>
# include <memory>

namespace ft
{
	template < class T >
	struct Node
	{
		typedef T*			pointer;
		typedef Node<T>*	node_pointer;

		pointer data;
		bool    color;
		node_pointer    parent;
		node_pointer	left;
		node_pointer	right;
		node_pointer 	leaf;

		node_pointer	getUncle() const
		{
			if (this->parent == NULL || this->parent->parent == NULL)
				return (NULL);
			else if (this->parent == this->parent->parent->right)
				return this->parent->parent->left;
			else
				return this->parent->parent->right;
		}

		node_pointer	getSibling() const
		{
			if (this->parent == NULL)
				return (NULL);
			else if (this == this->parent->right)
				return (this->parent->left);
			else
				return (this->parent->right);
		}

		void	moveDown(node_pointer nParent)
		{
			if (parent != NULL)
			{
				if (isLeftChild())
					parent->left = nParent;
				else
					parent->right = nParent;
			}
			nParent->parent = parent;
			parent = nParent;
		}

		bool isRightChild()
		{
			return (this->parent->right == this);
		}

		bool isLeftChild()
		{
			return (this->parent->left == this);
		}

		bool hasRedChild()
		{
			return ((this->left != leaf && this->left->color == RED) ||
					(this->right != leaf && this->right->color == RED));
		}

	};
}

#endif // NODE_HPP
