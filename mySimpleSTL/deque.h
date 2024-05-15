#pragma once
#ifndef MYSIMPLESTL_DEQUE_H
#define MYSIMPLESTL_DEQUE_H

#include"iterator.h"
#include"allocator.h"

namespace mySimpleSTL {


	/*
	* ���������ص�ʵ���ڣ�
	*   +=  ++
	* ����������������
	*/

	// ���Ǹ������㣬����ģ�峣����ʱ�����ʹ�����ַ�ʽ����
	template<typename T>
	struct __deque_buf_size
	{
		static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
	};
	template<typename T, typename Ref, typename Ptr>
	class deque_iterator : public iterator<random_access_iterator_tag, T> {     // ����ֱ�Ӽ̳�random_access_iterator

		// iterator�ĸ���typedef
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
		typedef mySimpleSTL::random_access_iterator_tag iterator_category;          // ע�⣬�������ṩ��ʱrandom_iterator
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
		void set_node(map_pointer new_node) {           // �����п����ģ�ͬʱ�ı�first,last
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


	// Ĭ�Ϲ�����ΪmySimpleSTL::allocator<T>
	template<typename T>
	class deque {
	public:    // һЩ����typedef �Ķ���
		// ����
		typedef mySimpleSTL::allocator<T>                      allocator_type;
		typedef mySimpleSTL::allocator<T>                      data_allocator;
		typedef mySimpleSTL::allocator<T*>                     map_allocator;
		// ����Ƕ�׶���
		typedef typename allocator_type::value_type      value_type;
		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;
		typedef pointer* map_pointer;
		typedef const_pointer* const_map_pointer;
		// ������Ƕ�׶���
		typedef deque_iterator<T, T&, T*>                iterator;
		typedef deque_iterator<T, const T&, const T*>    const_iterator;
		typedef mySimpleSTL::reverse_iterator<iterator>        reverse_iterator;
		typedef mySimpleSTL::reverse_iterator<const_iterator>  const_reverse_iterator;
		// ����buffer��С
		static const size_type buffer_size = __deque_buf_size<T>::value;
	private:
		// ���ݳ�Ա
		iterator    start;
		iterator    finish;
		map_pointer map;
		size_type   map_size;

	private:     // ���캯���Ĺ��ߺ���
		void destroy_map_and_buffers();               // ɾ��map��node
		void create_map_and_buffers(size_type num_elements);  // ����map��node
		void fill_initialize(size_type n, const value_type& value);     // ����n��value��Ԫ��
		void clear();

	public:     // ���캯��
		deque(size_type n, const value_type& value)
			:start(), finish(), map(nullptr), map_size(0)       // map�������������õģ�������create_map_and_buffers��������
		{
			this->fill_initialize(n, value);
		};
		deque(size_type n) :deque(n, value_type()) {}
		deque() :deque(0) {}		// ���ﲻ�ܼ򵥵�д8����Ϊд��8��ʾ��8��Ԫ�أ���ô�ͻ����pushһ��1,����front�Ǹ�Ĭ�Ϲ�������ĵ�8��Ԫ�صĵ�һ��
									// push��1��ʵ�ǵھŸ�Ԫ�أ�ǰ��8������Ĭ�Ϲ��캯��
		~deque();




	public:
		// �򵥷��ʹ��ܣ�  begin,end,operator[],front,back,size,max_size,empty
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

		// �޸�������ز���
	public:
		void push_back(const T& val);
		void push_front(const T& val);
	private:
		void push_front_aux(const T& val);		
		void push_back_aux(const T& val);	// ��ʹ��reserve_map_at_back��֤map���нڵ����ʹ�õģ�Ȼ�����һ��buffer���ٹ���һ��val
		void reserve_map_at_front(size_type nodes_to_add = 1);				// ���mapǰ�˽ڵ��Ƿ��㣬����������reallocate_map
		void reserve_map_at_back(size_type nodes_to_add = 1);				// ���map��˽ڵ��Ƿ���
		void reallocate_map(size_type nodes_to_add,bool add_at_front);		// �������·���map,map������������Ȼ��ڵ����
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* push_back
		* ��finish.cur������ݣ�
		* ��finish.cur== finish.last����ô����push_front_aux
		* reserve_map_at_back����map�ռ�,ȷ��map֮���пռ���ã�Ȼ��lastָ����һ��buffer,������Ԫ�أ�������������
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		void pop_back();
		void pop_front();
	private:
		void pop_back_aux();
		void pop_front_aux();
		/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* pop_back
		* ���ʵ�ֲ������ӣ���������߽�ֻ��Ҫ�ƶ�node�ͺã�
		* û�б߽�����ֻ��Ҫ����destroy,����ָ��
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


		// erase
	public:
		iterator erase(iterator pos);
		iterator erase(iterator first,iterator last);


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
		* erase
		* ����ȷ��Ҫɾ��������
		* �ж�ɾ������ǰ���Ԫ���٣����Ǻ����Ԫ���٣�
		* �����ڵ���������ƣ�����ֱ��ʹ���㷨�����ؿ��ǷֶεĽṹ
		* ����Ԫ�صĶ��٣������Ǵ�ǰ���ƶ����Ǻ����ƶ�; �ƶ�ͨ��copy�㷨ʵ��
		* * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// insert
	public:
		iterator insert(iterator pos, const value_type& x);
	private:
		iterator insert_aux(iterator pos, const value_type& x);
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		* insert
		* ���ж�������ڿ�ͷ�ͽ�β���Ԫ�ص����
		* ������ǣ���ôִ��������insert
		* �����ж�ǰ���Ԫ���٣����Ǻ����Ԫ���٣�
		* Ȼ��ͨ��copy�ƶ���ʵ��insert
		* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	


// ---------------------------------������ʵ�ֺ���------------------------------------------------
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
		// �õ������֮ǰ��Ԫ�ظ���
		difference_type index = pos - start;	// �õ������֮ǰ��Ԫ�ظ���

		if (index < (size() >> 1)) {	// ���ǰ���Ԫ���٣����ƶ�֮ǰ��Ԫ��
			push_front(front());	// ��ǰ�����һ��Ԫ��
			iterator front1 = start;	// �µ�start
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;	// ���¶�λpos
			iterator pos1 = pos;
			++pos1;
			mySimpleSTL::copy(front2, pos1, front1);	// ��pos֮ǰ��Ԫ������ƶ�һ��λ��
		}
		else {	// ��������Ԫ���٣����ƶ�֮���Ԫ��
			push_back(back());	// �ں������һ��Ԫ��
			iterator back1 = finish;	// �µ�finish
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;	// ���¶�λpos
			mySimpleSTL::copy_backward(pos, back2, back1);	// ��pos֮���Ԫ����ǰ�ƶ�һ��λ��
		}
		*pos = x;	// ������Ԫ��
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
		else {		// ���Ҫpop�������buffer�����һ��
			pop_front_aux();
		}
	}

	template<typename T>
	inline void deque<T>::pop_front_aux()
	{
		data_allocator::destroy(start.cur);  // delete ���������������
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
	template<typename T>
	void deque<T>::pop_back() {
		if (finish.cur != finish.first) {
			data_allocator::destroy(--finish.cur);
		}
		else			// ��bufferû��һ��Ԫ��,�������˼��Ҫpop��һ��buffer�����һ��Ԫ��
			pop_back_aux();
	}
	template<typename T>
	void deque<T>::pop_back_aux() {
		//data_allocator::destroy(finish.cur);
		data_allocator::deallocate(finish.cur);  // delete ���������������

		finish.set_node(finish.node-1);
		finish.cur = finish.last - 1;
		data_allocator::destroy(finish.cur);

	}


	template<typename T>
	void deque<T>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
		// ���ʹ�õ���
		size_type old_node_size = finish.node - start.node + 1;
		size_type new_node_size = old_node_size + nodes_to_add;
		
		
		map_pointer new_start;
		if (map_size > new_node_size * 2) {  // ���map����Ľڵ����͹�
			// ��node��λ�þ���
			new_start = map + (map_size / 2 - new_node_size) + (add_at_front ? nodes_to_add : 0); // �ҵ��м�ƫ���λ��

			if (new_start < start.node)		// �µ�λ����start����ߣ��Ǿ���Ҫ������Ŀ��ƶ�λ��
				std::copy(start.node, finish.node + 1, new_start + nodes_to_add);
			else
				std::copy_backward(start.node, finish.node + 1, new_start + old_node_size);
		}
		else {

			size_type new_map_size = map_size + mySimpleSTL::max(map_size,nodes_to_add) + 2;		// 2������
			// ����һ��ռ�
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (map_size - new_node_size) / 2 + (add_at_front ? nodes_to_add : 0);

			// �����ռ�
			std::copy(start.node, finish.node + 1, new_start);
			
			//�ͷſռ�
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
		//if (finish.cur != finish.cur - 1) {		// ��������
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
		// 1.�ж�map��û������������ˣ���ô����һ���µĺ�����map����ռ�
		// 2.���û��������ô��map�ĺ�һ���ڵ����һ��buffer
		// 3.���µ�buffer���湹��һ��Ԫ��
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
		size_type num_nodes = num_elements / buffer_size + 1;   // ������Ҫ���ٸ��ڵ�
		map_size = mySimpleSTL::max(size_type(8), num_nodes + 2);      // ����map�Ĵ�С,����Ҫ8��map�ڵ�
		map = map_allocator::allocate(map_size);                // ����map�Ŀռ�

		// ʹnstart��nfinishָ��map��ӵ�е��п�����
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try {
			// Ϊÿ���ڵ����ռ�

			for (cur = nstart; cur <= nfinish; ++cur) {
				*cur = data_allocator::allocate(buffer_size);
			}

		}
		catch (...) {
			// ��������쳣���ͽ��Ѿ�����õĿռ�ȫ���ͷŵ�
			for (map_pointer tmp = nstart; tmp < cur; ++tmp) {
				data_allocator::deallocate(*tmp, buffer_size);
			}
			map_allocator::deallocate(map, map_size);
			throw;
		}

		// ����start��finish�ĵ�����
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
			// ��������쳣���ͽ��Ѿ�����õĶ���ȫ��������
			for (map_pointer n = start.node; n < cur; ++n) {
				data_allocator::destroy(*n, *n + buffer_size);
			}
			destroy_map_and_buffers();
			throw;
		}

	}

	// ��������
	// 1. �ȵ���clear(),��mapָ�������buffer������Ԫ���ͷŵ�
	// 2. Ȼ���ͷŵ�map����
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