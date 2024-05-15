#pragma once
#ifndef MYSIMPLESTL_HEAP_ALGO
#define MYSIMPLESTL_HEAP_ALGO

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * *
* ����complete binary treeֱ��ʹ��vector�Ϳ��ԣ�����Ҫר��������ݽṹ
* ֻ��Ҫ��Ƴ�ά�����Ե��㷨�ͺã�
* 
* ���ԣ�
*	���������һ�㲻��Ϊ�գ������һ���Ǵ�����������
*	max-tree:���ڵ��ֵ���ӽڵ��
*	min-tree:���ڵ��ֵ���ӽڵ�С
*	���ڵ㣺i/2
*	���ӽڵ㣺2*i
*	���ӽڵ㣺2*i+1
* 
* ��ô�͵���������ߵĶ��Ǹ��ڵ㣬�������ұߵĶ����ӽڵ㣬����(size-2)/2���Ǹ��ڵ�
* 
* ����˵�Ķ���max-tree
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include"iterator.h"

namespace mySimpleSTL {
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	* ��heap�йص��㷨��
	* - push_heap
	*	push_heap�������һ���������򣬽�����������򿴳�һ��compelete binary tree�Ľṹ��
	*	�����һ��Ԫ�ؾ����²����Ԫ��
	*	push_heap����������һ��Ԫ��(��end()-1)����ά��max-tree�Ľṹ��ά���ķ�ʽ��ʹ�������㷨��
	*		�����㷨������ӽڵ�ȸ��ڵ����ô���ӽڵ㽻����ֱ���ӽڵ�ȸ��ڵ����������������ߵ�����ڵ㣻ʹ��whileʵ�������㷨
	*	push_heap���ýṹ
	*		push_heap->__push_heap_aux->__push_heap;�����㷨��__push_heap��ʵ��
	* 
	* - pop_heap
	*	���ã�����ȫ��������ȥ�����ڵ㣬���Ƿŵ������ĩ��
	* 
	*	���max-tree��������Ϊ��ʵ��һ��primary queue����primary queueÿ��pop��ʱ����pop�������Ǹ�ֵ��
	*	�������pop_heapÿ��pop��ȥ�Ķ��Ǹ��ڵ㣻
	*	popͨ��������ʵ�֣����Ӹ��ڵ�Ƚ������ӽڵ��Ǹ����󣬽�������Ǹ��ӽڵ�͸��ڵ㽻����Ȼ������Խ�����ĸ��ڵ�����Ƚϡ�
	*	����ʱһ�������Ĺ��̣�ֱ�����ڵ��ҵ�һ�������ҽڵ�ĵط��Ż�ֹͣ����ô�ͻ���һ���ӽڵ�Ϊ�������ڵ��ֵ��
	*	��ô����ײ����ұߵĽڵ���Ϊvalue,�Խ������ײ�ĸ��ڵ���Ϊ�ڵ㣬ֵΪvalue������push���Ϳ��ԣ�
	* 
	*	���Կ���ÿ��pop��ʱ�򶼻ᷢ����
	*		1.���ڵ�ŵ����
	*		2.��ײ����ұߵĽڵ����²���
	*	ÿ��popʱ�����и��ڵ���ʧ  ����  ���ұ���ײ�Ľڵ������push.
	*	����ÿ��pop��ʱ�򶼻���
	*		1. ���д��߼�����˵��һ���ڵ�,��������δ�ı��С
			2.���ұ���ײ�Ľڵ��Ϊ���ڵ㣬����Ҳ�������ұ���ײ�Ľڵ�
		// ÿ��pop��������һ����ײ����ұߵĽڵ�;��������ڵ㲢������ʧ�ˣ����ǻ��������ĸ��ڵ��ֵ
	* 
	*	ע�⣺	ÿ��pop��ʱ�����ĸ��ڵ㲻����ʧ��
	*			���Ƿŵ��������ĩ��
	* 
	* - sort_heap
	*	����ÿ��pop��ʱ�������ֵ,pop֮��Ὣ���ֵ�ŵ������ô�����ֻ��Ҫ��ͣpop�ͺ��ˣ�
	*	����ÿ�ζ�����һ���ڵ㣬�������֮�������߼�����˵Ҳ�Ͳ�������
	* 
	* - make_heap
	*	��һ�������rangeת��Ϊһ��compelete binary tree;
	*   ������ߵĶ��Ǹ��ڵ㣬�������ұߵĶ����ӽڵ㣬����(size-2)/2���Ǹ��ڵ�;
	*	��ô��ÿ�����ڵ����__adjust_heap�Ϳ��Դ���һ������;
	*	__adjust_heap�����þ�����������
	* 
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	

	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compare comp);
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap_aux(RandomIter first, RandomIter last, Distance*, T*, Compare comp);
	template<typename RandomIter, typename Compare>
	inline void push_heap(RandomIter first, RandomIter last, Compare comp);


	// push_heap

