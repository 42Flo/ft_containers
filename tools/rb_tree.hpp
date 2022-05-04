#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <iostream>
#include <functional>
#include <memory>
#include <queue>

# include "tools.hpp"
#include "node.hpp"

namespace ft
{
	template<class T, class Compare = std::less<T>, class ValueCompare = Compare,
			class Alloc = std::allocator<T> >
	class RedBlackTree
	{
        public :
		    typedef T value_type;
		    typedef T *pointer;
		    typedef T &reference;
		    typedef const T &const_reference;

		    typedef Node<T> * nodePtr;

		    typedef Compare compare;
		    typedef ValueCompare value_compare;
		    typedef Alloc alloc_type;
		    typedef std::allocator<Node<T> > node_alloc_type;

		    RedBlackTree(const compare comp = compare()) :
				_comp(comp),
				_value_compare(comp),
				_alloc(alloc_type()),
				_node_alloc(node_alloc_type())
		    {
			    _root = NULL;
			    _leaf = _createLeaf();
                _lowest = _leaf;
                _highest = _leaf;
		    }

		    RedBlackTree(const RedBlackTree &other) :
				_comp(other._comp),
				_value_compare(other._comp),
				_alloc(alloc_type()),
				_node_alloc(node_alloc_type())
		    {
			    _root = NULL;
			    _leaf = _createLeaf();
                _lowest = _leaf;
                _highest = _leaf;
			    CopyTree(other.getRoot(), &_root, NULL);
                _lowest = _findLowest();
                _highest = _findHighest();
		    }

		    ~RedBlackTree()
		    {
			    clear();
			    _freeLeaf();
	    	}

		    RedBlackTree	&operator=(const RedBlackTree &other)
		    {
			    if (this != &other)
			    {
			    	clear();
                    _lowest = _leaf;
                    _highest = _leaf;
				    if (other.getRoot() != NULL)
                	    CopyTree(other.getRoot(), &_root, NULL);
				    else
					    _root = NULL;
                    _lowest = _findLowest();
                    _highest = _findLowest();
			    }
			    return (*this);
		    }

            void    swap(RedBlackTree &other)
            {
                ft::swap(_root, other._root);
                ft::swap(_lowest, other._lowest);
                ft::swap(_highest, other._highest);
                ft::swap(_leaf, other._leaf);
                ft::swap(_alloc, other._alloc);
                ft::swap(_node_alloc, other._node_alloc);
            }

		    nodePtr getRoot() const
		    {
			    return (this->_root);
		    }

		    nodePtr getLowest() const
		    {
			    return (this->_lowest);
		    }

		    nodePtr getHighest() const
		    {
			    return (this->_highest);
		    }

		    nodePtr getLeaf() const
		    {
			    return (this->_leaf);
		    }

		    nodePtr searchByData(const_reference data) const
		    {
			    nodePtr tmp = _root;
			    if (_root == NULL)
			    {
			    	return (_root);
			    }
			    while (tmp != _leaf)
			    {
			    	if (_value_compare(data, *(tmp->data)))
				    {
				    	if (tmp->left == _leaf)
				    		break;
				    	else
				    		tmp = tmp->left;
				    }
                    else if (CheckEquality(*(tmp->data), data))
			    		break;
			    	else
			    	{
			    		if (tmp->right == _leaf)
			    			break;
			    		else
			    			tmp = tmp->right;
			    	}
			    }
			    return (tmp);
		    }

		    ft::pair<nodePtr, bool> insert(const_reference data)
		    {
			    nodePtr newNode = _createNode(data);
			    if (_root == NULL)
			    {
			    	newNode->color = BLACK;
			    	_root = newNode;
			    }
                else
			    {
		    		nodePtr tmp = searchByData(data);

			    	if (CheckEquality(*(tmp->data), data))
			    	{
			    		_freeNode(newNode);
                        return (ft::make_pair(tmp, false));
			    	}

			    	newNode->parent = tmp;
			    	if (_value_compare(data, *(tmp->data)))
			    		tmp->left = newNode;
			    	else
			    		tmp->right = newNode;
			    	_fixRedRed(newNode);
	    		}
    			if (_highest == _leaf ||
    				_value_compare(*(_highest->data), *(newNode->data)))
    			{
    				_highest = newNode;
	    			_leaf->parent = _highest;
	    		}
	    		if (_lowest == _leaf ||
	    			_value_compare(*(newNode->data), *(_lowest->data)))
	    			_lowest = newNode;
	    		_leaf->color = BLACK;
                return (ft::make_pair(newNode, true));
	    	}

