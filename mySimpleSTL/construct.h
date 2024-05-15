#pragma once
#ifndef MYSIMPLESTL_CONSTRUCT_H
#define MYSIMPLESTL_CONSTRUCT_H
#include <new>
#include "util.h"
namespace mySimpleSTL {
	/*
	template<typename T1>
	void construct(T1* p) {
		::new((void*)p) T1();
	}

	template<typename T1, typename T2>
	void construct(T1* p, T2 val) {
		::new ((void*)p) T1(val);
	};

	template<typename T1,typename... arg>
	void construct(T1* p, arg&&... val) {
		::new ((void*)p) T1(mySimpleSTL::forward<val>(val)...);
	}


	//destory
	// ����ƽ������---->���T�����Լ�����������
	template<typename T>
	inline void destory_one(T* p, std::false_type) {		// T���ǿ�ƽ��������,��ô����Ҫ���Լ�����������������
		if (p)
			p->~T();
	}
	// ��ƽ������---->���T������������Ĭ�ϵ�
	template<typename T>
	inline void destory_one(T* p, std::true_type) {		// T�ǿ�ƽ��������,����֪��Ϊʲô�Ͳ����ˣ�int*�ڴ�Ӧ�û���Ҫ�ͷŰ�
														// ��Ϊ�������������ٶ����ͷŶ���Ĳ���Ӧ�÷ŵ�allocator�е�deallocate����
														// �����ͷ��ˣ���ôdeallocate�ͻ���һ��nullptr
	}


	template<typename forwardIter>
	void destory_range(forwardIter bg, forwardIter ed, std::false_type) {
		while (bg!=ed)
			destroy(&*bg++);
		
	}
	template<typename forwardIter>
	void destory_range(forwardIter bg, forwardIter ed, std::true_type) {}

	template<typename T>
	inline void destroy(T* p) {
		destory_one(p, std::is_trivially_destructible<T>{});			// is_trivially_destructible��T�ǿ�ƽ����������
	}
	template<typename ForwardIter>
	inline void destroy(ForwardIter* bg, ForwardIter* ed) {
		destory_range(bg,ed, std::is_trivially_destructible<
			typename iterator_traits<ForwardIter>::value_type>{});
	}
*/
	/*
	��ƽ��������trivial destruction��ָ����һ�����͵Ķ���������ʱ����Ҫ������ʽ������������
		���һ�����;��п�ƽ����������ô��������������һ���պ��������Ǳ��������Զ����ɵ�Ĭ���������������������ʽ���߼���
		�������͵Ķ�������ʱ���ڴ��е����ݿ���ֱ�ӱ����ԣ���Ϊû����Ҫ���⴦�����Դ��Ҫ�ͷš�
		-- ��ƽ��������������int��char��Щ�������ͣ�����Ҫ��д�����Ĵ���

	������Щ���ǿ�ƽ�����������ͣ��������������ܻ��漰���ڴ��ͷš���Դ���յȲ�����
		��Ҫ�û��Լ�ʵ�ֻ������Ӧ��������������ɡ�
		��ˣ��ڷ����㷨����Բ�ͬ���Ͷ�����������󣬸������Ƿ�ӵ�п�ƽ���������ԣ�����ʹ�ò�ͬ�����ٷ�ʽ���Ż�Ч�ʡ�
		-- ����ƽ��������������class��malloc��Щ�ڴ棻��Ҫָ��free����
	*/


	/*
	* ʾ����
	int* p1 = new int(10);
    std::string* p2 = new std::string("hello world");
    std::vector<int>* p3 = new std::vector<int>{ 1,2,3,4,5 };

    mySimpleSTL::construct(p1, 10);
    mySimpleSTL::construct(p2, "hello world");
    mySimpleSTL::construct(p3, std::initializer_list<int>{1, 2, 3, 4, 5});

    mySimpleSTL::destroy(p1);
    mySimpleSTL::destroy(p2);
    mySimpleSTL::destroy(p3);
	*/
	// construct �������

	template <class Ty>
	void construct(Ty* ptr)
	{
		::new ((void*)ptr) Ty();
	}

	template <class Ty1, class Ty2>
	void construct(Ty1* ptr, const Ty2& value)
	{
		::new ((void*)ptr) Ty1(value);
	}

	template <class Ty, class... Args>
	void construct(Ty* ptr, Args&&... args)
	{
		::new ((void*)ptr) Ty(mySimpleSTL::forward<Args>(args)...);
	}

	// destroy ����������

	template <class Ty>
	void destroy_one(Ty*, std::true_type) {}

	template <class Ty>
	void destroy_one(Ty* pointer, std::false_type)
	{
		if (pointer != nullptr)
		{
			pointer->~Ty();
		}
	}

	template <class ForwardIter>
	void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

	template <class ForwardIter>
	void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
	{
		for (; first != last; ++first)
			destroy(&*first);
	}

	template <class Ty>
	void destroy(Ty* pointer)
	{
		destroy_one(pointer, std::is_trivially_destructible<Ty>{});
	}

	template <class ForwardIter>
	void destroy(ForwardIter first, ForwardIter last)
	{
		destroy_cat(first, last, std::is_trivially_destructible<
			typename iterator_traits<ForwardIter>::value_type>{});
	}

};


#endif