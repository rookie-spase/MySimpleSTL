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
    // �����copy����2���汾��
    //      ���input_iterator_tag�����random_access_iterator_tag,
    template <class InputIter, class OutputIter>
    OutputIter
        unchecked_copy_(InputIter bg, InputIter ed, OutputIter start,mySimpleSTL::input_iterator_tag)
    {
        while (bg != ed) {
            *(--bg) = *(--ed);
        }
        return bg;
    }

    // ramdom_access_iterator_tag �汾
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
        unchecked_copy(source bg, source ed,target bg_trg) {                // ��κ��������û��д��ָ�룬����uninitializedʹ�õ�ʱ���Ҳ���

        const auto n = static_cast<size_t>(ed - bg);

        if (n > 0)
            std::memmove(bg_trg, bg, n);
        return bg_trg + n;

        /*
        * ���������ģ�������Ҫ���͵�ʱenable_if�ĵط�,
        * ���enable_if������ģ�����棬�����ڷ���ֵ��������
        * ���ȣ������sourceĬ����const�ģ�д��ĵط�������const��
        * ���ȥ��const��source��targrt��һ������,���� target�ǿ��Ա���ֵ��
        * ��ô���з���ֵ������ֵΪtarget*
       
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
    // �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
    // �������Ԫ�ؿ�����ǰ��ȥ
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
    ʾ����
    int arr1[] = { 1, 2, 3, 4, 5 };
    int arr2[5] = { 0 };

    mySimpleSTL::unchecked_copy_backward_(std::begin(arr1), std::end(arr1), std::end(arr2), mySimpleSTL::random_access_iterator_tag{});

    for (auto i : arr2) {
        std::cout << i << ' ';
    }
    �����1 2 3 4 5
    �һ���Ϊ��ʵ�ִ��ˣ�û�£���ȷ��˳����������
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
    
    // copy_if��
    // ���������Ļ����Ϳ�����ȥ
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
    * ʾ��
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
    * copy_n ��һ�����俽������һ�����䣬����pairָ��ͷβ�����Ԫ��
    * ��2���汾��input_iterator��random_iterator
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
    * ʾ����
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
    // ��һ�����ƶ�����һ��
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
// �� [first, last)�����ڵ�Ԫ���ƶ��� [result - (last - first), result)��
// д��������嶼�����ƣ��ĺ����û��Ҫ��
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
* �Ƚϵ�һ������ [first, last)�����ϵ�Ԫ��ֵ�Ƿ�͵ڶ��������
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
// �� first λ�ÿ�ʼ��� n ��ֵ
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
        std::is_integral<source>::value && sizeof(source) == 1 &&           // �����������һ��1�ֽڵ�integralʱ
        !std::is_same<source, bool>::value&&                            // ����bool����
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
// Ϊ [first, last)�����ڵ�����Ԫ�������ֵ
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
// ���ֵ������ж��������н��бȽϣ�����ĳ��λ�÷��ֵ�һ�鲻���Ԫ��ʱ�������м��������
// (1)�����һ���е�Ԫ�ؽ�С������ true �����򷵻� false
// (2)������� last1 ����δ���� last2 ���� true
// (3)������� last2 ����δ���� last1 ���� false
// (4)���ͬʱ���� last1 �� last2 ���� false
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
        // �ȱȽ���ͬ���ȵĲ���
        const auto result = std::memcmp(first1, first2, mySimpleSTL::min(len1, len2));
        // ����ȣ����Ƚϳ��ıȽϴ�
        return result != 0 ? result < 0 : len1 < len2;
    }



/*****************************************************************************************/
// mismatch
// ƽ�бȽ��������У��ҵ���һ��ʧ���Ԫ�أ�����һ�Ե��������ֱ�ָ������������ʧ���Ԫ��
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