    		bool deleteByData(const_reference data)
    		{
    			if (_root == NULL)
    				return (false);
    			nodePtr tmp = searchByData(data);

    			if (!CheckEquality(data, *(tmp->data)))
    				return (false);
    			deleteNode(tmp);
    			_highest = _findHighest();
                _leaf->parent = _highest;
    			_lowest = _findLowest();    
    			return (true);
    		}

	    	void display(nodePtr node = NULL, std::string indent = "\t",
	    				 bool side = false) const
	    	{
		    	if (_root == NULL)
    			{
    				std::cout << "!-Tree is empty-!" << std::endl;
    				return;
	    		}
    			if (node == NULL)
    				node = _root;
    			if (node != _leaf)
    			{
    				std::cout << indent;
    				if (side)
    				{
	    				std::cout << "R----";
	     				indent += "\t";
	    			}
                    else
	    			{
	    				std::cout << "L----";
	    				indent += "|\t";
	    			}
	    			std::string color = node->color ? "RED" : "BLACK";
	    			std::cout << "Key = " << node->data->first << ", Value = "
	    					  << node->data->second;
	    			std::cout << "(" << color << ")" << std::endl;
	    			display(node->left, indent, false);
	    			display(node->right, indent, true);
	    		}
	    		else
	    		{
	    			std::cout << indent;
	    			if (side)
	    			{
	    				std::cout << "R----";
		    			indent += "\t";
		    		}
                    else
		    		{
		    			std::cout << "L----";
		    			indent += "|\t";
		    		}
	    			std::cout << "LEAF" << std::endl;
	    		}
    		}

            void	clear()
            {
    		    while (_root != NULL)
	    	    	deleteNode(_root);
               _lowest = _leaf;
               _highest = _leaf;
            }

	    private :
		    nodePtr _root;
    		nodePtr _lowest;
    		nodePtr _highest;
    		nodePtr _leaf;
    		compare _comp;
    		value_compare _value_compare;
    		alloc_type _alloc;
    		node_alloc_type _node_alloc;

	    	nodePtr _createLeaf()
	    	{
	    		nodePtr leaf = _node_alloc.allocate(1);
	    		leaf->data = NULL;
	    		leaf->color = BLACK;
	    		leaf->parent = NULL;
	    		leaf->right = NULL;
	    		leaf->left = NULL;
	    		leaf->leaf = leaf;
	    		return (leaf);
    		}

	    	nodePtr _createNode(const_reference val)
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

	    	void    _freeLeaf()
	    	{
	    		_node_alloc.deallocate(_leaf, 1);
	    		_leaf = NULL;
	    	}

    		void    _freeNode(nodePtr node)
    		{
    			_alloc.destroy(node->data);
    			_alloc.deallocate(node->data, 1);
     			_node_alloc.deallocate(node, 1);
    		}

    		nodePtr _getReplaceNode(nodePtr node)
    		{
    			if (node->left != _leaf && node->right != _leaf)
    				return (_getSuccessor(node->right));
    			if (node->left != _leaf)
    				return (node->left);
    			if (node->right != _leaf)
    				return (node->right);
	    		return (_leaf);
    		}

	    	void    deleteNode(nodePtr node)
	    	{
    			nodePtr u = _getReplaceNode(node);

    			bool uvBlack = ((u == _leaf || u->color == BLACK)
                        && node->color == BLACK);
    			nodePtr parent = node->parent;
    			if (u == _leaf)
    			{
    				if (node == _root)
    					_root = NULL;
                    else
    				{
    					if (uvBlack)
    						_fixDoubleBlack(node);
                        else if (node->getSibling() != NULL)
                            node->getSibling()->color = RED;
    					if (node->isLeftChild())
    						parent->left = _leaf;
    					else
    						parent->right = _leaf;
    				}
    				_freeNode(node);
    				return;
	    		}

    			if (node->left == _leaf || node->right == _leaf)
    			{
    				if (node == _root)
    				{
    					ft::swap(node->data, u->data);
    					node->left = _leaf;
    					node->right = _leaf;
    					_freeNode(u);
    				}
                    else
    				{
    					if (node->isLeftChild())
    						parent->left = u;
    					else
    						parent->right = u;
    					_freeNode(node);
    					u->parent = parent;
    					if (uvBlack)
    						_fixDoubleBlack(u);
    					else
    						u->color = BLACK;
    				}
	    			return;
    			}
    			ft::swap(node->data, u->data);
    			deleteNode(u);
    			_leaf->color = BLACK;
    		}