// push_heap����
	template<typename RandomIter, typename Compare>
	inline void push_heap(RandomIter first, RandomIter last, Compare comp = std::less<typename decltype(value_type(first))>) {
		// ���ȼ�������Ƿ���Ч
		if (first == last) return; // �������������
		if (first + 1 == last) return; // ֻ��һ��Ԫ���������
		// Ȼ�����__push_heap_aux����
		__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}

	// __push_heap_aux����
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap_aux(RandomIter first, RandomIter last, Distance*, T*, Compare comp) {
		// ���Ȼ�ȡ�²���Ԫ�ص�λ�ú�ֵ
		auto pos = last - 1; // �²����Ԫ�ص�λ�������һ��Ԫ��
		auto value = *pos; // �²����Ԫ�ص�ֵ
		// Ȼ�����__push_heap����
		__push_heap(first, Distance((last - first) - 1), Distance(0), value, comp);
	}

	// __push_heap����
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compare comp) {
		// �����ҵ����ڵ��λ��
		Distance parent = (holeIndex - 1) / 2;
		// Ȼ����������㷨�������ֵ�����丸�ڵ㣨���ݱȽ��������жϣ����򽫸��ڵ����ƣ����������ϱȽ�ֱ�����ڵ������������ֵС�Ľڵ�Ϊֹ
		while (holeIndex > topIndex && comp(*(first + parent), value)) {
			*(first + holeIndex) = *(first + parent); // �����ڵ��ֵ�����ӽڵ�
			holeIndex = parent; // �����ӽڵ��λ��Ϊ���ڵ��λ��
			parent = (holeIndex - 1) / 2; // ���¸��ڵ��λ��
		}
		// �����ֵ���뵽��ȷ��λ��
		*(first + holeIndex) = value;
	}



	// pop_heap

	template<typename RandomIter, typename T, typename Compare>
	inline void __pop_heap_aux(RandomIter first, RandomIter last, T*, Compare comp);
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compare comp);
	template<typename RandomIter, typename T, typename Distance, typename Compare>
	inline void __pop_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, Compare comp);

// pop_heap����
	template<typename RandomIter, typename Compare>
	inline void pop_heap(RandomIter first, RandomIter last, Compare comp = std::less<typename decltype(value_type(first))>) {
		// ���ȼ�������Ƿ���Ч
		if (first == last) return; // �������������
		if (first + 1 == last) return; // ֻ��һ��Ԫ���������
		// Ȼ�����__pop_heap_aux����
		__pop_heap_aux(first, last, value_type(first), comp);
	}

	// __pop_heap_aux����
	template<typename RandomIter, typename T, typename Compare>
	inline void __pop_heap_aux(RandomIter first, RandomIter last, T*, Compare comp) {
		// ���Ȼ�ȡ���ֵ��λ�ú�ֵ
		auto result = last - 1; // ���ֵ��λ�������һ��Ԫ��
		auto value = *result; // ���ֵ��ֵ
		// Ȼ�����__pop_heap����
		__pop_heap(first, result, result, value, distance_type(first), comp);
	}

	// __pop_heap����
	template<typename RandomIter, typename T, typename Distance, typename Compare>
	inline void __pop_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, Compare comp) {
		// ���Ƚ����ֵ�ŵ����һ��Ԫ�ص�λ��
		*result = *first;
		// Ȼ�����__adjust_heap�������Ե�һ��Ԫ�ؽ����³�����
		__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
	}

	// __adjust_heap����
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compare comp) {
		// ���Ȼ�ȡ���ڵ��λ��
		Distance topIndex = holeIndex;
		// Ȼ������³��������Ӹ��ڵ�Ƚ������ӽڵ��ĸ����󣨸��ݱȽ��������жϣ�����������Ǹ��ӽڵ�͸��ڵ㽻����Ȼ������Խ�����ĸ��ڵ�����Ƚ�
		while (holeIndex * 2 + 1 < len) { // ���������ӽڵ�ʱ
			auto left = holeIndex * 2 + 1; // �������ӽڵ��λ��
			auto right = left + 1; // �������ӽڵ��λ��
			if (right < len && comp(*(first + left), *(first + right))) { // ������ӽڵ�����ұ����ӽڵ��
				left = right; // ��������ӽڵ��λ��Ϊ���ӽڵ��λ��
			}
			if (comp(*(first + left), value)) { // �������ӽڵ��ֵС��Ҫ�³���ֵ��˵���Ѿ��������ѵ���������������³�
				break;
			}
			*(first + holeIndex) = *(first + left); // ������ӽڵ��ֵ�������ڵ�
			holeIndex = left; // ���¸��ڵ��λ��Ϊ����ӽڵ��λ��
		}
		// ������__push_heap�����������һ��Ԫ������push������
		__push_heap(first, holeIndex, topIndex, value, comp);
	}


	// sort_heap
	template<typename RandomIter>
	void sort_heap(RandomIter first, RandomIter last);
	template<typename RandomIter>
	void sort_heap(RandomIter first, RandomIter last) {
		while (last - first > 1) {
			pop_heap(first,last--);		// ÿ��pop��������һ����ײ����ұߵĽڵ�;��������ڵ㲢������ʧ�ˣ����ǻ��������ĸ��ڵ��ֵ
		}
	}
	
	
	// make_heap

	template<typename RandomIter, typename T, typename Distance>
	inline void __make_heap(RandomIter first, RandomIter last, T*, Distance*);
	template<typename RandomIter>
	inline void make_heap(RandomIter first, RandomIter last);

	template<typename RandomIter>
	inline void make_heap(RandomIter first, RandomIter last) {
		__make_heap(first,last,value_type(first),distance_type(first));
	};

	template<typename RandomIter, typename T, typename Distance>
	inline void __make_heap(RandomIter first, RandomIter last, T*, Distance*) {
		if (last - first < 2)return;
		Distance len = last - first;
		// �ҵ�ͷ��
		Distance holeIndex = (len - 2) / 2;
		// ���Ÿ��ڵ�
		while (true) {
			__adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
			if (holeIndex == 0)return;
			holeIndex--;
		}
	}

};

#endif