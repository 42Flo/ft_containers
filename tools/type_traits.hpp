#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft
{
    // enable_if:           type T is enabled if Cond is true,
    //                      otherwise type is not defined

    template < bool Cond, class T = void >
    struct enable_if{};

    template < class T >
    struct enable_if<true, T>
    {
        typedef T type;
    };

    // is_integral:         set integral_constant::type to true if overloaded
    //                      by an integral type, otherwise type is false

    template < class T, class = void>
    struct is_integral { const static bool value = false;};

    template <>
    struct is_integral<bool> { const static bool value = true;};

    template <>
    struct is_integral<char> { const static bool value = true;};

    template <>
    struct is_integral<wchar_t> { const static bool value = true;};

    template <>
    struct is_integral<short> { const static bool value = true;};

    template <>
    struct is_integral<int> { const static bool value = true;};

    template <>
    struct is_integral<long> { const static bool value = true;};

    template <>
    struct is_integral<long long> { const static bool value = true;};

    template <>
    struct is_integral<unsigned char> { const static bool value = true;};

    template <>
    struct is_integral<unsigned short> { const static bool value = true;};

    template <>
    struct is_integral<unsigned int> { const static bool value = true;};

    template <>
    struct is_integral<unsigned long> { const static bool value = true;};

    template <>
    struct is_integral<unsigned long long> { const static bool value = true;};
}

#endif // UTILS_HPP
