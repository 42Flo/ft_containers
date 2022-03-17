#include "helper.hpp"

int main()
{
    NS_TARGET::vector<int> vec(10);
    unsigned int        size = vec.size();
    srand(time(NULL));

    // Assign random values using operator[]
    for (unsigned int i = 0 ; i < size ; ++i)
        vec[i] = rand() % MAX_RAND + MIN_RAND;
    printVector(vec);

    // Reverse values
    for (unsigned int i = 0 ; i < size / 2 ; ++i)
    {
        int tmp = vec[size - i - 1];
        vec[size - i - 1] = vec[i];
        vec[i] = tmp;
    }
    printVector(vec);
}
