#ifndef CONSTRUCTORS_CPP
# define CONSTRUCTORS_CPP

# include "helper.hpp"

int main()
{//TODO tests with custom class
    TARGET::vector<int> intVec;
    TARGET::vector<int> intVec2(10, 42);

    printVector(intVec);
    printVector(intVec2);
}

#endif // CONSTRUCTORS_CPP
