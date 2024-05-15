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
	// 不可平凡析构---->这个T有它自己的析构函数
	template<typename T>
	inline void destory_one(T* p, std::false_type) {		// T不是可平凡析构的,那么就需要它自己的析构函数来析构
		if (p)
			p->~T();
	}
	// 可平凡析构---->这个T的析构函数是默认的
	template<typename T>
	inline void destory_one(T* p, std::true_type) {		// T是可平凡析构的,但不知道为什么就不管了，int*内存应该还是要释放啊
														// 因为析构函数是销毁对象，释放对象的操作应该放到allocator中的deallocate函数
														// 这里释放了，那么deallocate就会是一个nullptr
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
		destory_one(p, std::is_trivially_destructible<T>{});			// is_trivially_destructible；T是可平凡析构的吗？
	}
	template<typename ForwardIter>
	inline void destroy(ForwardIter* bg, ForwardIter* ed) {
		destory_range(bg,ed, std::is_trivially_destructible<
			typename iterator_traits<ForwardIter>::value_type>{});
	}
*/
	/*
	可平凡析构（trivial destruction）指的是一个类型的对象在销毁时不需要进行显式的析构操作。
		如果一个类型具有可平凡析构，那么它的析构函数是一个空函数或者是被编译器自动生成的默认析构函数，不会包含显式的逻辑。
		当该类型的对象被销毁时，内存中的内容可以直接被忽略，因为没有需要特殊处理的资源需要释放。
		-- 可平凡析构即，类似int，char这些内置类型；不需要你写析构的代码

	对于那些不是可平凡析构的类型，其析构函数可能会涉及到内存释放、资源回收等操作，
		需要用户自己实现或调用相应的析构函数来完成。
		因此，在泛型算法中针对不同类型对象的销毁需求，根据其是否拥有可平凡析构特性，可以使用不同的销毁方式以优化效率。
		-- 不可平凡析构即，类似class，malloc这些内存；需要指出free操作
	*/


	/*
	* 示例：
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
	// construct 构造对象

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

	// destroy 将对象析构

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