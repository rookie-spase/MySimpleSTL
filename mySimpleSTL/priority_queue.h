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
		// �ͱ���
	private:
		// ���� �� �Ƚ�����
		Sequence c;
		Compare comp;
	public:
		// ���캯��
		priority_queue() : c(), comp() {} // Ĭ�Ϲ��캯��
		explicit priority_queue(const Compare& x) : c(), comp(x) {} // �ԱȽϺ�������Ϊ�����Ĺ��캯��
		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) { // �Ե�������Χ�ͱȽϺ�������Ϊ�����Ĺ��캯��
			make_heap(c.begin(), c.end(), comp); // ���ײ�����ת��Ϊ��
		}
		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last) : c(first, last), comp() { // �Ե�������ΧΪ�����Ĺ��캯��
			make_heap(c.begin(), c.end(), comp); // ���ײ�����ת��Ϊ��
		}
		bool empty()  { return c.empty(); } // �ж����ȶ����Ƿ�Ϊ��
		size_type size()  { return c.size(); } // �������ȶ�����Ԫ�صĸ���
		const_reference top()  { return c.front(); } // �������ȶ��������Ԫ��
		void push(const value_type& x) { // �����ȶ����в���һ��Ԫ��
			c.push_back(x); // �ڵײ�������ĩβ����Ԫ��
			mySimpleSTL::push_heap(c.begin(), c.end(), comp); // �����ѵ�����
		}
		void pop() { // �����ȶ����е���һ��Ԫ��
			mySimpleSTL::pop_heap(c.begin(), c.end(), comp); // ���ѵĶ���Ԫ�طŵ��ײ�������ĩβ
			c.pop_back(); // �ӵײ�������ɾ��ĩβԪ��
		}
	};
};

#endif