	    	void    _fixRedRed(nodePtr node)
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
	    				_fixRedRed(grandParent);
	    			} else
	     			{
	    				if (parent->isLeftChild())
	    				{
	    					if (node->isLeftChild())
	    					{
	    						ft::swap(parent->color, grandParent->color);
	     					}
                            else
		    				{
    							rotateLeft(parent);
    							ft::swap(node->color, grandParent->color);
    						}
    						rotateRight(grandParent);
	    				}
                        else
    					{
    						if (node->isLeftChild())
    						{
    							rotateRight(parent);
    							ft::swap(node->color, grandParent->color);
    						}
                            else
    						{
    							ft::swap(parent->color, grandParent->color);
    						}
    						rotateLeft(grandParent);
    					}
    				}
    			}
    		}

    		void    _fixDoubleBlack(nodePtr node)
    		{
    			if (node == _root)
    				return;

	    		nodePtr sibling = node->getSibling();
	    		nodePtr parent = node->parent;
	    		if (sibling == NULL)
	    			_fixDoubleBlack(parent);
    			else
    			{
    				if (sibling->color == RED)
    				{
    					parent->color = RED;
    					sibling->color = BLACK;
    					if (sibling->isLeftChild())
    						rotateRight(parent);
    					else
    						rotateLeft(parent);
    
    					_fixDoubleBlack(node);
    				}
                    else
    				{
    					if (sibling->hasRedChild())
    					{
    						if (sibling->left != _leaf &&
    							sibling->left->color == RED)
    						{
       							if (sibling->isLeftChild())
    							{
    								// left left
    								sibling->left->color = sibling->color;
    								sibling->color = parent->color;
    								rotateRight(parent);
    							}
                                else
    							{
    								// right left
    								sibling->left->color = parent->color;
    								rotateRight(sibling);
    								rotateLeft(parent);
    							}
    						}
                            else
    						{
    							if (sibling->isLeftChild())
    							{
    								// left right
    								sibling->right->color = parent->color;
    								rotateLeft(sibling);
    								rotateRight(parent);
    							}
                                else
    							{
    								// right right
    								sibling->right->color = sibling->color;
	    							sibling->color = parent->color;
    								rotateLeft(parent);
    							}
    						}
    						parent->color = BLACK;
    					}
                        else
    					{
    						sibling->color = RED;
    						if (parent->color == BLACK)
    							_fixDoubleBlack(parent);
	     					else
	    						parent->color = BLACK;
	    				}
	    			}
	    		}
	    	}
    
    		nodePtr _getSuccessor(nodePtr node)
    		{
    			nodePtr tmp = node;

	    		while (tmp->left != _leaf)
	    			tmp = tmp->left;
	    		return tmp;
    		}



	    	void rotateLeft(nodePtr node)
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

	    	void rotateRight(nodePtr node)
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


	    	bool CheckEquality(const_reference val1, const_reference val2) const
	    	{
	    		return (!_value_compare(val1, val2) &&
	    				!_value_compare(val2, val1));
	    	}

    		nodePtr _findHighest()
    		{
    			if (_root == NULL)
                    return (_leaf);
    			nodePtr tmp = _root;
    			while (tmp->right != _leaf)
    				tmp = tmp->right;
    			return (tmp);
    		}

    		nodePtr _findLowest()
    		{
    			if (_root == NULL)
                    return (_leaf);
    			nodePtr tmp = _root;
    			while (tmp->left != _leaf)
    				tmp = tmp->left;
    			return (tmp);
    		}

	    	void	CopyTree(nodePtr nodeOther, nodePtr * curNode = NULL, nodePtr parent = NULL)
	    	{
	    		if (nodeOther == nodeOther->leaf)
	    		{
	    			*curNode = _leaf;
	    			(*curNode)->parent = parent;
	    			return;
	    		}
	    		nodePtr newNode = _createNode(*(nodeOther->data));
	    		newNode->color = nodeOther->color;
	    		(*curNode) = newNode;
	    		(*curNode)->parent = parent;
	    		CopyTree(nodeOther->right, &(*curNode)->right, *curNode);
	    		CopyTree(nodeOther->left, &(*curNode)->left, *curNode);
    		}
    };
}

#endif // RB_TREE_HPP
