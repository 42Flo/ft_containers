#ifndef MAIN_CPP
# define MAIN_CPP

# include <iostream>

# ifdef STL_EXAMPLE
    #include <vector>
    #include <algorithm>
    namespace ft = std;
# else
    #include "vector.hpp"
# endif

int	main()
{
    ft::vector<int>  v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.insert(v.end() - 3, 3, 42);

    std::cout << "size: " << v.size() << std::endl << 
        "capacity: " << v.capacity() << std::endl;

    ft::vector<int>::iterator   it = v.begin();

    for ( ; it != v.end() ; ++it)
        std::cout << *it << std::endl;

    return (0);
}

#endif // MAIN_CPP
