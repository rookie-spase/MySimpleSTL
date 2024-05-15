#pragma once
#ifndef MYSIMPLESTL_UTIL_H
#define MYSIMPLESTL_UTIL_H
#include <cstddef>
#include "type_traits.h"

namespace mySimpleSTL {
	template<typename T>
	typename std::remove_reference<T>::type&& move(T&& val) noexcept {
		return static_cast<std::remove_reference<T>::type&&>(val);
	};
	/*
	* 解释：
	* remove_reference<T>可以将int&变为int,而T&&既可以接受左值也可以接受右值
	* remove_reference<T>变为int之后加了&&，变为了右值
	* 
	* 示例：
	* int x = 42;int&& y = mySimpleSTL::move(x);std::cout << "Is y a rvalue reference? " << std::is_rvalue_reference<decltype(y)>::value << std::endl;
	* 
	*/



	template <class T>
	T&& forward(typename std::remove_reference<T>::type& val) noexcept		//int&
	{
		return static_cast<T&&>(val);
	};
	template <class T>
	T&& forward(typename std::remove_reference<T>::type&& val) noexcept		// int&&
	{
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(val);
	}
	/*
	* 重点：
	* remove_refrence解析为int，那么在forward<int>的时候，就会出现2个函数
	* forward(int&)
	* forward(int&&)
	* 所以2个函数并不冲突！
	* 
	* 解释：
	* 模拟完美转发，意思就是传经来的是int&那么就返回int&；
	* 若传经来的是int&& ， 那么返回int&&
	* 如果传入的int&，那么arg是int&
	* 而static_cast<T&&>中的T&&是可以接受左值也可以接收右值的所以static_cast<T&&>会得到int&
	* T&& 也可以接受左值，所以返回值为int&
	* 
	* 
	* 示例：
	* 
	* void f(int& e) {cout << "left value" << endl;};
	* void f(int&& e) {cout << "right value" << endl;}
	* int main() {int e = 12;f(mySimpleSTL::forward<int>(e));return 0;}
	* 
	*/



	template<typename T>
	void swap(T& lv, T& rv) {
		auto tmp(mySimpleSTL::move(lv));
		lv = mySimpleSTL::move(rv);
		rv = mySimpleSTL::move(tmp);
	}
	/*
	* 解释：
	* 传入的参数不能是T&&，否则实参改变不了
	* 
	* 示例：
	* int w = 3,e =2;swap(w,e);cout << w;
	*/

	template<typename forwardIterator_1, typename forwardIterator_2>
	forwardIterator_2 swap_range(forwardIterator_1 begin, forwardIterator_1 end, forwardIterator_2 target) {
		while (begin != end) 
			mySimpleSTL::swap(*begin++,*target++);

		return target;
	}
	/*
	* 解释：
	* 实现2个序列的交换
	* 
	* 示例：
	*  std::vector<int> a{ 1, 2, 3 };std::vector<int> b{ 4, 5, 6 }; auto iter = mySimpleSTL::swap_range(a.begin(), a.end(), b.begin()); for (const auto& x : b) { std::cout << x << " "; } // 输出：1 2 3
	*/
	
	template<typename T,size_t N>
	void swap(T(&a), T(&b)) {			// 对数组的引用，而不是指针
		mySimpleSTL::swap(a,a+N,b);
	}
	/*
	* 示例：
	*     int arr1[] = { 1, 2, 3 };
    int arr2[] = { 4, 5, 6 };

    cout << "Before swapping:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << ", "
            << "arr2[" << i << "] = " << arr2[i] << endl;
    }

    // 调用 swap 函数来交换数组的值
    swap<int, 3>(arr1, arr2);

    cout << "\nAfter swapping:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << ", "
            << "arr2[" << i << "] = " << arr2[i] << endl;
    }
	*/

	// 接下来是pair的实现
	
	template<typename T1,typename T2>
	class pair {
	public:
		T1 first;
		T2 second;



		// ------------------------构造函数----------------------------
		// 默认构造函数
		template<typename Other1 = T1, typename Other2 = T2,
			typename = typename std::enable_if<				// enable_if检查是为1，是的话启用构造函数，不是的话禁用构造函数
			std::is_default_constructible<Other1>::value&&		// 检查Other1是否有默认构造函数，如果有那么value就是true
			std::is_default_constructible<Other2>::value
			, void			>::type>
		pair() :first(), second() {}

