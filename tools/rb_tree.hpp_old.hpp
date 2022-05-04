#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <iostream>
# include <functional>
# include <memory>

# include "tools.hpp"

//# include "../iterators/bidirectional_iterator.hpp"

# define BLACK 0
# define RED 1

template < class T >
struct Node
{
    typedef T*  ptr;
    typedef Node<T>* nodePtr;

    ptr		data;
    bool	color;
    nodePtr		parent;
    nodePtr		left;
    nodePtr		right;
    nodePtr 	leaf;

    nodePtr	getUncle() const
    {
        if (this->parent == NULL || this->parent->parent == NULL)
            return NULL;
        else if (this->parent == this->parent->parent->right)
            return this->parent->parent->left;
        else
            return this->parent->parent->right;
    }

    nodePtr	getSibling() const
    {
        if (this->parent == NULL)
            return NULL;
        else if (this == this->parent->right)
            return (this->parent->left);
        else
            return (this->parent->right);
    }

    void	moveDown(nodePtr	nParent)
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
        return this->parent->right == this;
    }

    bool isLeftChild()
    {
        return this->parent->left == this;
    }

    bool hasRedChild()
    {
        return ((this->left != leaf && this->left->color == RED) ||
                (this->right != leaf && this->right->color == RED));
    }
};

template < class T, class Compare = std::less<T>, class ValueCompare = Compare,
         class Alloc = std::allocator<T> >
class RBTree
{
	public :
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;

		typedef Node<T> *nodePtr;

		typedef Compare compare;
		typedef ValueCompare value_compare;
		typedef Alloc alloc_type;

		typedef std::allocator< Node <T> >  node_alloc_type;

		RBTree(const compare comp = compare()) :
		_comp(comp),
		_value_compare(comp),
		_alloc(alloc_type()),
		_node_alloc(node_alloc_type())
		{
			_lowest = NULL;
			_highest = NULL;
			_root = NULL;
			_leaf = CreateLeaf();
		}

		~RBTree()
		{
//			FreeNode(_leaf);
			FreeLeaf();
		}

		nodePtr getRoot() const
		{
			return (this->_root);
		}

		nodePtr getLowest() const
		{
			return this->_lowest;
		}

		nodePtr getHighest() const
		{
			return this->_highest;
		}

        nodePtr getLeaf() const
        {
            return (this->_leaf);
        }

		nodePtr FindNode(const value_type &newVal)
		{
			nodePtr tmp = _root;
			while (tmp != _leaf)
			{
				if (_value_compare(newVal, *(tmp->data)))
				{
					if (tmp->left == _leaf)
						break;
					else
						tmp = tmp->left;
				} else if (CheckEquality(*(tmp->data), newVal))
					break;
				else
				{
					if (tmp->right == _leaf)
						break;
					else
						tmp = tmp->right;
				}
			}
			return tmp;
		}

		nodePtr insert(const value_type &newVal)
		{
			nodePtr newNode = CreateNode(newVal);
			if (_root == NULL)
			{
				newNode->color = BLACK;
				_root = newNode;
			} else
			{
				nodePtr tmp = FindNode(newVal);

				if (CheckEquality(*(tmp->data), newVal))
                {
                    freeNode(newNode);
					return tmp; //No insertion happened due to the element being presented
                }

				newNode->parent = tmp;
				if (_value_compare(newVal, *(tmp->data)))
					tmp->left = newNode;
				else
					tmp->right = newNode;

				FixRedRed(newNode);
			}
			return newNode;
		}

		void	deleteByValue(const value_type &val)
		{
			if (_root == NULL)
				return;

			nodePtr tmp = FindNode(val);

			if (!CheckEquality(val, *(tmp->data)))
			{
				return;
			}
			deleteNode(tmp);
		}

