#pragma once
#ifndef MYSIMPLESTL_QUEUE_H
#define MYSIMPLESTL_QUEUE_H
#include"deque.h"

namespace mySimpleSTL {
	

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	* queue
	* queue的特性先进先出，那么将deque的双向push&pop，改成前面pop，后面push就好了
	* queue和stack一样是deque的adapter;
	* 而queue中能够取用到的元素只有front,不需要迭代器提供遍历功能
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template<typename T,class Sequence = mySimpleSTL::deque<T>>
	class queue {
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::const_reference	const_reference;
	private:
		Sequence c;
	public:
		bool empty() const { return c.empty(); };
		size_type size()const { return c.size(); };
		reference front() { return c.front(); };
		const_reference front()const { return c.front(); };
		reference back() { return c.back(); };
		const_reference back()const { return c.back(); };
	public:
		friend bool operator==(const queue& lv, const queue& rv);
		friend bool operator < (const queue & lv, const queue & rv);
		
	public:
		void push(const value_type& val) { c.push_back(val); };
		void pop() { c.pop_front(); }
	};

	template<typename T,class Sequence = mySimpleSTL::deque<T>>
	bool operator==(const queue<T, Sequence>& lv, const queue<T, Sequence>& rv) {
		return lv == rv;
	}
	template<typename T, class Sequence = mySimpleSTL::deque<T>>
	bool operator < (const queue<T, Sequence>&lv, const queue<T, Sequence>&rv) {
		return lv < rv;
	}

}

#endif