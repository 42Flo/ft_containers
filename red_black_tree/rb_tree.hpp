#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <iostream>
#include <functional>
#include <memory>
#include <queue>

# include "../tools/tools.hpp"
#include "node.hpp"

namespace ft
{
	template<class T, class Compare = std::less<T>, class ValueCompare = Compare,
			class Alloc = std::allocator<T> >
	class RedBlackTree
	{
        public :
		    typedef T           value_type;
		    typedef T*          pointer;
		    typedef T&          reference;
		    typedef const T&    const_reference;

		    typedef Node<T>*    node_pointer;

		    typedef Compare         compare;
		    typedef ValueCompare    value_compare;

		    typedef Alloc   alloc_type;
		    typedef std::allocator<Node<T> >    node_alloc_type;

            /// Constructors

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
			    deepCopy(other.getRoot(), &_root, NULL);
                _lowest = _findLowest();
                _highest = _findHighest();
		    }

            /// Destructor

		    ~RedBlackTree()
		    {
			    clear();
			    _freeLeaf();
	    	}

            /// Assignation

		    RedBlackTree	&operator=(const RedBlackTree &other)
		    {
			    if (this != &other)
			    {
			    	clear();
                    _lowest = _leaf;
                    _highest = _leaf;
				    if (other.getRoot() != NULL)
                	    deepCopy(other.getRoot(), &_root, NULL);
				    else
					    _root = NULL;
                    _lowest = _findLowest();
                    _highest = _findLowest();
			    }
			    return (*this);
		    }

            /// Node pointer getters

		    node_pointer getRoot() const
		    {
			    return (this->_root);
		    }

		    node_pointer getLowest() const
		    {
			    return (this->_lowest);
		    }

		    node_pointer getHighest() const
		    {
			    return (this->_highest);
		    }

		    node_pointer getLeaf() const
		    {
			    return (this->_leaf);
		    }