		void display(nodePtr node = NULL, std::string indent = "\t", bool side = false) const
		{
			if (_root == NULL)
				return;
			if (node == NULL)
				node = _root;
			if (node != _leaf)
			{
				std::cout << indent;
				if (side)
				{
					std::cout << "R----";
					indent += "   ";
				} else
				{
					std::cout << "L----";
					indent += "|  ";
				}
				std::string color = node->color ? "RED" : "BLACK";
				std::cout << node->data->first << ", " << node->data->second <<
						  "(" << color << ")" << std::endl;
				display(node->left, indent, false);
				display(node->right, indent, true);
			}
            else
            {             
				std::cout << indent;
				if (side)
				{
					std::cout << "R----";
					indent += "   ";
				} else
				{
					std::cout << "L----";
					indent += "|  ";
				}
                std::cout << "LEAF" << std::endl;
            }
		}

		// prints level order for given node
		/*void levelOrder(nodePtr x)
		{
			if (x == NULL)
				// return if node is null
				return;

			// queue for level order
			std::queue<nodePtr> q;
			nodePtr curr;

			// push x
			q.push(x);

			while (!q.empty())
			{
				// while q is not empty
				// dequeue
				curr = q.front();
				q.pop();

				// print node value
				std::cout << (curr->data->first) << " ";

				// push children to queue
				if (curr->left != _leaf)
					q.push(curr->left);
				if (curr->right != _leaf)
					q.push(curr->right);
			}
		}

		void printLevelOrder()
		{
			std::cout << "Level order: " << std::endl;
			if (_root == NULL)
				std::cout << "Tree is empty" << std::endl;
			else
				levelOrder(_root);
			std::cout << std::endl;
		}*/


	private :
		nodePtr _root;
		nodePtr _lowest;
		nodePtr _highest;
		nodePtr _leaf;
		compare _comp;
		value_compare _value_compare;
		alloc_type _alloc;
		node_alloc_type _node_alloc;

		nodePtr CreateLeaf()
		{
			nodePtr leaf = _node_alloc.allocate(1);
			leaf->data = NULL;
			leaf->color = BLACK;
			leaf->parent = NULL;
			leaf->right = NULL;
			leaf->left = NULL;
			return leaf;
		}

		nodePtr CreateNode(const value_type &val)
		{
			nodePtr newNode = _node_alloc.allocate(1);
			newNode->data = _alloc.allocate(1);
			_alloc.construct(newNode->data, val);
			newNode->leaf = _leaf;
			newNode->color = RED;
			newNode->parent = NULL;
			newNode->right = _leaf;
			newNode->left = _leaf;
			return newNode;
		}

		void	FreeLeaf()
		{
			_node_alloc.deallocate(_leaf, 1);
		}

		void	FreeNode(nodePtr node)
		{
//			std::cout  << "The node to be freed = " << node->data->first << std::endl;
			_alloc.destroy(node->data);
			_alloc.deallocate(node->data, 1);
			_node_alloc.deallocate(node, 1);
		}

        void    insertUpLowHigh(nodePtr node)
        {

        }

		void	deleteNode(nodePtr node)
		{
			nodePtr u = BSTreplace(node);

			bool uvBlack = ((u == _leaf || u->color == BLACK)
							&& node->color == BLACK);
			nodePtr parent = node->parent;
			if (u == _leaf)
			{
				if (node == _root)
				{
					_root = NULL;
				}
				else
				{
					if (uvBlack)
					{
						FixDoubleBlack(node);
					}
					else
					{
						if (node->getSibling() != NULL)
							node->getSibling()->color = RED;
					}
					if (node->isLeftChild())
						parent->left = _leaf;
					else
						parent->right = _leaf;
				}
				FreeNode(node);
				return;
			}

			if (node->left == _leaf || node->right == _leaf)
			{
				if (node == _root)
				{
					ft::swap(node->data, u->data);
					node->left = _leaf;
					node->right = _leaf;
					FreeNode(u);
				}
				else
				{
					if (node->isLeftChild())
						parent->left = u;
					else
						parent->right = u;
					FreeNode(node);
					u->parent = parent;
					if (uvBlack)
						FixDoubleBlack(u);
					else
						u->color = BLACK;
				}
				return;
			}
			ft::swap(node->data, u->data);
			deleteNode(u);
		}

