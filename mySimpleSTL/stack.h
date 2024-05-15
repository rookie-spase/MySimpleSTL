#pragma once
#ifndef MYSIMPLESTL_STACK_H
#define MYSIMPLESTL_STACK_H

#include "deque.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
* stack的要求仅为先进后出
* 因此将deque的双端变为一端就可以实现;
* 因为元素需要先进后出，那么只有顶端的元素才能被使用，因此用不到迭代器，也就没有设计迭代器。
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace mySimpleSTL {
	
	template<typename T,class Sequence = mySimpleSTL::deque<T>>
	class stack {
	private:		// 真正的成员
		Sequence c;
		
	public:		// 型别定义
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::const_reference	const_reference;
	public:
		bool empty() const { return c.empty(); };
		size_type size() const { return c.size(); };
		reference top() { return c.back(); };
		const_reference top() const { return c.back(); };
	public:
		void push(const value_type& e) { c.push_back(e);};
		void pop() { c.pop_back(); };
	public:
		friend bool operator == (const stack& lv, const stack& rv);
		friend bool operator < (const stack& lv, const stack& rv);
	};

	template<typename T,class Sequence>
	bool  operator == (const stack<T, Sequence>& lv, const stack<T, Sequence>& rv) {
		return lv.c == rv.c;
	}

	template<typename T, class Sequence>
	bool  operator < (const stack<T, Sequence>& lv, const stack<T, Sequence>& rv) {
		return lv.c < rv.c;
	}

}



#endif