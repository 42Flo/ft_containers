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

    // integral_constant:   provide types for is_integral

    template < class T, T v >
    struct integral_constant
    {
        typedef T   value_type;
        typedef integral_constant<T, v> type;
    };

    // is_integral:         set integral_constant::type to true if overloaded
    //                      by an integral type, otherwise type is false

    template < class T >
    struct is_integral : public integral_constant<bool, false>{};

    template <>
    struct is_integral<bool> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<char> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<wchar_t> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<signed char> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<short> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<int> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<long> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<long long> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<unsigned char> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<unsigned short> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<unsigned int> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<unsigned long> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<unsigned long long> : public integral_constant<bool, true>{};
}

#endif // UTILS_HPP
