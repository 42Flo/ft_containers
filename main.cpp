#ifndef MAIN_CPP
# define MAIN_CPP

# include <iostream>

# ifdef STL_EXAMPLE
    #include <vector>
    #include <stack>
    #include <algorithm>
    namespace ft = std;
# else
    #include "vector.hpp"
    #include "stack.hpp"
    #include "map.hpp"
# endif

int	main()
{
    /*
    ft::vector<int> a;
    ft::vector<int> b(10, 21);

    for (unsigned int i = 1 ; i < 6 ; ++i)
        a.push_back(i);
    a.insert(a.end() - 3, 3, 42);
    a.erase(a.begin() + 2, a.end() - 2);

    std::cout << "vector a" << std::endl;
    for (ft::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;
    std::cout << "vector b" << std::endl;
    for (ft::vector<int>::iterator it = b.begin() ; it != b.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;

    std::cout << "swap called" << std::endl;
    a.swap(b);
    std::cout << std::endl;

    std::cout << "vector a" << std::endl;
    for (ft::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;
    std::cout << "vector b" << std::endl;
    for (ft::vector<int>::iterator it = b.begin() ; it != b.end() ; ++it)
        std::cout << *it << std::endl;
*/
    RBTree<int>  tree;

    tree.insert(10);
    tree.insert(18);
    tree.insert(7);
    tree.insert(15);
    tree.insert(16);
    tree.insert(30);
    tree.insert(25);
    tree.insert(40);
    tree.insert(60);
    tree.insert(2);
    tree.insert(1);
    tree.insert(70);
    tree.display(tree.getRoot(), "", true);

    return (0);
}

#endif // MAIN_CPP
