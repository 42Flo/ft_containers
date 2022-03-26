#include <list>
#include <deque>
#include "helper.hpp"

template < class T, class Container = ft::vector<T> >
void    push_pop_top(NS_TARGET::stack<T, Container> &st)
{
    for (unsigned int i = 0 ; i < 10 ; ++i)
    {
        st.push(i);
        std::cout << st.top() << std::endl;
    }
    printSize(st);
    for (unsigned int i = 0 ; i < 5 ; ++i)
    {
        st.pop();
        std::cout << st.top() << std::endl;
    }
    printSize(st);
}

int main()
{
    NS_TARGET::stack<TYPE>  st;
    NS_TARGET::stack<int>   intSt;
    NS_TARGET::stack<int, std::list<int>>   intSt2;

    printSize(st);
    std::cout << st.empty() << std::endl;
    st.push(TYPE());
    std::cout << st.empty() << std::endl;
    printSize(st);

    push_pop_top(intSt);
    push_pop_top(intSt2);

    //TODO tests for relationnal operators
}
