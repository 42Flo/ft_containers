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

    // pair:    couple together a pair of values (T1, T2)

    template < class T1, class T2 >
    struct pair
    {
        typedef T1  first_type;
        typedef T2  second_type;

        first_type  first;
        second_type second;

        // Default constructor
        pair() : first(0), second(0){}

        // Copy constructor
        template < class U, class V>
        pair(const ft::pair<U, V> &pr) : first(pr.first), second(pr.second){}

        // Init constructor
        pair(const first_type &a, const second_type &b) : first(a), second(b){}

        // Assignation operator
        pair    &operator=(const ft::pair<T1, T2> &pr)
        {
            first = pr.first;
            second = pr.second;
        }

        // Relational operators
        friend bool operator==(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (l.first == r.first && l.second == r.second);
        }
        friend bool operator!=(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (!(l == r));
        }
        friend bool operator<(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (l.first < r.first || (!(r.first < l.first) &&
                        l.second < r.second));
        }
        friend bool operator<=(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (!(l < r));
        }
        friend bool operator>(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (r < l);
        }
        friend bool operator>=(const ft::pair<T1, T2> &l,
                const ft::pair<T1, T2> &r)
        {
            return (!(r < l));
        }
    };

    // make_pair:   construct a pair object

    template < class T1, class T2>
    ft::pair<T1, T2>    make_pair(T1 x, T2 y)
    {
        return (ft::pair<T1, T2>(x, y));
    }

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
}

#endif // TOOLS_HPP
