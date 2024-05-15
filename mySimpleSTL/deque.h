#pragma once
#ifndef MYSIMPLESTL_DEQUE_H
#define MYSIMPLESTL_DEQUE_H

#include"iterator.h"
#include"allocator.h"

namespace mySimpleSTL {


	/*
	* 迭代器的重点实现在，
	*   +=  ++
	* 这两个操作符身上
	*/

	// 这是个技术点，定义模板常量的时候可以使用这种方式定义
	template<typename T>
	struct __deque_buf_size
	{
		static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
	};
	template<typename T, typename Ref, typename Ptr>
	class deque_iterator : public iterator<random_access_iterator_tag, T> {     // 这里直接继承random_access_iterator

		// iterator的各种typedef
	public:
		typedef deque_iterator<T, Ref, Ptr> iterator;
		typedef deque_iterator<T, const T&, const T*> const_iterator;


		typedef T            value_type;
		typedef Ptr          pointer;
		typedef Ref          reference;
		typedef size_t       size_type;
		typedef ptrdiff_t    difference_type;
		typedef T* value_pointer;
		typedef T** map_pointer;

		static const size_type buffer_size = __deque_buf_size<T>::value;

	public:
		typedef mySimpleSTL::random_access_iterator_tag iterator_category;          // 注意，这里它提供的时random_iterator
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;
		typedef deque_iterator self;
	public:
		T* cur;
		T* first;
		T* last;
		map_pointer node;
	public:
		deque_iterator(T* x, map_pointer y) :cur(x), first(*y), last(*y + buffer_size), node(y) {}
		deque_iterator() :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
		deque_iterator(const iterator& x) :cur(x.cur), first(x.first), last(x.last), node(x.node) {}
	public:
		void set_node(map_pointer new_node) {           // 设置中控中心，同时改变first,last
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size);
		}
	public:
		reference operator*()const {
			return *cur;
		}
		pointer operator->()const {
			return &(operator*());
		}
		difference_type operator-(const self& x)const {
			return difference_type(buffer_size) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		self& operator++() {
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}



		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}
		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size)) {
				cur += n;
			}
			else {
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size) : -difference_type((-offset - 1) / buffer_size) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset * difference_type(buffer_size));
			}
			return *this;
		}
		self operator+(difference_type n)const {
			self tmp = *this;
			return tmp += n;
		}
		self& operator-=(difference_type n) {
			return *this += -n;
		}
		self operator-(difference_type n)const {
			self tmp = *this;
			return tmp -= n;
		}
		reference operator[](difference_type n)const {
			return *(*this + n);
		}
		bool operator==(const self& x)const {
			return cur == x.cur;
		}
		bool operator!=(const self& x)const {
			return !(*this == x);
		}
		bool operator<(const self& x)const {
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}

	};


	// 默认构造器为mySimpleSTL::allocator<T>
	template<typename T>
	class deque {
	public:    // 一些关于typedef 的定义
		// 构造
		typedef mySimpleSTL::allocator<T>                      allocator_type;
		typedef mySimpleSTL::allocator<T>                      data_allocator;
		typedef mySimpleSTL::allocator<T*>                     map_allocator;
		// 基本嵌套定义
		typedef typename allocator_type::value_type      value_type;
		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;
		typedef pointer* map_pointer;
		typedef const_pointer* const_map_pointer;
		// 迭代器嵌套定义
		typedef deque_iterator<T, T&, T*>                iterator;
		typedef deque_iterator<T, const T&, const T*>    const_iterator;
		typedef mySimpleSTL::reverse_iterator<iterator>        reverse_iterator;
		typedef mySimpleSTL::reverse_iterator<const_iterator>  const_reverse_iterator;
		// 定义buffer大小
		static const size_type buffer_size = __deque_buf_size<T>::value;
	private:
		// 数据成员
		iterator    start;
		iterator    finish;
		map_pointer map;
		size_type   map_size;

	private:     // 构造函数的工具函数
		void destroy_map_and_buffers();               // 删除map和node
		void create_map_and_buffers(size_type num_elements);  // 构建map和node
		void fill_initialize(size_type n, const value_type& value);     // 创建n个value的元素
		void clear();

	public:     // 构造函数
		deque(size_type n, const value_type& value)
			:start(), finish(), map(nullptr), map_size(0)       // map不是在这里设置的，而是在create_map_and_buffers里面设置
		{
			this->fill_initialize(n, value);
		};
		deque(size_type n) :deque(n, value_type()) {}
		deque() :deque(0) {}		// 这里不能简单的写8，因为写了8表示有8个元素，那么就会出现push一个1,但是front是个默认构造出来的的8个元素的第一个
									// push的1其实是第九个元素，前面8个都是默认构造函数
		~deque();




	public:
		// 简单访问功能，  begin,end,operator[],front,back,size,max_size,empty
		iterator begin() const { return start; };
		const_iterator cbegin() const { return const_iterator(start); };
		iterator end() const { return finish; };
		const_iterator cend() const { return const_iterator(finish); };
		reference front() { return *(start.cur); };
		reference back() { return *(finish.cur - 1); };
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); }
		bool empty() const { return start == finish; }
		reference operator[](size_t n) { return *(start + n); };

		// 修改容器相关操作
	public:
		void push_back(const T& val);
		void push_front(const T& val);
	private:
		void push_front_aux(const T& val);		
		void push_back_aux(const T& val);	// 先使用reserve_map_at_back保证map是有节点可以使用的，然后分配一个buffer，再构建一个val
		void reserve_map_at_front(size_type nodes_to_add = 1);				// 检查map前端节点是否不足，如果不足调用reallocate_map
		void reserve_map_at_back(size_type nodes_to_add = 1);				// 检查map后端节点是否不足
		void reallocate_map(size_type nodes_to_add,bool add_at_front);		// 用于重新分配map,map是两倍增长，然后节点居中
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* push_back
		* 在finish.cur添加数据；
		* 若finish.cur== finish.last，那么调用push_front_aux
		* reserve_map_at_back处理map空间,确保map之中有空间可用，然后last指向下一个buffer,再新增元素，最后调整迭代器
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		void pop_back();
		void pop_front();
	private:
		void pop_back_aux();
		void pop_front_aux();
		/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* pop_back
		* 这个实现并不复杂，如果遇到边界只需要移动node就好；
		* 没有边界的情况只需要调用destroy,调整指针
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		// erase
	public:
		iterator erase(iterator pos);
		iterator erase(iterator first,iterator last);


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
		* erase
		* 首先确定要删除的区间
		* 判断删除区间前面的元素少，还是后面的元素少；
		* 得益于迭代器的设计，可以直接使用算法而不必考虑分段的结构
		* 根据元素的多少，决定是从前面移动还是后面移动; 移动通过copy算法实现
		* * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// insert
	public:
		iterator insert(iterator pos, const value_type& x);
	private:
		iterator insert_aux(iterator pos, const value_type& x);
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* insert
		* 先判断特殊的在开头和结尾添加元素的情况
		* 如果不是，那么执行正常的insert
		* 首先判断前面的元素少，还是后面的元素少；
		* 然后通过copy移动来实现insert
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	