		template <class Other1 = T1, class Other2 = T2,
			typename std::enable_if<
			std::is_copy_constructible<Other1>::value&&
			std::is_copy_constructible<Other2>::value&&			// 检查Other是否具有拷贝构造函数
			std::is_convertible<const Other1&, T1>::value&&		// 检查T1是否可以转换为const T1&
			std::is_convertible<const Other2&, T2>::value, int>::type = 0>
		constexpr pair(const T1& a, const T2& b)
			: first(a), second(b){}

		pair(const pair& rv) = default;
		pair(pair&& rv) = default;
		// ------------------------构造函数-------------------------------


		// 拷贝赋值
		pair& operator = (const pair& rv) {
			if (this != &rv) {
				first = rv.first;
				second = rv.second;
			}
			return *this;
		}
		// 移动赋值
		pair& operator = (pair&& rv) noexcept {
			if (this != &rv) {
				first = mySimpleSTL::move(rv.first);
				second = mySimpleSTL::move(rv.second);
			}
			return *this;
		}

		// 模板拷贝赋值
		template <typename Other1, typename Other2,
			typename = std::enable_if_t<std::is_convertible_v<Other1, T1>&& std::is_convertible_v<Other2, T2>>>
		pair& operator = (const pair<Other1, Other2>& rv) {
			this->first = rv.first;
			this->second = rv.second;

			return *this;
		};

		// 模板移动赋值
		template <typename Other1, typename Other2,
			typename = std::enable_if_t<std::is_convertible_v<Other1, T1>&& std::is_convertible_v<Other2, T2>>>
		pair&& operator = (const pair<Other1, Other2>&& rv) {
			this->first = mySimpleSTL::move(rv.first);
			this->second = mySimpleSTL::move(rv.second);

			return mySimpleSTL::move(*this);
		};

		// 析构函数
		~pair() = default;

		// 交换函数
		void swap(pair& other)
		{
			if (this != &other)
			{
				mySimpleSTL::swap(first, other.first);
				mySimpleSTL::swap(second, other.second);
			}
		}



	};

	// == < != > <=  >=  swap_pair  make_pair

	template<typename T1, typename T2>
	bool operator == (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return (lv.first == rv.first) && (lv.second == rv.second);
	}

	template<typename T1, typename T2>
	bool operator != (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return !(lv == rv);
	}

	template<typename T1, typename T2>
	bool operator <  (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return (lv.first < rv.first) || (lv.first == rv.first && lv.second < rv.second);
	}

	template<typename T1, typename T2>
	bool operator > (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return rv < lv;
	}

	template<typename T1, typename T2>
	bool operator <= (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return !(rv < lv);
	}

	template<typename T1, typename T2>
	bool operator >= (const pair<T1, T2>& lv, const pair<T1, T2>& rv) {
		return !(lv < rv);
	}

	template<typename T1, typename T2>
	void swap_pair(pair<T1, T2>& p) {
		mySimpleSTL::swap(p.first, p.second);
	}

	template<typename T1, typename T2>
	pair<T1, T2> make_pair(T1&& t, T2&& u) {
		return pair<T1, T2>(mySimpleSTL::forward<T1>(t), mySimpleSTL::forward<T2>(u));
	}
};

void test_pair() {
	// 测试pair的构造函数
	mySimpleSTL::pair<int, double> p1(1, 2.5);
	mySimpleSTL::pair<int, double> p2(p1);
	mySimpleSTL::pair<int, double> p3(mySimpleSTL::make_pair(3, 4.5));

	// 测试pair的赋值运算符
	p2 = p3;

	// 测试pair的比较运算符
	if (p1 == p2) {
		std::cout << "p1 and p2 are equal" << std::endl;
	}
	if (p2 != p3) {
		std::cout << "p2 and p3 are not equal" << std::endl;
	}
	if (p1 < p3) {
		std::cout << "p1 is less than p3" << std::endl;
	}
	if (p3 > p1) {
		std::cout << "p3 is greater than p1" << std::endl;
	}

	// 测试swap函数、swap_pair函数和pair的交换成员函数
	mySimpleSTL::pair<int, int> p4(1, 2);
	mySimpleSTL::pair<int, int> p5(3, 4);
	std::cout << "Before swapping: " << p4.first << ", " << p4.second
		<< " and " << p5.first << ", " << p5.second << std::endl;
	mySimpleSTL::swap(p4, p5);
	std::cout << "After swapping with swap function: " << p4.first << ", " << p4.second
		<< " and " << p5.first << ", " << p5.second << std::endl;
	mySimpleSTL::swap_pair(p4);
	std::cout << "After swapping with swap_pair function: " << p4.first << ", " << p4.second << std::endl;
	p4.swap(p5);
	std::cout << "After swapping with member function: " << p4.first << ", " << p4.second
		<< " and " << p5.first << ", " << p5.second << std::endl;
}


#endif