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
    struct is_integral<char16_t> : public integral_constant<bool, true>{};

    template <>
    struct is_integral<char32_t> : public integral_constant<bool, true>{};

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

    // equal:               compare the elements in the range [first1, last1]
    //                      with those in the range beginning at first2,
    //                      return true if the element in both range match

    template < class InputIterator1, class InputIterator2>
    bool    equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2)
    {
        while (first1 != last1)
        {
            if (*first1 != *first2)
                return (false);
            ++first1;
            ++first2;
        }
        return (true);
    }

    // same with custom comparison function
    template < class InputIterator1, class InputIterator2, class BinaryPredicate >
    bool    equal(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2, BinaryPredicate pred)
    {
        while (first1 != last1)
        {
            if (pred(*first1, *first2) == false)
                return (false);
            ++first1;
            ++first2;
        }
        return (true);
    }

    // lexicographical_compare:     return true if the range [first1, last1]
    //                              compares lexicographically less than
    //                              the range [first2, last2]

    template < class InputIterator1, class InputIterator2 >
    bool    lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2)
    {
        while (first1 != last1)
        {
            if (first2 == last2 || *first2 < *first1)
                return (false);
            else if (*first1 < *first2)
                return (true);
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }

    // same with custom comparison function
    template < class InputIterator1, class InputIterator2, class Compare >
    bool    lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2, Compare comp)
    {
        while (first1 != last1)
        {
            if (first2 == last2 || comp(*first2, *first1) == true)
                return (false);
            else if (comp(*first1, *first2) == true)
                return (true);
        }
        return (first2 != last2);
    }

    //TODO pair, make_pair
}

#endif // UTILS_HPP