// ---------------------------------以下是实现函数------------------------------------------------
	template<typename T>
	inline deque<T>::iterator mySimpleSTL::deque<T>::insert(iterator pos, const value_type& x)
	{
		if (pos == start) {
			push_front(x);
			return start;
		}
		else if (pos == finish) {
			push_back(x);
			return finish;
		}
		return insert_aux(pos,x);
	}
	template<typename T>
	inline deque<T>::iterator mySimpleSTL::deque<T>::insert_aux(iterator pos, const value_type& x)
	{
		// 得到插入点之前的元素个数
		difference_type index = pos - start;	// 得到插入点之前的元素个数

		if (index < (size() >> 1)) {	// 如果前面的元素少，就移动之前的元素
			push_front(front());	// 在前面添加一个元素
			iterator front1 = start;	// 新的start
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;	// 重新定位pos
			iterator pos1 = pos;
			++pos1;
			mySimpleSTL::copy(front2, pos1, front1);	// 将pos之前的元素向后移动一个位置
		}
		else {	// 如果后面的元素少，就移动之后的元素
			push_back(back());	// 在后面添加一个元素
			iterator back1 = finish;	// 新的finish
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;	// 重新定位pos
			mySimpleSTL::copy_backward(pos, back2, back1);	// 将pos之后的元素向前移动一个位置
		}
		*pos = x;	// 插入新元素
		return pos;
	}
	template<typename T>
	inline typename deque<T>::iterator deque<T>::erase(iterator pos)
	{
		difference_type index = pos - start;

		if (index < (size() >> 1)) {
			mySimpleSTL::copy_backward(start, pos, pos + 1);
			pop_front();
		}
		else {
			std::copy(pos + 1, finish, pos);
			pop_back();
		}
		return start + index;
	}

	template<typename T>
	inline typename deque<T>::iterator deque<T>::erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
			this->clear();

		difference_type len = last - first;
		difference_type elems_before = first - start;
		if (elems_before < (size() - len) / 2) {
			copy_backward(start, first, last);
			iterator new_start = start + len;
			destroy(start, new_start);
			start = new_start;
		}
		else {
			copy(last, finish, first);
			iterator new_finish = finish - len;
			destroy(new_finish, finish);
			finish = new_finish;
		}
		return start + elems_before;
	}

	template<typename T>
	inline void mySimpleSTL::deque<T>::pop_front()
	{
		if (start.cur != start.last - 1) {
			data_allocator::destroy(start.cur);
			start.cur++;
		}
		else {		// 如果要pop的是这个buffer的最后一个
			pop_front_aux();
		}
	}

	template<typename T>
	inline void deque<T>::pop_front_aux()
	{
		data_allocator::destroy(start.cur);  // delete 会调用析构函数的
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
	template<typename T>
	void deque<T>::pop_back() {
		if (finish.cur != finish.first) {
			data_allocator::destroy(--finish.cur);
		}
		else			// 该buffer没有一个元素,这里的意思是要pop上一个buffer的最后一个元素
			pop_back_aux();
	}
	template<typename T>
	void deque<T>::pop_back_aux() {
		//data_allocator::destroy(finish.cur);
		data_allocator::deallocate(finish.cur);  // delete 会调用析构函数的

		finish.set_node(finish.node-1);
		finish.cur = finish.last - 1;
		data_allocator::destroy(finish.cur);

	}


	template<typename T>
	void deque<T>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
		// 获得使用的量
		size_type old_node_size = finish.node - start.node + 1;
		size_type new_node_size = old_node_size + nodes_to_add;
		
		
		map_pointer new_start;
		if (map_size > new_node_size * 2) {  // 如果map本身的节点数就够
			// 将node的位置居中
			new_start = map + (map_size / 2 - new_node_size) + (add_at_front ? nodes_to_add : 0); // 找到中间偏左的位置

			if (new_start < start.node)		// 新的位置在start的左边，那就需要将本身的块移动位置
				std::copy(start.node, finish.node + 1, new_start + nodes_to_add);
			else
				std::copy_backward(start.node, finish.node + 1, new_start + old_node_size);
		}
		else {

			size_type new_map_size = map_size + mySimpleSTL::max(map_size,nodes_to_add) + 2;		// 2倍增长
			// 配置一块空间
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (map_size - new_node_size) / 2 + (add_at_front ? nodes_to_add : 0);

			// 拷贝空间
			std::copy(start.node, finish.node + 1, new_start);
			
			//释放空间
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;

		}

		start.set_node(new_start);
		finish.set_node(new_start + old_node_size - 1);
		
	}

	template<typename T>
	void deque<T>::reserve_map_at_front(size_type nodes_to_add)
	{
		if (nodes_to_add > start.node - map) {
			reallocate_map(nodes_to_add, false);
		}
	}
	template<typename T>
	void deque<T>::reserve_map_at_back(size_type nodes_to_add)
	{
		if (nodes_to_add + 1 > map_size - (finish.node - map)) {
			reallocate_map(nodes_to_add, true);
		}
	}
	template<typename T>
	void deque<T>::push_back(const T& val) {
		//if (finish.cur != finish.cur - 1) {		// 这里问题
		if (finish.cur != finish.last - 1) {
			data_allocator::construct(finish.cur, val);
			++finish.cur;
		}
		else {
			push_back_aux(val);
		}
	};
	template<typename T>
	void deque<T>::push_front(const T& val) {
		if (start.cur != start.first) {
			data_allocator::construct(start.cur - 1, val);
			--start.cur;
		}
		else {
			push_front_aux(val);
		}
	}
	template<typename T>
	void deque<T>::push_front_aux(const T& val) {
		reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(buffer_size);
		try {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			data_allocator::construct(start.cur, val);
		}
		catch (...) {
			start.set_node(start.node + 1);
			start.cur = start.first;
			data_allocator::deallocate(*(start.node - 1), buffer_size);
			throw;
		}
	}

	template<typename T>
	void deque<T>::push_back_aux(const T& val) {
		// 1.判断map有没有满，如果满了，那么调用一个新的函数给map分配空间
		// 2.如果没有满，那么在map的后一个节点分配一个buffer
		// 3.在新的buffer里面构造一个元素
		reserve_map_at_back();
		*(finish.node + 1) = data_allocator::allocate(buffer_size);
		try {
			data_allocator::construct(finish.cur, val);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			data_allocator::deallocate(*(finish.node + 1), buffer_size);
			throw;
		}
	}
	template<typename T>
	inline void deque<T>::destroy_map_and_buffers()
	{
		for (map_pointer cur = start.node; cur <= finish.node; ++cur) {
			data_allocator::deallocate(*cur, buffer_size);
		}
		map_allocator::deallocate(map, map_size);
	}
	template<typename T>
	inline void deque<T>::create_map_and_buffers(size_type num_elements)
	{
		size_type num_nodes = num_elements / buffer_size + 1;   // 计算需要多少个节点
		map_size = mySimpleSTL::max(size_type(8), num_nodes + 2);      // 计算map的大小,至少要8个map节点
		map = map_allocator::allocate(map_size);                // 分配map的空间

		// 使nstart和nfinish指向map所拥有的中控中心
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try {
			// 为每个节点分配空间

			for (cur = nstart; cur <= nfinish; ++cur) {
				*cur = data_allocator::allocate(buffer_size);
			}

		}
		catch (...) {
			// 如果发生异常，就将已经分配好的空间全部释放掉
			for (map_pointer tmp = nstart; tmp < cur; ++tmp) {
				data_allocator::deallocate(*tmp, buffer_size);
			}
			map_allocator::deallocate(map, map_size);
			throw;
		}

		// 设置start和finish的迭代器
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size;
	}
	template<typename T>
	inline void deque<T>::fill_initialize(size_type n, const value_type& value)
	{

		create_map_and_buffers(n);
		map_pointer cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur) {
				mySimpleSTL::uninitialized_fill(*cur, *cur + buffer_size, value);
			}
			mySimpleSTL::uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...) {
			// 如果发生异常，就将已经构造好的对象全部析构掉
			for (map_pointer n = start.node; n < cur; ++n) {
				data_allocator::destroy(*n, *n + buffer_size);
			}
			destroy_map_and_buffers();
			throw;
		}

	}

	// 析构函数
	// 1. 先调用clear(),将map指向的所有buffer的所有元素释放掉
	// 2. 然后释放掉map本身
	template<typename T>
	inline deque<T>::~deque()
	{
		clear();
		destroy_map_and_buffers();
	}

	template<typename T>
	inline void deque<T>::clear()
	{
		for (map_pointer cur = start.node; cur < finish.node; ++cur) {
			data_allocator::destroy(*cur, *cur + buffer_size);
		}
		if (start.node != finish.node) {
			data_allocator::destroy(start.cur, start.last);
			data_allocator::destroy(finish.first, finish.cur);
		}
		else {
			data_allocator::destroy(start.cur, finish.cur);
		}
		finish = start;
	}
	;

};





#endif