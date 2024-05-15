#pragma once
#ifndef LIST_H
#define LIST_H
#include<algorithm>

namespace mySimpleSTL {


	template <typename T>
	class list {


	private:				// 内部类型
		struct list_node;
		typedef list_node* link_type;
		link_type node;

			
	public:				// 型别设置
		typedef mySimpleSTL::allocator<T> alloc;

		typedef alloc::value_type  value_type;
		typedef alloc::reference reference;
		typedef alloc::pointer pointer;


		// 为了适配stack,queue
		typedef typename alloc::size_type		size_type;
		typedef typename alloc::const_reference	const_reference;
		
	public:				// 内部类型,构造函数
		class iterator;
		class const_iterator;

		list();			// 构造，并且自己创建一个空节点
		list(const list& other);

		list(list&& other) noexcept {
			node = other.node;
			other.node = nullptr;
		}

		list& operator=(const list& other) {
			if (this != &other) {
				clear();
				for (auto it = other.cbegin(); it != other.cend(); ++it) {
					push_back(*it);
				}
			}
			return *this;
		}

		list& operator=(list&& other) noexcept {
			if (this != &other) {
				clear();
				node = other.node;
				other.node = nullptr;
			}
			return *this;
		}
		
		list(const std::initializer_list<T>& il);
			

		~list();
	public:				// 基本访问函数
		void clear();
		bool empty() const;
		size_t size() const;
		reference front();
		reference back();
		iterator begin();
		const_iterator cbegin();
		iterator end();
		const_iterator cend();
	public:				// 操作元素函数
		iterator insert(iterator position, const T& x);
		void push_back(const T& x);
		void push_front(const T& x);
		iterator erase(iterator pos);
		void pop_front();
		void pop_back();
		void remove(const T& value);		// 会释放节点
		void unique();						// 如果
	private:		// 分配，构造，释放函数
		link_type get_node();
		void release_node(link_type ptr);			// 释放节点，在erase，unique，remove的时候都会调用这个函数
		link_type create_node(const T& val);
		void destory_node(link_type ptr);

		
	private:			// 以下操作是迁移操作
		static void transfer(iterator position, iterator first, iterator last); // 将[first,last)的内容放到 [position 之前
	public:
		static void splice(iterator pos, list& e); 			// 将list接到pos前面,功能完成
		static void splice(iterator pos,list&,iterator i); // 将i所指向的元素放在pos之前
		static void splice(iterator pos,list&,iterator start,iterator last); // 转调用
	public:
		void sort();				// 本来是想按照标准库那样子调用merge,swap实现，但就成功不了。
									// 写累了，用vector暴力把;
									// 时间是这样: 遍历+vector排序+析构+重新构建
									// 优化了之后： 析构的同时遍历+vector排序+重新构建;
		void merge(list& x);
		void reverse();
	private:
		void swap(list& x);
	};



	template<typename T>
	struct list<T>::list_node {
		T data;
		list_node* prev;
		list_node* next;
		list_node(const T& d = T(), list_node* p = nullptr, list_node* n = nullptr)
			: data(d), prev(p), next(n) {}
	};

	template<typename T>
	class list<T>::iterator {
	public:
		typedef iterator self;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;
		link_type node;
		iterator(link_type x = nullptr) : node(x) {}
		iterator(const iterator& x) : node(x.node) {}
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		reference operator*() const { return node->data; }
		pointer operator->() const { return &(operator*()); }
		self& operator+(int v) {
			while (v > 0) {
				++*this;
				--v;
			}
			return *this;
		}
		self& operator++() {
			node = node->next;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--() {
			node = node->prev;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}
	};

	template<typename T>
	class list<T>::const_iterator {
	public:
		typedef const_iterator self;
		typedef T value_type;
		typedef const T& reference;
		typedef const T* pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef std::ptrdiff_t difference_type;
		link_type node;
		const_iterator(link_type x = nullptr) : node(x) {}
		const_iterator(const const_iterator& x) : node(x.node) {}
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		reference operator*() const { return node->data; }
		pointer operator->() const { return &(operator*()); }
		self& operator++() {
			node = node->next;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--() {
			node = node->prev;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}

		const_iterator& operator=(const iterator& it) {
			node = it.node;
			return *this;
		}
	};

	template<typename T>
	typename list<T>::link_type list<T>::get_node() {
		return allocator<list_node>::allocate();
	}

	template<typename T>
	void list<T>::release_node(link_type ptr) {
		allocator<list_node>::deallocate(ptr);
	}

	template<typename T>
	typename list<T>::link_type list<T>::create_node(const T& val) {
		link_type p = get_node();
		construct(&p->data, val);
		return p;
	}

	template<typename T>
	void list<T>::destory_node(link_type ptr) {
		destroy(&ptr->data);
		release_node(ptr);
	}

	template<typename T>
	inline void list<T>::reverse()
	{
		if (node->next == node || node->next->next == node) {
			return;
		}
		link_type cur = node->next;
		link_type tmp = nullptr;
		while (cur != node) {
			tmp = cur->prev;
			cur->prev = cur->next;
			cur->next = tmp;
			cur = cur->prev;
		}
		tmp = node->prev;
		node->prev = node->next;
		node->next = tmp;
	}

	template<typename T>
	list<T>::list() {
		node = get_node();
		node->prev = node;
		node->next = node;
	}

	template<typename T>
	inline list<T>::list(const list& other)
		:list()
	{
		for (auto it = other.begin(); it != other.begin(); ++it) {
			push_back(*it);
		}
	}

