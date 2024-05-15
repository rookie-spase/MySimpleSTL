#pragma once
#ifndef MYSIMPLESTL_ALGOBASE_H_
#define MYSIMPLESTL_ALGOBASE_H_


#include <cstring>

#include "iterator.h"
#include "util.h"

namespace mySimpleSTL
{

    // max
    template <class T>
    const T& max(const T& lhs, const T& rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }
    template <class T, class Compare>
    const T& max(const T& lhs, const T& rhs, Compare e)
    {
        return e(lhs, rhs) ? rhs : lhs;
    }

    // min
    template <class T>
    const T& min(const T& lhs, const T& rhs)
    {
        return rhs < lhs ? rhs : lhs;
    }
    template <class T, class Compare>
    const T& min(const T& lhs, const T& rhs, Compare e)
    {
        return e(rhs, lhs) ? rhs : lhs;
    }

    // iter_swap
    template <class FIter1, class FIter2>
    void iter_swap(FIter1 lhs, FIter2 rhs)
    {
        mySimpleSTL::swap(*lhs, *rhs);
    }

    // copy
    // 这里的copy具有2个版本，
    //      针对input_iterator_tag，针对random_access_iterator_tag,
    template <class InputIter, class OutputIter>
    OutputIter
        unchecked_copy_(InputIter bg, InputIter ed, OutputIter start,mySimpleSTL::input_iterator_tag)
    {
        while (bg != ed) {
            *(--bg) = *(--ed);
        }
        return bg;
    }

    // ramdom_access_iterator_tag 版本
    template <class randomIter, class OutputIter>
    OutputIter
        unchecked_copy_(randomIter bg, randomIter ed, OutputIter start, mySimpleSTL::random_access_iterator_tag)
    {
        for (auto n = ed - bg; n > 0;--n) {
            *bg++ = *start++;
        };
        return start;
    };

    template <class InputIter, class OutputIter>
    OutputIter
        unchecked_copy(InputIter bg, InputIter ed, OutputIter start)
    {
        return unchecked_copy_(bg, ed, start, iterator_category(bg));
    };
/*
    template<typename source,typename target>
    typename std::enable_if<
        std::is_same<typename std::remove_const<source>::type,target>::value&&
        std::is_trivially_copy_assignable<target>::value
        ,target*>::type
        unchecked_copy(source bg, source ed,target bg_trg) {                // 这段函数的这里，没有写成指针，导致uninitialized使用的时候找不到

        const auto n = static_cast<size_t>(ed - bg);

        if (n > 0)
            std::memmove(bg_trg, bg, n);
        return bg_trg + n;

        /*
        * 这个函数的模板参数需要解释的时enable_if的地方,
        * 这个enable_if不是在模板里面，而是在返回值类型里面
        * 首先，传入的source默认是const的，写入的地方不能是const的
        * 如果去掉const的source和targrt是一个类型,并且 target是可以被赋值的
        * 那么就有返回值，返回值为target*
       
    }
    */ 
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
        unchecked_copy(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
            std::memmove(result, first, n * sizeof(Up));
        return result + n;
    }


    template<typename source, typename target>
    target copy(source bg, source ed, target bg_trg) {
        return unchecked_copy(bg, ed, bg_trg);
    };

 
    // copy_backward
    // 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
    // 将后面的元素拷贝到前面去
    template<class source, class target>
    target
        unchecked_copy_backward_(source bg, source ed, target bg_trg, mySimpleSTL::bidirectional_iterator_tag)
    {
        while (bg != ed) {
            *(--bg_trg) = *(--ed);
        }
        return bg_trg;
    };
    template<class source, class target>
    target
        unchecked_copy_backward_(source bg, source ed, target bg_trg, mySimpleSTL::random_access_iterator_tag)
    {
        auto len = ed - bg;
        while (len--) {
            *(--bg_trg) = *(--ed);
        }
        return bg_trg;
    };
    /*   
    示例：
    int arr1[] = { 1, 2, 3, 4, 5 };
    int arr2[5] = { 0 };

    mySimpleSTL::unchecked_copy_backward_(std::begin(arr1), std::end(arr1), std::end(arr2), mySimpleSTL::random_access_iterator_tag{});

    for (auto i : arr2) {
        std::cout << i << ' ';
    }
    结果是1 2 3 4 5
    我还以为我实现错了，没事，正确的顺序就是正向的
    template <class RandomIter1, class BidirectionalIter2>
    BidirectionalIter2
        unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
            BidirectionalIter2 result, mySimpleSTL::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n)
            *--result = *--last;
        return result;
    }
    */
    template<class source, class target>
    target
        unchecked_copy_backward(source bg, source ed, target bg_trg)
    {
        return unchecked_copy_backward_(bg,ed,bg_trg,mySimpleSTL::iterator_category(bg));
    };

