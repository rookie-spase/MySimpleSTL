#pragma once
#ifndef MYSIMPLESTL_UNINITIALIZED_H
#define MYSIMPLESTL_UNINITIALIZED_H

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"


namespace mySimpleSTL {
	/*****************************************************************************************/
	// uninitialized_copy
	// �� [first, last) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	/*****************************************************************************************/
	template<typename Iter1, typename Iter2>
	Iter2 unchecked_uninit_copy(Iter1 bg, Iter1 ed, Iter2 bg_start, std::true_type) {	// ���û���Լ��Ŀ������캯������ô����true_type
		return mySimpleSTL::copy(bg, ed, bg_start);
	};
	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		auto cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
			{
				mySimpleSTL::construct(&*cur, *first);
			}
		}
		catch (...)
		{
			for (; result != cur; --cur)
				mySimpleSTL::destroy(&*cur);
		}
		return cur;

	};
	template<typename Iter1, typename Iter2>
	Iter2 uninitialized_copy(Iter1 bg, Iter1 ed, Iter2 bg_start) {
		return unchecked_uninit_copy(bg, ed, bg_start,
			std::is_trivially_copy_assignable<typename iterator_traits<Iter2>::value_type>{}
		);
	};


	/*****************************************************************************************/
	// uninitialized_copy_n
	// �� [first, first + n) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
	/*****************************************************************************************/

	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_copy_n(InputIter first, size_t n, ForwardIter result, std::true_type) {
		return mySimpleSTL::copy_n(first, n, result).second;
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_copy_n(InputIter first, size_t n, ForwardIter result, std::false_type) {
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first, ++cur)
			{
				mySimpleSTL::construct(&*cur, *first);
			}
		}
		catch (...)
		{
			for (; result != cur; --cur)
				mySimpleSTL::destroy(&*cur);
		}
		return cur;
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter uninitialized_copy_n(InputIter first, size_t n, ForwardIter result) {
		return unchecked_uninit_copy_n(first, n, result,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{}
		);
	}
	/*****************************************************************************************/
	// uninitialized_fill
	// �� [first, last) ���������Ԫ��ֵ
	/*****************************************************************************************/


	template<typename ForwardIter, typename T>
	void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type) {
		mySimpleSTL::fill(first, last, value);
	}
	template<typename ForwardIter, typename T>
	void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type) {
		auto cur = first;
		try
		{
			for (; cur != last; ++cur)
			{
				mySimpleSTL::construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				mySimpleSTL::destroy(&*first);
		}
	}
	template<typename ForwardIter, typename T>
	void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
		unchecked_uninit_fill(first, last, value,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{}
		);
	}

/*****************************************************************************************/
// uninitialized_fill_n
// �� first λ�ÿ�ʼ����� n ��Ԫ��ֵ��������������λ��
/*****************************************************************************************/


	template<typename ForwardIter, typename T>
	ForwardIter unchecked_uninit_fill_n(ForwardIter first, size_t n, const T& value, std::true_type) {
		return mySimpleSTL::fill_n(first, n, value);
	}
	template<typename ForwardIter, typename T>
	ForwardIter unchecked_uninit_fill_n(ForwardIter first, size_t n, const T& value, std::false_type) {
		auto cur = first;
		try
		{
			for (; n > 0; --n, ++cur)
			{
				mySimpleSTL::construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				mySimpleSTL::destroy(&*first);
		}
		return cur;
	}
	template<typename ForwardIter, typename T>
	ForwardIter uninitialized_fill_n(ForwardIter first, size_t n, const T& value) {
		return unchecked_uninit_fill_n(first, n, value,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{}
		);
	}

	/*****************************************************************************************/
	// uninitialized_move
	// ��[first, last)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
	/*****************************************************************************************/


	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return mySimpleSTL::move(first, last, result);
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		auto cur = result;
		try
		{
			for (; first != last; ++first, ++cur)
			{
				mySimpleSTL::construct(&*cur, mySimpleSTL::move(*first));
			}
		}
		catch (...)
		{
			for (; result != cur; --cur)
				mySimpleSTL::destroy(&*cur);
		}
		return cur;
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
		return unchecked_uninit_move(first, last, result,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{}
		);
	}
/*****************************************************************************************/
// uninitialized_move_n
// ��[first, first + n)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
/*****************************************************************************************/

	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_move_n(InputIter first, size_t n, ForwardIter result, std::true_type) {
		return mySimpleSTL::move(first, first + n, result);
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter unchecked_uninit_move_n(InputIter first, size_t n, ForwardIter result, std::false_type) {
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first, ++cur)
			{
				mySimpleSTL::construct(&*cur, mySimpleSTL::move(*first));
			}
		}
		catch (...)
		{
			for (; result != cur; --cur)
				mySimpleSTL::destroy(&*cur);
		}
		return cur;
	}
	template<typename InputIter, typename ForwardIter>
	ForwardIter uninitialized_move_n(InputIter first, size_t n, ForwardIter result) {
		return unchecked_uninit_move_n(first, n, result,
			std::is_trivially_copy_assignable<typename iterator_traits<ForwardIter>::value_type>{}
		);
	}


	/*
	ʾ���������гɹ�_����û������_iterator��algobase��copyȷʵ�޺��ˣ�
		//����uninitialized_copy
	int a[5] = { 1,2,3,4,5 };
	int b[5];
	mySimpleSTL::uninitialized_copy(a, a + 5, b);
	for (int i = 0; i < 5; i++) {
		std::cout << b[i] << " ";
	}
	std::cout << std::endl;

	//����uninitialized_copy_n
	int c[5] = { 1,2,3,4,5 };
	int d[5];
	mySimpleSTL::uninitialized_copy_n(c, 5, d);
	for (int i = 0; i < 5; i++) {
		std::cout << d[i] << " ";
	}
	std::cout << std::endl;

	//����uninitialized_fill
	int e[5];
	mySimpleSTL::uninitialized_fill(e, e + 5, 5);
	for (int i = 0; i < 5; i++) {
		std::cout << e[i] << " ";
	}
	std::cout << std::endl;

	//����uninitialized_fill_n
	int f[5];
	mySimpleSTL::uninitialized_fill_n(f, 5, 5);
	for (int i = 0; i < 5; i++) {
		std::cout << f[i] << " ";
	}
	std::cout << std::endl;

	//����uninitialized_move
	std::string s[5] = { "hello","world","my","name","is" };
	std::string s1[5];
	mySimpleSTL::uninitialized_move(s, s + 5, s1);
	for (int i = 0; i < 5; i++) {
		std::cout << s1[i] << " ";
	}
	std::cout << std::endl;

	//����uninitialized_move_n
	std::string s2[5] = { "hello","world","my","name","is" };
	std::string s3[5];
	mySimpleSTL::uninitialized_move_n(s2, 5, s3);
	for (int i = 0; i < 5; i++) {
		std::cout << s3[i] << " ";
	}
	std::cout << std::endl;

	*/

};



#endif