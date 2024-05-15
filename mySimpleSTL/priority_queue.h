#pragma once
#ifndef MYSIMPLESTL_PRIORITY_QUEUE_H
#define MYSIMPLESTL_PRIORITY_QUEUE_H

#include"heap_algo.h"
#include"vector.h"

namespace mySimpleSTL {

	template<typename T,class Sequence = mySimpleSTL::vector<T>,class Compare = std::less<typename Sequence::value_type>>
	class priority_queue {
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::iterator iterator;
		// 型别定义
	private:
		// 数组 和 比较运算
		Sequence c;
		Compare comp;
	public:
		// 构造函数
		priority_queue() : c(), comp() {} // 默认构造函数
		explicit priority_queue(const Compare& x) : c(), comp(x) {} // 以比较函数对象为参数的构造函数
		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) { // 以迭代器范围和比较函数对象为参数的构造函数
			make_heap(c.begin(), c.end(), comp); // 将底层容器转换为堆
		}
		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last) : c(first, last), comp() { // 以迭代器范围为参数的构造函数
			make_heap(c.begin(), c.end(), comp); // 将底层容器转换为堆
		}
		bool empty()  { return c.empty(); } // 判断优先队列是否为空
		size_type size()  { return c.size(); } // 返回优先队列中元素的个数
		const_reference top()  { return c.front(); } // 返回优先队列中最大元素
		void push(const value_type& x) { // 向优先队列中插入一个元素
			c.push_back(x); // 在底层容器的末尾插入元素
			mySimpleSTL::push_heap(c.begin(), c.end(), comp); // 调整堆的性质
		}
		void pop() { // 从优先队列中弹出一个元素
			mySimpleSTL::pop_heap(c.begin(), c.end(), comp); // 将堆的顶部元素放到底层容器的末尾
			c.pop_back(); // 从底层容器中删除末尾元素
		}
	};
};

#endif