		void SwapValue(nodePtr node1, nodePtr node2)
		{
			value_type tmp;
			tmp = *(node1->data);
			*(node1->data) = *(node2->data);
			*(node2->data) = tmp;
		}

		void FixRedRed(nodePtr node)
		{
			if (node == _root)
			{
				node->color = BLACK;
				return;
			}

			nodePtr parent = node->parent;
			nodePtr grandParent = parent->parent;
			nodePtr uncle = node->getUncle();

			if (parent->color != BLACK)
			{
				if (uncle != NULL && uncle->color == RED)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					grandParent->color = RED;
					FixRedRed(grandParent);
				} else
				{
					if (parent->isLeftChild())
					{
						if (node->isLeftChild())
						{
							ft::swap(parent->color, grandParent->color);
						} else
						{
							LeftRotate(parent);
							ft::swap(node->color, grandParent->color);
						}
						RightRotate(grandParent);
					} else
					{
						if (node->isLeftChild())
						{
							RightRotate(parent);
							ft::swap(node->color, grandParent->color);
						} else
						{
							ft::swap(parent->color, grandParent->color);
						}
						LeftRotate(grandParent);
					}
				}
			}
		}

		void FixDoubleBlack(nodePtr node)
		{
			if (node == _root)
				return;

			nodePtr sibling = node->getSibling();
			nodePtr parent = node->parent;
			if (sibling == NULL)
				FixDoubleBlack(parent);
			else
			{
				if (sibling->color == RED)
				{
					parent->color = RED;
					sibling->color = BLACK;
					if (sibling->isLeftChild())
						RightRotate(parent);
					else
						LeftRotate(parent);

					FixDoubleBlack(node);
				}
				else
				{
					if (sibling->hasRedChild())
					{
						if (sibling->left != _leaf && sibling->left->color == RED)
						{
							if (sibling->isLeftChild())
							{
								// left left
								sibling->left->color = sibling->color;
								sibling->color = parent->color;
								RightRotate(parent);
							}
							else
							{
								// right left
								sibling->left->color = parent->color;
								RightRotate(sibling);
								LeftRotate(parent);
							}
						}
						else
						{
							if (sibling->isLeftChild())
							{
								// left right
								sibling->right->color = parent->color;
								LeftRotate(sibling);
								RightRotate(parent);
							} else
							{
								// right right
								sibling->right->color = sibling->color;
								sibling->color = parent->color;
								LeftRotate(parent);
							}
						}
						parent->color = BLACK;
					}
					else
					{
						sibling->color = RED;
						if (parent->color == BLACK)
							FixDoubleBlack(parent);
						else
							parent->color = BLACK;
					}
				}
			}
		}

		nodePtr	successor(nodePtr node)
		{
			nodePtr tmp = node;

			while (tmp->left != _leaf)
				tmp = tmp->left;
			return tmp;
		}

		nodePtr	BSTreplace(nodePtr node)
		{
			if (node->left != _leaf && node->right != _leaf)
				return successor(node->right);

			if (node->left == _leaf && node->right == _leaf)
				return _leaf;

			if (node->left != _leaf)
				return node->left;
			else
				return node->right;
		}

		void LeftRotate(nodePtr node)
		{
			nodePtr nParent = node->right;

			if (node == _root)
				_root = nParent;

			node->moveDown(nParent);

			node->right = nParent->left;

			if (nParent->left != _leaf)
				nParent->left->parent = node;

			nParent->left = node;
		}

		void RightRotate(nodePtr node)
		{
			nodePtr nParent = node->left;

			if (node == _root)
				_root = nParent;

			node->moveDown(nParent);
			node->left = nParent->right;
			if (nParent->right != _leaf)
				nParent->right->parent = node;

			nParent->right = node;
		}

		bool CheckEquality(const value_type &val1, const value_type &val2)
		{
			return (!_value_compare(val1, val2) &&
					!_value_compare(val2, val1));
		}

	};


#endif
