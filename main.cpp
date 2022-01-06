#ifndef MAIN_CPP
# define MAIN_CPP

#include <iostream>
#include <vector>

# include "vector.hpp"

int	main()//TODO fix push_back!
{
    {
        ft::vector<int>  v(5, 42);

        std::cout << "*** ft::vector(5, 42) ***" << std::endl;
        std::cout << "size: " << v.size() << std::endl
            << "scapacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
        std::cout << std::endl;

        ft::vector<int>::iterator   begin = v.begin();
        ft::vector<int>::iterator   end = v.end();

        for (ft::vector<int>::iterator it = begin ; it != end ; ++it)
            std::cout << *it << std::endl;
        std::cout << std::endl;
    }
    {
        std::vector<int>    v(5, 42);

        std::cout << "*** std::vector(5, 42) ***" << std::endl;
        std::cout << "size: " << v.size() << std::endl
            << "capacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
        std::cout << std::endl;
        
        std::vector<int>::iterator   begin = v.begin();
        std::vector<int>::iterator   end = v.end();

        for (std::vector<int>::iterator it = begin ; it != end ; ++it)
            std::cout << *it << std::endl;
    }
    
    std::cout << std::endl;

    {
        ft::vector<int>  v;

        std::cout << "*** ft::vector ***" << std::endl;
        std::cout << "size: " << v.size() << std::endl
            << "capacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
        std::cout << std::endl;

        std::cout << "push_back (1, 2, 3, 4)" << std::endl;
        v.push_back(1);
        v.push_back(2);
        v.push_back(5);
        v.push_back(5);
        v.push_back(5);
        v.push_back(5);
        v.push_back(5);
        v.push_back(5);

        std::cout << "size: " << v.size() << std::endl
            << "capacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
 
        ft::vector<int>::iterator   begin = v.begin();
        ft::vector<int>::iterator   end = v.end();

        for (ft::vector<int>::iterator it = begin ; it != end ; ++it)
            std::cout << *it << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << std::endl;

    {
        std::vector<int>  v;

        std::cout << "*** std::vector ***" << std::endl;
        std::cout << "size: " << v.size() << std::endl
            << "capacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
        std::cout << std::endl;

        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);

        std::cout << "size: " << v.size() << std::endl
            << "capacity: " << v.capacity() << std::endl
            << "max size: " << v.max_size() << std::endl;
        
        std::vector<int>::iterator   begin = v.begin();
        std::vector<int>::iterator   end = v.end();

        for (std::vector<int>::iterator it = begin ; it != end ; ++it)
            std::cout << *it << std::endl;
    }

    return (0);
}

#endif // MAIN_CPP
