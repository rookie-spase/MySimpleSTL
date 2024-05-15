#pragma once
#ifndef MYSIMPLESTL_HEAP_ALGO
#define MYSIMPLESTL_HEAP_ALGO

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * *
* 由于complete binary tree直接使用vector就可以，不需要专门设计数据结构
* 只需要设计出维护特性的算法就好；
* 
* 特性：
*	树除了最后一层不能为空，且最后一层是从左到右连续；
*	max-tree:父节点的值比子节点大
*	min-tree:父节点的值比子节点小
*	父节点：i/2
*	左子节点：2*i
*	右子节点：2*i+1
* 
* 那么就导致数组左边的都是根节点，而数组右边的都是子节点，数组(size-2)/2就是根节点
* 
* 这里说的都是max-tree
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include"iterator.h"

namespace mySimpleSTL {
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	* 和heap有关的算法有
	* - push_heap
	*	push_heap传入的是一个数组区域，将这个数组区域看成一个compelete binary tree的结构，
	*	而最后一个元素就是新插入的元素
	*	push_heap就是针对最后一个元素(即end()-1)，来维护max-tree的结构。维护的方式是使用上溯算法；
	*		上溯算法：如果子节点比父节点大，那么夫子节点交换，直到子节点比父节点大的条件不成立或者到达根节点；使用while实现上溯算法
	*	push_heap调用结构
	*		push_heap->__push_heap_aux->__push_heap;上溯算法在__push_heap中实现
	* 
	* - pop_heap
	*	作用：从完全二叉树中去除根节点，但是放到数组的末端
	* 
	*	这个max-tree的作用是为了实现一个primary queue，而primary queue每次pop的时候都是pop出最大的那个值；
	*	所以这个pop_heap每次pop出去的都是根节点；
	*	pop通过上溯来实现，即从根节点比较左右子节点那个更大，将更大的那个子节点和父节点交换，然后继续以交换后的父节点继续比较。
	*	上溯时一个迭代的过程，直到根节点找到一个无左右节点的地方才会停止，那么就会有一个子节点为本来根节点的值；
	*	那么将最底层最右边的节点作为value,以交换到底层的根节点作为节点，值为value，重新push，就可以；
	* 
	*	可以看作每次pop的时候都会发生：
	*		1.根节点放到最后
	*		2.最底层最右边的节点重新插入
	*	每次pop时都会有根节点消失  并且  最右边最底层的节点会重新push.
	*	所以每次pop的时候都会有
	*		1. 树中从逻辑上来说少一个节点,但是数组未改变大小
			2.最右边最底层的节点变为根节点，而树也少了最右边最底层的节点
		// 每次pop，都会少一个最底层最右边的节点;但是这个节点并不是消失了，而是换成了最大的根节点的值
	* 
	*	注意：	每次pop的时候，最。大的根节点不是消失了
	*			而是放到了数组的末端
	* 
	* - sort_heap
	*	由于每次pop的时候都是最大值,pop之后会将最大值放到最后，那么排序就只需要不停pop就好了；
	*	但是每次都会少一个节点，排序完成之后，树从逻辑上来说也就不存在了
	* 
	* - make_heap
	*	将一个数组的range转换为一个compelete binary tree;
	*   数组左边的都是根节点，而数组右边的都是子节点，数组(size-2)/2就是根节点;
	*	那么对每个根节点进行__adjust_heap就可以创造一个出来;
	*	__adjust_heap的作用就是重排子树
	* 
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	

	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compare comp);
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap_aux(RandomIter first, RandomIter last, Distance*, T*, Compare comp);
	template<typename RandomIter, typename Compare>
	inline void push_heap(RandomIter first, RandomIter last, Compare comp);


	// push_heap

