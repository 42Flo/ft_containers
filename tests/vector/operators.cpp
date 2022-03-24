#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(10, TYPE());
    NS_TARGET::vector<TYPE> vec2(8, TYPE());
    NS_TARGET::vector<TYPE> vec3(10, TYPE());
    NS_TARGET::vector<TYPE> vec4;

    std::cout << (vec == vec2) << std::endl;
    std::cout << (vec != vec2) << std::endl;
    std::cout << (vec < vec2) << std::endl;
    std::cout << (vec <= vec2) << std::endl;
    std::cout << (vec <= vec3) << std::endl;
    std::cout << (vec >= vec2) << std::endl;
    std::cout << (vec >= vec3) << std::endl;

    vec4 = vec;
    printVector(vec4);
    vec4 = vec2;
    printVector(vec4);
    vec4 = vec3;
    printVector(vec4);
}