    /*
    template<typename source, typename target>
    typename std::enable_if<
        std::is_same<typename std::remove_const<source>::type, target>::value&&
        std::is_trivially_copy_assignable<target>::value
        , target*>::type
        unchecked_copy_backward(source bg, source ed, target bg_trg)
    {
        const auto n = static_cast<size_t>(ed - bg);
        if (n != 0)
        {
            bg_trg -= n;
            std::memmove(bg_trg, bg, n * sizeof(target));
        }
        return bg_trg;
    };*/
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
        unchecked_copy_backward(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }
    template<class source, class target>
    target
        copy_backward(source bg, source ed, target bg_trg)
    {
        return unchecked_copy_backward(bg, ed, bg_trg);
    };
    
    // copy_if，
    // 满足条件的话，就拷贝过去
    template<typename source, typename target, typename condition>
    target* copy_if(source bg, source ed, target* bg_trg, condition con) {
        while (bg != ed && bg_trg != nullptr) {
            if (con(*bg)) {
                *bg_trg++ = *bg;
            }
            ++bg;
        }
        return bg_trg;
    }
    /*
    * 示例
    *     
    int arr1[] = { 1, 2, 3, 4, 5 };
    int arr2[5] = { 0 };

    mySimpleSTL::copy_if(std::begin(arr1), std::end(arr1), std::begin(arr2), [](int e) {
        return e % 2 == 0;
        });

    for (auto i : arr2) {
        std::cout << i << ' ';
    }
    */


    /*
    * copy_n 将一个区间拷贝到另一个区间，返回pair指向头尾区间的元素
    * 有2个版本：input_iterator，random_iterator
    * 
    */

    template<typename source, typename Size, typename target>
    mySimpleSTL::pair<source, target> copy_n_(source first, Size count, target result,
        mySimpleSTL::input_iterator_tag)
    {
        for (Size i = 0; i < count; ++i) {
            *result++ = *first++;
        }
        return mySimpleSTL::pair<source, target>(first, result);
    };/*
    template<typename source, typename Size, typename target>
    mySimpleSTL::pair<source, target> copy_n_(source first, Size count, target result,
        mySimpleSTL::random_access_iterator_tag)
    {
        for (Size i = 0; i < count; ++i) {
            *result++ = *first++;
        }
        return mySimpleSTL::pair<source, target>(first, result);
    };*/

    template <class RandomIter, class Size, class OutputIter>
    mySimpleSTL::pair<RandomIter, OutputIter>
        unchecked_copy_n(RandomIter first, Size n, OutputIter result,
            mySimpleSTL::random_access_iterator_tag)
    {
        auto last = first + n;
        return mySimpleSTL::pair<RandomIter, OutputIter>(last, mySimpleSTL::copy(first, last, result));
    }

    template<typename source, typename Size, typename target>
    mySimpleSTL::pair<source, target> copy_n(source first, Size count, target result)
    {
        return copy_n_(first,count,result,mySimpleSTL::iterator_category(first));
    };


    /*
    * 示例：
    *     
    int arr1[5] = { 1,2,3,4,5 };
    int arr2[5] = { 0 };
    mySimpleSTL::copy_n_(arr1, 5, arr2, mySimpleSTL::random_access_iterator_tag{});
    for (int i = 0; i < 5; ++i) {
        std::cout << arr2[i] << " ";
    };
    std::cout << std::endl;
    */


    // move
    // 将一段数移动到另一段
    template<class InputIt, class OutputIt>
    OutputIt unchecked_move_(InputIt first, InputIt last, OutputIt result, mySimpleSTL::input_iterator_tag) {
        while (first != last) {
            *result = std::move(*first);
            ++result;
            ++first;
        }
        return result;
    };

    template<class InputIt, class OutputIt>
    OutputIt unchecked_move_(InputIt first, InputIt last, OutputIt result, mySimpleSTL::random_access_iterator_tag) {
        for (; first != last; ++first, ++result) {
            *result = std::move(*first);
        }
        return result;
    };
    
    template<class InputIt, class OutputIt>
    OutputIt unchecked_move(InputIt first, InputIt last, OutputIt result) {
        return unchecked_move_(first,last,mySimpleSTL::iterator_category(first));
    };
    template<typename source, typename target>
    typename std::enable_if<
        std::is_same<typename std::remove_const<source>::type, target>::value&&
        std::is_trivially_copy_assignable<target>::value
        , target*>::type
     unchecked_move(source first, source last, target result) {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
            std::memmove(result, first, n * sizeof(target));
        return result + n;
    };

