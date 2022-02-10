#ifndef TOOLS_HPP
# define TOOLS_HPP

namespace ft
{
    template < class x>
    void    swap(x &a, x &b)
    {
        x   tmp = a;

        a = b;
        b = tmp;
    }
}

#endif // TOOLS_HPP
