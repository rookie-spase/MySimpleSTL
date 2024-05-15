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
	* ���ͣ�
	* remove_reference<T>���Խ�int&��Ϊint,��T&&�ȿ��Խ�����ֵҲ���Խ�����ֵ
	* remove_reference<T>��Ϊint֮�����&&����Ϊ����ֵ
	* 
	* ʾ����
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
	* �ص㣺
	* remove_refrence����Ϊint����ô��forward<int>��ʱ�򣬾ͻ����2������
	* forward(int&)
	* forward(int&&)
	* ����2������������ͻ��
	* 
	* ���ͣ�
	* ģ������ת������˼���Ǵ���������int&��ô�ͷ���int&��
	* ������������int&& �� ��ô����int&&
	* ��������int&����ôarg��int&
	* ��static_cast<T&&>�е�T&&�ǿ��Խ�����ֵҲ���Խ�����ֵ������static_cast<T&&>��õ�int&
	* T&& Ҳ���Խ�����ֵ�����Է���ֵΪint&
	* 
	* 
	* ʾ����
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
	* ���ͣ�
	* ����Ĳ���������T&&������ʵ�θı䲻��
	* 
	* ʾ����
	* int w = 3,e =2;swap(w,e);cout << w;
	*/

	template<typename forwardIterator_1, typename forwardIterator_2>
	forwardIterator_2 swap_range(forwardIterator_1 begin, forwardIterator_1 end, forwardIterator_2 target) {
		while (begin != end) 
			mySimpleSTL::swap(*begin++,*target++);

		return target;
	}
	/*
	* ���ͣ�
	* ʵ��2�����еĽ���
	* 
	* ʾ����
	*  std::vector<int> a{ 1, 2, 3 };std::vector<int> b{ 4, 5, 6 }; auto iter = mySimpleSTL::swap_range(a.begin(), a.end(), b.begin()); for (const auto& x : b) { std::cout << x << " "; } // �����1 2 3
	*/
	
	template<typename T,size_t N>
	void swap(T(&a), T(&b)) {			// ����������ã�������ָ��
		mySimpleSTL::swap(a,a+N,b);
	}
	/*
	* ʾ����
	*     int arr1[] = { 1, 2, 3 };
    int arr2[] = { 4, 5, 6 };

    cout << "Before swapping:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << ", "
            << "arr2[" << i << "] = " << arr2[i] << endl;
    }

    // ���� swap ���������������ֵ
    swap<int, 3>(arr1, arr2);

    cout << "\nAfter swapping:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << ", "
            << "arr2[" << i << "] = " << arr2[i] << endl;
    }
	*/

	// ��������pair��ʵ��
	
	template<typename T1,typename T2>
	class pair {
	public:
		T1 first;
		T2 second;



		// ------------------------���캯��----------------------------
		// Ĭ�Ϲ��캯��
		template<typename Other1 = T1, typename Other2 = T2,
			typename = typename std::enable_if<				// enable_if�����Ϊ1���ǵĻ����ù��캯�������ǵĻ����ù��캯��
			std::is_default_constructible<Other1>::value&&		// ���Other1�Ƿ���Ĭ�Ϲ��캯�����������ôvalue����true
			std::is_default_constructible<Other2>::value
			, void			>::type>
		pair() :first(), second() {}

		template <class Other1 = T1, class Other2 = T2,
			typename std::enable_if<
			std::is_copy_constructible<Other1>::value&&
			std::is_copy_constructible<Other2>::value&&			// ���Other�Ƿ���п������캯��
			std::is_convertible<const Other1&, T1>::value&&		// ���T1�Ƿ����ת��Ϊconst T1&
			std::is_convertible<const Other2&, T2>::value, int>::type = 0>
		constexpr pair(const T1& a, const T2& b)
			: first(a), second(b){}

		pair(const pair& rv) = default;
		pair(pair&& rv) = default;
		// ------------------------���캯��-------------------------------


		// ������ֵ
		pair& operator = (const pair& rv) {
			if (this != &rv) {
				first = rv.first;
				second = rv.second;
			}
			return *this;
		}
		// �ƶ���ֵ
		pair& operator = (pair&& rv) noexcept {
			if (this != &rv) {
				first = mySimpleSTL::move(rv.first);
				second = mySimpleSTL::move(rv.second);
			}
			return *this;
		}

		// ģ�忽����ֵ
		template <typename Other1, typename Other2,
			typename = std::enable_if_t<std::is_convertible_v<Other1, T1>&& std::is_convertible_v<Other2, T2>>>
		pair& operator = (const pair<Other1, Other2>& rv) {
			this->first = rv.first;
			this->second = rv.second;

			return *this;
		};

		// ģ���ƶ���ֵ
		template <typename Other1, typename Other2,
			typename = std::enable_if_t<std::is_convertible_v<Other1, T1>&& std::is_convertible_v<Other2, T2>>>
		pair&& operator = (const pair<Other1, Other2>&& rv) {
			this->first = mySimpleSTL::move(rv.first);
			this->second = mySimpleSTL::move(rv.second);

			return mySimpleSTL::move(*this);
		};

		// ��������
		~pair() = default;

		// ��������
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
	// ����pair�Ĺ��캯��
	mySimpleSTL::pair<int, double> p1(1, 2.5);
	mySimpleSTL::pair<int, double> p2(p1);
	mySimpleSTL::pair<int, double> p3(mySimpleSTL::make_pair(3, 4.5));

	// ����pair�ĸ�ֵ�����
	p2 = p3;

	// ����pair�ıȽ������
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

	// ����swap������swap_pair������pair�Ľ�����Ա����
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