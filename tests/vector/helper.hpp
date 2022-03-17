#ifndef HELPER_HPP
# define HELPER_HPP

# include <stdlib.h>
# include <time.h>

# include "../test.hpp"

# if NS_TARGET == std
    #include <vector>
# else
    #include "vector.hpp"
# endif

# ifndef TYPE
    #define TYPE int
# endif

# define MIN_RAND -100
# define MAX_RAND 100

template <class T>
void    printSize(NS_TARGET::vector<T> &vec)//TODO same
{
    std::cout << "size: " << vec.size() << std::endl;
    std::cout << "capacity: " << vec.capacity() << std::endl;
    std::cout << "max_size: " << vec.max_size() << std::endl;
}

template <class T>
void    printVector(NS_TARGET::vector<T> &vec)//TODO const vec, const it
{
    typename NS_TARGET::vector<T>::iterator    it = vec.begin();
    typename NS_TARGET::vector<T>::iterator    ite = vec.end();
    for ( ; it != ite ; ++it)
        std::cout << *it << std::endl;
    
    printSize(vec);
}


#endif // HELPER_HPP