		    node_pointer searchByData(const_reference data) const
		    {
			    node_pointer tmp = _root;
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
                    else if (_valuesEquals(*(tmp->data), data))
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

            /// Modifiers

		    ft::pair<node_pointer, bool> insert(const_reference data, node_pointer hint = NULL)
		    {
			    node_pointer newNode = _createNode(data);
			    if (_root == NULL)
			    {
			    	newNode->color = BLACK;
			    	_root = newNode;
			    }
                else
			    {
		    		node_pointer tmp = searchByData(data);

			    	if (_valuesEquals(*(tmp->data), data))
			    	{
			    		_freeNode(newNode);
                        return (ft::make_pair(tmp, false));
			    	}
                    if (hint != NULL)
                        tmp = hint;
			    	newNode->parent = tmp;
			    	if (_value_compare(data, *(tmp->data)))
			    		tmp->left = newNode;
			    	else
			    		tmp->right = newNode;
			    	_fixRedRed(newNode);
	    		}
                _insertUpdateLowHigh(newNode);
	    		_leaf->color = BLACK;
                return (ft::make_pair(newNode, true));
	    	}

    		bool    erase(const_reference data)
    		{
    			if (_root == NULL)
    				return (false);
    			node_pointer tmp = searchByData(data);

    			if (!_valuesEquals(data, *(tmp->data)))
    				return (false);
    			_deleteNode(tmp);
    			_highest = _findHighest();
                _leaf->parent = _highest;
    			_lowest = _findLowest();    
    			return (true);
    		}

            // only swap pointer, no deep copy
            void    swap(RedBlackTree &other)
            {
                ft::swap(_root, other._root);
                ft::swap(_lowest, other._lowest);
                ft::swap(_highest, other._highest);
                ft::swap(_leaf, other._leaf);
                ft::swap(_alloc, other._alloc);
                ft::swap(_node_alloc, other._node_alloc);
            }

            void	clear()
            {
    		    while (_root != NULL)
	    	    	_deleteNode(_root);
               _lowest = _leaf;
               _highest = _leaf;
            }

            // perform a deep copy from x to *curNode (&_root)
	    	void	deepCopy(node_pointer x, node_pointer *curNode = NULL,
                    node_pointer parent = NULL)
	    	{
	    		if (x == x->leaf)
	    		{
	    			*curNode = _leaf;
	    			(*curNode)->parent = parent;
	    			return;
	    		}
	    		node_pointer newNode = _createNode(*(x->data));
	    		newNode->color = x->color;
	    		(*curNode) = newNode;
	    		(*curNode)->parent = parent;
	    		deepCopy(x->right, &(*curNode)->right, *curNode);
	    		deepCopy(x->left, &(*curNode)->left, *curNode);
    		}


	    private :
		    node_pointer    _root;
    		node_pointer    _lowest;
    		node_pointer    _highest;
    		node_pointer    _leaf;
    		compare         _comp;
    		value_compare   _value_compare;
    		alloc_type      _alloc;
    		node_alloc_type _node_alloc;

            /// Node initialisation

	    	node_pointer _createLeaf()
	    	{
	    		node_pointer leaf = _node_alloc.allocate(1);
	    		leaf->data = NULL;
	    		leaf->color = BLACK;
	    		leaf->parent = NULL;
	    		leaf->right = NULL;
	    		leaf->left = NULL;
	    		leaf->leaf = leaf;
	    		return (leaf);
    		}

	    	node_pointer _createNode(const_reference val)
	     	{
    			node_pointer newNode = _node_alloc.allocate(1);
    			newNode->data = _alloc.allocate(1);
    			_alloc.construct(newNode->data, val);
    			newNode->leaf = _leaf;
    			newNode->color = RED;
    			newNode->parent = NULL;
    			newNode->right = _leaf;
    			newNode->left = _leaf;
    			return newNode;
    		}

            /// Node destruction

	    	void    _freeLeaf()
	    	{
	    		_node_alloc.deallocate(_leaf, 1);
	    		_leaf = NULL;
	    	}

    		void    _freeNode(node_pointer node)
    		{
    			_alloc.destroy(node->data);
    			_alloc.deallocate(node->data, 1);
     			_node_alloc.deallocate(node, 1);
    		}

            /// Insert helpers

	    	void    _fixRedRed(node_pointer node)
	    	{
	    		if (node == _root)
	    			node->color = BLACK;
                else if (node->parent->color != BLACK)
    			{
    				if (node->getUncle() != NULL && node->getUncle()->color == RED)
    				{
                        _fixRedRedUncleRed(node); // uncle is red, push red up to grandparent,
	    				_fixRedRed(node->parent->parent); // then recurse from grandparent
	    			}
                    else // uncle is black, perform rotates depending on node position
                        _fixRedRedUncleBlack(node);
                }
    		}

            void    _fixRedRedUncleRed(node_pointer node)
            { 
                node->parent->color = BLACK;
                node->getUncle()->color = BLACK;
                node->parent->parent->color = RED;
            }

            void    _fixRedRedUncleBlack(node_pointer node)
            {
    			node_pointer parent = node->parent;
    			node_pointer grandParent = parent->parent;

                if (parent->isLeftChild())
                {
                    if (node->isLeftChild())
                    {
                        ft::swap(parent->color, grandParent->color);
                    }
                    else
                    {
                        _rotateLeft(parent);
                        ft::swap(node->color, grandParent->color);
                    }
                    _rotateRight(grandParent);
                }
                else
                {
                    if (node->isLeftChild())
                    {
                        _rotateRight(parent);
                        ft::swap(node->color, grandParent->color);
                    }
                    else
                    {
                        ft::swap(parent->color, grandParent->color);
                    }
                    _rotateLeft(grandParent);
                }
            }

            void    _insertUpdateLowHigh(node_pointer newNode)
            {
    			if (_highest == _leaf ||
    				_value_compare(*(_highest->data), *(newNode->data)))
    			{
    				_highest = newNode;
	    			_leaf->parent = _highest;
	    		}
	    		if (_lowest == _leaf ||
	    			_value_compare(*(newNode->data), *(_lowest->data)))
	    			_lowest = newNode;
            }

            /// Erase helpers

	    	void    _deleteNode(node_pointer node)
	    	{
    			node_pointer r = _getReplaceNode(node);
                bool    isDB = _checkDoubleBlack(node, r);

    			if (r == _leaf)
                    _deleteLeaf(node, isDB);
                else if (node->left == _leaf || node->right == _leaf)
                    _deleteOneChild(node, r, isDB);
                else
                {
                    ft::swap(node->data, r->data);
                    _deleteNode(r);
                    _leaf->color = BLACK;
                }
    		}

            void    _deleteLeaf(node_pointer node, bool isDB)
            {
                node_pointer    parent = node->parent;

                if (node == _root)
                    _root = NULL;
                else
                {
                    if (isDB)
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

            void    _deleteOneChild(node_pointer node, node_pointer r, bool isDB)
            {
                node_pointer    parent = node->parent;

                if (node == _root)
                {
                    ft::swap(node->data, r->data);
                    node->left = _leaf;
                    node->right = _leaf;
                    _freeNode(r);
                }
                else
                {
                    if (node->isLeftChild())
                        parent->left = r;
                    else
                        parent->right = r;
                    _freeNode(node);
                    r->parent = parent;
                    if (isDB)
                        _fixDoubleBlack(r);
                    else
                        r->color = BLACK;
                }
            }
    
    		node_pointer _getSuccessor(node_pointer node)
    		{
    			node_pointer tmp = node;

	    		while (tmp->left != _leaf)
	    			tmp = tmp->left;
	    		return tmp;
    		}

    		node_pointer _getReplaceNode(node_pointer node)
    		{
    			if (node->left != _leaf && node->right != _leaf)
    				return (_getSuccessor(node->right));
    			if (node->left != _leaf)
    				return (node->left);
    			if (node->right != _leaf)
    				return (node->right);
	    		return (_leaf);
    		}

            bool    _checkDoubleBlack(node_pointer node, node_pointer r)
            {
                return ((r == _leaf || r->color == BLACK) && node->color == BLACK);
            }

    		void    _fixDoubleBlack(node_pointer node)
    		{
    			if (node == _root)
    				return;

	    		node_pointer sibling = node->getSibling();
	    		node_pointer parent = node->parent;
	    		if (sibling == NULL) // sibling NULL, push DB up
	    			_fixDoubleBlack(parent);
                else if (sibling->color == RED) // sibling red
                {
                    _fixDBSiblingRed(node);
                    _fixDoubleBlack(node);
                }
                else if (sibling->hasRedChild()) // sibling black but has a red child
                    _fixDBSiblingRedChild(node);
                else // sibling black with both childs black
                {
                    sibling->color = RED;
                    if (parent->color == BLACK)
                        _fixDoubleBlack(parent);
                    else
                        parent->color = BLACK;
                }
            }


            void    _fixDBSiblingRed(node_pointer node)
            {
                node->parent->color = RED;
                node->getSibling()->color = BLACK;
                if (node->getSibling()->isLeftChild())
                    _rotateRight(node->parent);
                else
                    _rotateLeft(node->parent);
            }

            // push DB to sibling, do rotates
            void    _fixDBSiblingRedChild(node_pointer node)
            {
                node_pointer    parent = node->parent;
                node_pointer    sibling = node->getSibling();

                if (sibling->left != _leaf && sibling->left->color == RED)
                {
                    if (sibling->isLeftChild())
                    {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        _rotateRight(parent);
                    }
                    else
                    {
                        sibling->left->color = parent->color;
                        _rotateRight(sibling);
                        _rotateLeft(parent);
                    }
                }
                else
                {
                    if (sibling->isLeftChild())
                    {
                        sibling->right->color = parent->color;
                        _rotateLeft(sibling);
                        _rotateRight(parent);
                    }
                    else
                    {
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        _rotateLeft(parent);
                    }
                }
                parent->color = BLACK;
            }

            /// Rotate

	    	void    _rotateLeft(node_pointer node)
	    	{
	    		node_pointer nParent = node->right;

		    	if (node == _root)
		    		_root = nParent;
    			node->moveDown(nParent);
	    		node->right = nParent->left;
		    	if (nParent->left != _leaf)
		    		nParent->left->parent = node;
		    	nParent->left = node;
    		}

	    	void    _rotateRight(node_pointer node)
	    	{
	    		node_pointer nParent = node->left;

		    	if (node == _root)
		    		_root = nParent;
    			node->moveDown(nParent);
    			node->left = nParent->right;
    			if (nParent->right != _leaf)
    				nParent->right->parent = node;
	    		nParent->right = node;
    		}

            /// Tools

	    	bool    _valuesEquals(const_reference val1, const_reference val2) const
	    	{
	    		return (!_value_compare(val1, val2) && !_value_compare(val2, val1));
	    	}

    		node_pointer _findHighest()
    		{
    			if (_root == NULL)
                    return (_leaf);
    			node_pointer tmp = _root;
    			while (tmp->right != _leaf)
    				tmp = tmp->right;
    			return (tmp);
    		}

    		node_pointer _findLowest()
    		{
    			if (_root == NULL)
                    return (_leaf);
    			node_pointer tmp = _root;
    			while (tmp->left != _leaf)
    				tmp = tmp->left;
    			return (tmp);
    		}
    };
}

#endif // RB_TREE_HPP