// push_heap函数
	template<typename RandomIter, typename Compare>
	inline void push_heap(RandomIter first, RandomIter last, Compare comp = std::less<typename decltype(value_type(first))>) {
		// 首先检查输入是否有效
		if (first == last) return; // 空区域无需操作
		if (first + 1 == last) return; // 只有一个元素无需操作
		// 然后调用__push_heap_aux函数
		__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
	}

	// __push_heap_aux函数
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap_aux(RandomIter first, RandomIter last, Distance*, T*, Compare comp) {
		// 首先获取新插入元素的位置和值
		auto pos = last - 1; // 新插入的元素的位置是最后一个元素
		auto value = *pos; // 新插入的元素的值
		// 然后调用__push_heap函数
		__push_heap(first, Distance((last - first) - 1), Distance(0), value, comp);
	}

	// __push_heap函数
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __push_heap(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compare comp) {
		// 首先找到父节点的位置
		Distance parent = (holeIndex - 1) / 2;
		// 然后进行上溯算法，如果新值大于其父节点（根据比较器对象判断），则将父节点下移，并继续向上比较直到根节点或者遇到比新值小的节点为止
		while (holeIndex > topIndex && comp(*(first + parent), value)) {
			*(first + holeIndex) = *(first + parent); // 将父节点的值赋给子节点
			holeIndex = parent; // 更新子节点的位置为父节点的位置
			parent = (holeIndex - 1) / 2; // 更新父节点的位置
		}
		// 最后将新值插入到正确的位置
		*(first + holeIndex) = value;
	}



	// pop_heap

	template<typename RandomIter, typename T, typename Compare>
	inline void __pop_heap_aux(RandomIter first, RandomIter last, T*, Compare comp);
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compare comp);
	template<typename RandomIter, typename T, typename Distance, typename Compare>
	inline void __pop_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, Compare comp);

// pop_heap函数
	template<typename RandomIter, typename Compare>
	inline void pop_heap(RandomIter first, RandomIter last, Compare comp = std::less<typename decltype(value_type(first))>) {
		// 首先检查输入是否有效
		if (first == last) return; // 空区域无需操作
		if (first + 1 == last) return; // 只有一个元素无需操作
		// 然后调用__pop_heap_aux函数
		__pop_heap_aux(first, last, value_type(first), comp);
	}

	// __pop_heap_aux函数
	template<typename RandomIter, typename T, typename Compare>
	inline void __pop_heap_aux(RandomIter first, RandomIter last, T*, Compare comp) {
		// 首先获取最大值的位置和值
		auto result = last - 1; // 最大值的位置是最后一个元素
		auto value = *result; // 最大值的值
		// 然后调用__pop_heap函数
		__pop_heap(first, result, result, value, distance_type(first), comp);
	}

	// __pop_heap函数
	template<typename RandomIter, typename T, typename Distance, typename Compare>
	inline void __pop_heap(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, Compare comp) {
		// 首先将最大值放到最后一个元素的位置
		*result = *first;
		// 然后调用__adjust_heap函数来对第一个元素进行下沉操作
		__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
	}

	// __adjust_heap函数
	template<typename RandomIter, typename Distance, typename T, typename Compare>
	inline void __adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compare comp) {
		// 首先获取根节点的位置
		Distance topIndex = holeIndex;
		// 然后进行下沉操作，从根节点比较左右子节点哪个更大（根据比较器对象判断），将更大的那个子节点和父节点交换，然后继续以交换后的父节点继续比较
		while (holeIndex * 2 + 1 < len) { // 当还有左子节点时
			auto left = holeIndex * 2 + 1; // 计算左子节点的位置
			auto right = left + 1; // 计算右子节点的位置
			if (right < len && comp(*(first + left), *(first + right))) { // 如果右子节点存在且比左子节点大
				left = right; // 更新最大子节点的位置为右子节点的位置
			}
			if (comp(*(first + left), value)) { // 如果最大子节点的值小于要下沉的值，说明已经满足最大堆的条件，无需继续下沉
				break;
			}
			*(first + holeIndex) = *(first + left); // 将最大子节点的值赋给父节点
			holeIndex = left; // 更新父节点的位置为最大子节点的位置
		}
		// 最后调用__push_heap函数来将最后一个元素重新push到堆中
		__push_heap(first, holeIndex, topIndex, value, comp);
	}


	// sort_heap
	template<typename RandomIter>
	void sort_heap(RandomIter first, RandomIter last);
	template<typename RandomIter>
	void sort_heap(RandomIter first, RandomIter last) {
		while (last - first > 1) {
			pop_heap(first,last--);		// 每次pop，都会少一个最底层最右边的节点;但是这个节点并不是消失了，而是换成了最大的根节点的值
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
		// 找到头部
		Distance holeIndex = (len - 2) / 2;
		// 重排根节点
		while (true) {
			__adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
			if (holeIndex == 0)return;
			holeIndex--;
		}
	}

};

#endif