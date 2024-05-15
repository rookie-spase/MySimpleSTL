#pragma once
#ifndef MYSIMPLESTL_STACK_H
#define MYSIMPLESTL_STACK_H

#include "deque.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
* stack��Ҫ���Ϊ�Ƚ����
* ��˽�deque��˫�˱�Ϊһ�˾Ϳ���ʵ��;
* ��ΪԪ����Ҫ�Ƚ��������ôֻ�ж��˵�Ԫ�ز��ܱ�ʹ�ã�����ò�����������Ҳ��û����Ƶ�������
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace mySimpleSTL {
	
	template<typename T,class Sequence = mySimpleSTL::deque<T>>
	class stack {
	private:		// �����ĳ�Ա
		Sequence c;
		
	public:		// �ͱ���
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