	template<typename T>
	inline list<T>::list(const std::initializer_list<T>& il)
		:list() {
		for (auto it : il)push_back(it);
	}

	template<typename T>
	list<T>::~list() {
		clear();
		release_node(node);
	}

	template<typename T>
	void list<T>::clear() {
		if (node == nullptr)return;
		link_type cur = node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = cur->next;
			destory_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	template<typename T>
	bool list<T>::empty() const {
		return node->next == node;
	}

	template<typename T>
	size_t list<T>::size() const {
		size_t result = 0;
		link_type cur = node->next;
		while (cur != node) {
			++result;
			cur = cur->next;
		}
		return result;
	}

	template<typename T>
	list<T>::reference list<T>::front() {
		return *begin();
	}

	template<typename T>
	list<T>::reference list<T>::back() {
		return *(--end());
	}

	template<typename T>
	typename list<T>::iterator list<T>::begin() {
		return  iterator(node->next);
	}

	template<typename T>
	inline list<T>::const_iterator list<T>::cbegin()
	{
		return const_iterator(node->next);
	}

	template<typename T>
	typename list<T>::iterator list<T>::end() {
		return iterator(node);
	}

	template<typename T>
	inline list<T>::const_iterator list<T>::cend()
	{
		return const_iterator(node);
	}

	template<typename T>
	typename list<T>::iterator list<T>::insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		position.node->prev->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}

	template<typename T>
	void list<T>::push_back(const T& x) {
		insert(end(), x);
	}

	template<typename T>
	inline void list<T>::push_front(const T& x)
	{
			insert(begin(), x);
	}

	template<typename T>
	inline list<T>::iterator list<T>::erase(iterator pos)
	{

		link_type p = pos.node;
		p->prev->next = p->next;
		p->next->prev = p->prev;
		destroy(&p->data);
		release_node(p);
		return iterator(p->next);
	}

	template<typename T>
	inline void list<T>::pop_front()
	{
		erase(begin());
	}

	template<typename T>
	inline void list<T>::pop_back()
	{

		iterator tmp = end();
		erase(--tmp);
	}

	template<typename T>
	inline void list<T>::remove(const T& value)
	{

		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (*first == value) erase(first);
			first = next;
		}

	}
	template<typename T>
	inline void list<T>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last) {
			if (*first == *next) {
				erase(next);
			}
			else {
				first = next;
			}
			next = first;
		}

	};
	template<typename T>
	 void list<T>::transfer(list<T>::iterator position, list<T>::iterator first, list<T>::iterator last)
	{
		if (position == last) return;
		link_type p = position.node;
		link_type f = first.node;
		link_type l = last.node->prev;
		p->prev->next = f;
		f->prev->next = last.node;
		last.node->prev->next = p;
		link_type tmp = p->prev;
		p->prev = l;
		l->next = p;
		f->prev = tmp;
		return;
	}
	 template<typename T>
	 inline void list<T>::splice(iterator pos, list& e)
	 {
		 if (e.empty())return;
		 transfer(pos, e.begin(), e.end());
	 }
	 template<typename T>
	 inline void list<T>::splice(iterator pos, list&, iterator i)
	 {
		 iterator j = i;
		 ++j;
		 
		 if (pos == i || pos == j)return;
		 transfer(pos,i,j);
	 }
	 template<typename T>
	 inline void list<T>::splice(iterator pos, list&, iterator start, iterator last)
	 {
		 if (start != last)return;
		 transfer(pos,start,last);
	 };

	template<class T>
	void list<T>::merge(list<T>& x)
	{
		if (this == &x) {
			return;
		}
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else {
				++first1;
			}
		}
		if (first2 != last2) {
			transfer(last1, first2, last2);
		}
	};

	template<class T>
	void list<T>::sort() {						// 累了，暴力把
		mySimpleSTL::vector<int> values;
		
		//for (auto it : *this)
			//values.push_back(it);
				// 还是,优化了一下...
		if (node == nullptr)return;
		link_type cur = node->next;
		while (cur != node) {
										values.push_back(cur->data);
			link_type tmp = cur;
			cur = cur->next;
			destory_node(tmp);
		}
		node->next = node;
		node->prev = node;

		std::sort(values.begin(), values.end());

		//this->clear();
		for (auto it : values)
			this->push_back(it);
	}


	template<typename T>
	void list<T>::swap(list<T>& x) {
		mySimpleSTL::swap(node, x.node);
	};
};


/*  实例1-成功：
     mySimpleSTL::list<int> l;
    for (int i = 0; i < 10; i++) {
        l.push_back(i);
    }
    std::cout << "size:" << l.size() << std::endl;
    std::cout << "front:" << l.front() << std::endl;
    std::cout << "back:" << l.back() << std::endl;
    l.pop_front();
    std::cout << "size:" << l.size() << std::endl;
    std::cout << "front:" << l.front() << std::endl;
    std::cout << "back:" << l.back() << std::endl;
    l.pop_back();
    std::cout << "size:" << l.size() << std::endl;
    std::cout << "front:" << l.front() << std::endl;
    std::cout << "back:" << l.back() << std::endl;
    l.remove(5);
    std::cout << "size:" << l.size() << std::endl;
    std::cout << "front:" << l.front() << std::endl;
    std::cout << "back:" << l.back() << std::endl;
    l.unique();
    std::cout << "size:" << l.size() << std::endl;
    std::cout << "front:" << l.front() << std::endl;
    std::cout << "back:" << l.back() << std::endl;
*/

#endif