    template <class InputIter, class OutputIter>
    OutputIter move(InputIter first, InputIter last, OutputIter result)
    {
        return mySimpleSTL::unchecked_move(first, last, result);
    }


/*****************************************************************************************/
// move_backward
// 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
// 写到这里大体都是相似，的后面的没必要了
/*****************************************************************************************/

    
    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward_(BidirectionalIter1 first, BidirectionalIter1 last, 
        BidirectionalIter2 result, mySimpleSTL::bidirectional_iterator_tag) {
        while (first != last) {
            *(--result) = std::move(*(--last));
        }
        return result;
    };
    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward_(BidirectionalIter1 first, BidirectionalIter1 last,
        BidirectionalIter2 result, mySimpleSTL::random_access_iterator_tag) {
        for (; first != last; --last, --result) {
            *(result - 1) = std::move(*(last - 1));
        }
        return result;
    };
    template<class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
        BidirectionalIter2 result) {
        return unchecked_move_backward_(first, last, result, mySimpleSTL::iterator_category(first));
    };
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
        unchecked_move_backward(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
        move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return unchecked_move_backward(first, last, result);
    }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
* equal
* 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    template<typename qj1,typename qj2>
    bool compare_qj(qj1 bg,qj1 end,qj2 start) {
        while (bg++ != end) {
            if (*bg != *start++) {
                return false;
            }
        }
        return true;
    }

    template<typename qj1, typename qj2,typename Comp>
    bool compare_qj(qj1 bg, qj1 end, qj2 start) {
        while (bg++ != end) {
            if (Comp(bg++,start++)) {
                return false;
            }
        }
        return true;
    }

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
    template<typename Iter,typename T>
    Iter unchecked_fill_n(Iter bg, size_t n, T val) {
        while (n--) {
            *bg++ = val;
        }
        return bg;
    };
    template <class source, class Size, class T>
    typename std::enable_if<
        std::is_integral<source>::value && sizeof(source) == 1 &&           // 传入的类型是一个1字节的integral时
        !std::is_same<source, bool>::value&&                            // 并非bool类型
        std::is_integral<T>::value && sizeof(T) == 1,
        source*>::type
       unchecked_fill_n(source bg, size_t n, T val)
    {
        if (n > 0)
            std::memset(bg, (unsigned char)val, (size_t)(n));
        
        return bg + n;
    };


    template <class OutputIter, class Size, class T>
    OutputIter fill_n(OutputIter first, Size n, const T& value)
    {
        return unchecked_fill_n(first, n, value);
    }

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/

    template<typename Iter,typename T>
    void fill_(Iter bg, Iter ed, T val, mySimpleSTL::forward_iterator_tag) {
        while (bg++ != ed) {
            *bg = val;
        }
    };
    template<typename Iter, typename T>
    void fill_(Iter bg, Iter ed, T val, mySimpleSTL::random_access_iterator_tag) {
        fill_n(bg, ed - bg, val);
    };
    template<typename Iter, typename T>
    void fill(Iter bg, Iter ed, T val) {
        fill_(bg, ed, val,mySimpleSTL::iterator_category(bg));
    };

/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/

    template<typename xl1,typename xl2>
    bool lexicographical_compare(xl1 start1, xl1 last1, xl2 start2, xl2 last2) 
    {
        while (start1 != last1 && start2 != last2) {
            if (*start1 < *start2)return true;
            else if (*start1 > *start2)return false;
            start1++; start2++;
       }
        return (start1 == last1) && (start2 != last2);
    };
    template<typename xl1, typename xl2,typename Comp>
    bool lexicographical_compare(xl1 start1, xl1 last1, xl2 start2, xl2 last2, Comp e)
    {
        while (start1 != last1 && start2 != last2) {
            if (e(start1,start2))return true;
            else if (e(start1, start2))return false;
            start1++; start2++;
        }
        return (start1 == last1) && (start2 != last2);
    };
    bool lexicographical_compare(const unsigned char* first1,
        const unsigned char* last1,
        const unsigned char* first2,
        const unsigned char* last2)
    {
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        // 先比较相同长度的部分
        const auto result = std::memcmp(first1, first2, mySimpleSTL::min(len1, len2));
        // 若相等，长度较长的比较大
        return result != 0 ? result < 0 : len1 < len2;
    }



/*****************************************************************************************/
// mismatch
// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
/*****************************************************************************************/

   template<typename xl1,typename xl2>
   mySimpleSTL::pair<xl1, xl2> mismatch(xl1 start1, xl1 last1, xl2 start2, xl2 last2) {
       while (start1 != last1 && start2 != last2) {
           if (*start1++ != *start2++) {
               return mySimpleSTL::pair<xl1, xl2>(start1,start2);
           }
       }
       return  mySimpleSTL::pair<xl1, xl2>();
   }
   template<typename xl1, typename xl2,typename Comp>
   mySimpleSTL::pair<xl1, xl2> mismatch(xl1 start1, xl1 last1, xl2 start2, xl2 last2, Comp e) {
       while (start1 != last1 && start2 != last2) {
           if (e(start1++,start2++)) {
               return mySimpleSTL::pair<xl1, xl2>(start1, start2);
           }
       }
       return  mySimpleSTL::pair<xl1, xl2>();
   }
    
};


#endif