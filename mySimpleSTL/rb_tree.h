#pragma once
#ifndef MYSIMPLESTL_RB_TREE_H
#define MYSIMPLESTL_RB_TREE_H

#include "iterator.h"
#include "allocator.h"


// 两层设计在转型的时候很麻烦，我直接整成一层算了。

namespace mySimpleSTL {

	// 节点设计
	typedef bool __rb_tree_color_type;
	constexpr const __rb_tree_color_type __rb_tree_red = true;
	constexpr const __rb_tree_color_type __rb_tree_black = false;
	
	template<typename T>
	struct __rb_tree_node {
	public:
		typedef __rb_tree_node* link_type;

	public:
		__rb_tree_color_type color;
		link_type parent;
		link_type left;
		link_type right;
		T val;
	public:
		static link_type minimum(link_type ptr);
		static link_type maximum(link_type ptr);
	};

	
	// 迭代器设计,同样也是将2层整成了一层
	template<typename T, typename Ref, typename Ptr>
	struct __rb_tree_iterator {
	public:
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_iterator<T, Ref, Ptr> self;
		typedef __rb_tree_iterator<T, T&, T*> iterator;
		typedef __rb_tree_iterator<T, const T&, const T*> const_iterator;
		typedef __rb_tree_node<T>* link_type;
		typedef typename mySimpleSTL::bidirectional_iterator_tag iterator_category;

	public:
		__rb_tree_iterator() {}
		__rb_tree_iterator(link_type ptr) { this->node = ptr; }
		__rb_tree_iterator(const iterator& iter) { node = iter.node; }


	public:
		link_type node;
	public:
		void increment();
		void decrement();
	public:
		reference operator*() const { return static_cast<link_type>(node)->val; }
		pointer operator->() const { return &(operator*()); }
		self& operator++() { increment(); return *this; }
		self operator++(int) { self tmp = *this; increment(); return tmp; }
		self& operator--() { decrement(); return *this; }
		self operator--(int) { self tmp = *this; decrement(); return tmp; }
		bool operator==(const iterator& e) { return this->node == e.node; }
		bool operator!=(const iterator& rv) { return this->node != rv.node; }
	};


	// 红黑树本身设计
	template<typename T, typename Compare = std::less<T>>
	class rb_tree {
	public:
		typedef void* void_pointer;
		typedef __rb_tree_node<T> rb_tree_node;
		typedef __rb_tree_color_type color_type;
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;
		typedef __rb_tree_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef mySimpleSTL::allocator<rb_tree_node> alloc_node;




		// 和节点创建析构相关的函数
	private:
		link_type get_node() { return alloc_node::allocate(); }
		void put_node(link_type ptr) { alloc_node::deallocate(ptr); }
		link_type create_node(const T& val) {
			link_type tmp = get_node();
			try {
				mySimpleSTL::allocator<T>::construct(&tmp->val, val);
			}
			catch (...) {
				put_node(tmp);
				throw;
			}
			return tmp;
		}
		link_type clone_node(link_type ptr) {
			link_type tmp = create_node(ptr->val);
			tmp->color = ptr->color;
			tmp->left = nullptr;
			tmp->right = nullptr;
			return tmp;
		}
		void destroy_node(link_type ptr) {
			mySimpleSTL::allocator<T>::destroy(&ptr->val);
			put_node(ptr);
		}

	private:
		link_type header;  // hearder节点     hader节点和root节点互为parent，这是一种实现手法
							// 而header的左指针指向的是最左边的节点，右指针指向的是最右边最后的节点
		size_type node_count;  // 元素个数
		Compare key_compare;  // 比较函数对象
	private:
		link_type& root() const { return header->parent; };
		link_type& mostleft()const { return header->left; };
		link_type& mostright()const { return header->right; };

		reference& front() { return mostleft()->val; };
		reference& back() { return mostright()->val; }
		
		// 辅助函数: left,right,parent,value,key,color,minimum,maximum
		link_type left(link_type ptr)const { return ptr->left; }
		link_type right(link_type ptr)const { return ptr->right; }
		link_type parent(link_type ptr)const { return ptr->parent; }
		const_reference value(link_type ptr)const { return ptr->val; }
		reference value(link_type ptr) { return ptr->val; }

		link_type minimin(link_type ptr) { return __rb_tree_node<T>::minimum(ptr);}
		link_type maximum(link_type ptr) {return __rb_tree_node<T>::maximum(ptr);}
	public:
		typedef __rb_tree_iterator<T, T&, T*> iterator;
		typedef __rb_tree_iterator<T, const T&, const T*> const_iterator;
	public:
		Compare key_comp()const { return key_compare; };
		iterator begin() { return iterator(mostleft()); };
		iterator end() { return iterator(header); };
		bool empty()const { return node_count == 0; };  // nodecoung不包括header
		size_type size()const { return node_count; };
		size_type max_size() const { return size_type(-1); };

	public:
		inline iterator find(const value_type& val);



	public:
		typedef mySimpleSTL::allocator<rb_tree_node> alloc_node;
		typedef mySimpleSTL::allocator<T> alloc_T;
	private:
		void init();   // 如果init,clear不实现的话会出现符号错误LINK2019
		void clear();  // clear()函数将树所有的节点释放，但是不释放header.那是析构函数释放header.
	public:
		rb_tree(const Compare& comp = Compare())
			:node_count(0), key_compare(comp) {
			init();
		}
		~rb_tree() { clear(); put_node(header); }

		rb_tree& operator=(const rb_tree& rv);


		// 用于操作元素的函数，以及工具函数
	private:
		iterator __insert(link_type x, link_type y, const value_type& val);		// 真正执行插入的函数
	public:
		void __erase(link_type x);
		void __erase(value_type val);
	private:
		inline void __rb_tree_rebalance(rb_tree_node* x, rb_tree_node*& root);
		inline void __rb_tree_rotate_left(rb_tree_node* x, rb_tree_node*& root);
		inline void __rb_tree_rotate_right(rb_tree_node* x, rb_tree_node*& root);
	public:
		pair<iterator, bool> insert_unique(const value_type& val);
		iterator insert_equal(const value_type& val);
	};






	// -------------------以下仅为实现部分--------------------- 
	template<typename T, typename Compare>
	inline void mySimpleSTL::rb_tree<T, Compare>::__erase(value_type val)
	{

		iterator iter = find(val);
		if (iter != end()) {
			__erase(iter.node);
		}
	}
	template<typename T, typename Compare>
	inline rb_tree<T, Compare>::iterator rb_tree<T, Compare>::find(const value_type& val)
	{
		
		for (iterator iter = begin(); iter != end(); ++iter) {
			if (*iter == val) {
				return iter;
			}
		}

		return end();
	}



	template<typename T, typename Compare>
	inline void mySimpleSTL::rb_tree<T, Compare>::__erase(link_type x)
	{

		// 如果x有两个儿子，那么找到x的后继节点y，然后交换x和y的值，然后删除y
		if (x->left != nullptr && x->right != nullptr) {
			link_type y = x->right;
			while (y->left != nullptr) {
				y = y->left;
			}
			x->val = y->val;
			x = y;
		}

		// 如果x只有一个儿子或者没有儿子，那么直接删除x
		link_type replacement = x->left != nullptr ? x->left : x->right;
		if (replacement != nullptr) {
			replacement->parent = x->parent;
			if (x->parent == nullptr) {
				root() = replacement;
			}
			else if (x == x->parent->left) {
				x->parent->left = replacement;
			}
			else {
				x->parent->right = replacement;
			}

			if (x == mostleft()) {
				if (x->right == nullptr) {
					mostleft() = x->parent;
				}
				else {
					mostleft() = minimin(replacement);
				}
			}
			else if (x == mostright()) {
				if (x->left == nullptr) {
					mostright() = x->parent;
				}
				else {
					mostright() = maximum(replacement);
				}
			}
		}
		else if (x->parent == nullptr) {
			root() = nullptr;
			mostleft() = header;
			mostright() = header;
		}
		else {
			if (x == mostleft()) {
				mostleft() = x->parent;
			}
			if (x == mostright()) {
				mostright() = x->parent;
			}
			if (x == x->parent->left) {
				x->parent->left = nullptr;
			}
			else {
				x->parent->right = nullptr;
			}
		}

		// 如果x是黑色节点，那么需要重新平衡
		if (x->color == __rb_tree_black) {
			__rb_tree_rebalance(x->parent, root());
		}

		// 释放x
		destroy_node(x);
		--node_count;


	}

	template<typename T, typename Compare>
	inline void rb_tree<T, Compare>::__rb_tree_rebalance(rb_tree_node* x, rb_tree_node*& root)
	{
		x->color = __rb_tree_red;
		while (x != root && x->parent->color == __rb_tree_red) {
			if (x->parent == x->parent->parent->left) {
				rb_tree_node* y = x->parent->parent->right;
				if (y && y->color == __rb_tree_red) {
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						__rb_tree_rotate_left(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else {
				rb_tree_node* y = x->parent->parent->left;
				if (y && y->color == __rb_tree_red) {
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black;
	}

	template<typename T, typename Compare>
	inline void rb_tree<T, Compare>::__rb_tree_rotate_left(rb_tree_node* x, rb_tree_node*& root)
	{
		rb_tree_node* y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	template<typename T, typename Compare>
	inline void rb_tree<T, Compare>::__rb_tree_rotate_right(rb_tree_node* x, rb_tree_node*& root)
	{
		rb_tree_node* y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// 插入值，允许重复
	template<typename T, typename Compare>
	inline typename mySimpleSTL::rb_tree<T, Compare>::iterator
		mySimpleSTL::rb_tree<T, Compare>::insert_equal(const value_type& val)
	{

		link_type y = header;
		link_type x = root();

		while (x != nullptr) {
			y = x;
			x = this->key_compare(val, x->val) ? left(x) : right(x);
		}

		return __insert(x, y, val);
	}


	// insert的值不允许重复，否则插入无效
	template<typename T, typename Compare>
	inline pair<typename mySimpleSTL::rb_tree<T, Compare>::iterator, bool>
		mySimpleSTL::rb_tree<T, Compare>::insert_unique(const value_type& val)
	{
		
		link_type y = header;
		link_type x = root();
		bool comp = true;
		
		//  和insert_euqal一样，向下找
		while (x != nullptr) {
			y = x; 
			comp = this->key_compare(val, x->val);
			x = comp ? left(x) : right(x);
		};

		
		iterator  j = iterator(y);
		if (comp) {
			if (j == begin())
				return mySimpleSTL::pair<iterator, bool>(__insert(x, y, val), true);
			else
				--j;
		}

		if (key_compare(*j, val)) {
			return mySimpleSTL::pair<iterator, bool>(__insert(x,y,val),true);
		}

		return mySimpleSTL::pair<iterator, bool>(j,false);
	}


	template<typename T, typename Compare>
	typename mySimpleSTL::rb_tree<T, Compare>::iterator
		mySimpleSTL::rb_tree<T, Compare>::__insert(link_type x, link_type y, const value_type& val)
	{
		link_type z = create_node(val);  // 创建新节点z

		if (y == header || x != nullptr || key_compare(val, y->val)) {
			// 插入到y的左侧
			y->left = z;
			if (y == header) {
				root() = z;
				mostright() = z;
			}
			else if (y == mostleft()) {
				mostleft() = z;  // 更新mostleft
			}
		}
		else {
			// 插入到y的右侧
			y->right = z;
			if (y == mostright()) {
				mostright() = z;  // 更新mostright
			}
		}

		z->parent = y;
		z->left = nullptr;
		z->right = nullptr;
		__rb_tree_rebalance(z, header->parent);  // 重新平衡红黑树

		++node_count;  // 节点数量增加
		return iterator(z);  // 返回迭代器指向新节点
	}





	
	template<typename T>
	inline __rb_tree_node<T>::link_type __rb_tree_node<T>::minimum(link_type ptr)
	{
		while (ptr->left != nullptr) {
			ptr = ptr->left;
		}
		return ptr;
	}

	template<typename T>
	inline __rb_tree_node<T>::link_type __rb_tree_node<T>::maximum(link_type ptr)
	{
		while (ptr->right != nullptr) {
			ptr = ptr->right;
		}
		return ptr;
	}


	template<typename T, typename Compare>
	inline void rb_tree<T, Compare>::init()
	{
		header = get_node();
		header->color = __rb_tree_red;
		alloc_T::construct(&(header->val),T());		// 添加正确,看来文档思路没错
		mostleft() = header;
		mostright() = header;
		header->parent = nullptr;
	}

	template<typename T, typename Compare>
	inline void rb_tree<T, Compare>::clear()
	{
		if (node_count == 0) {
			return;
		}

		// 使用一个栈来存储需要删除的节点
		mySimpleSTL::stack<link_type> s;
		s.push(root()); // 将根节点压入栈中
		while (!s.empty()) { // 当栈不为空时
			link_type curr = s.top(); // 取出栈顶元素
			s.pop(); // 弹出栈顶元素
			if (curr->left != nullptr) { // 如果有左子节点
				s.push(curr->left); // 将左子节点压入栈中
			}
			if (curr->right != nullptr) { // 如果有右子节点
				s.push(curr->right); // 将右子节点压入栈中
			}
			destroy_node(curr); // 删除当前节点
			--node_count; // 将node_count减一
		}

		mostleft() = header; // 将header节点的左指针指向自己
		mostright() = header; // 将header节点的右指针指向自己
	}




	// 用于实现迭代器的++操作
	template<typename T, typename Ref, typename Ptr>
	void __rb_tree_iterator<T, Ref, Ptr>::increment() {
		
		if (node->right != nullptr) {
			// 如果存在右节点，则下一个节点是右子树中的最左节点
			node = node->right;
			
			while (node->left != nullptr) {
				node = node->left;
			}
		}
		else {
			// 如果不存在右节点，则向上找到第一个比当前节点大的父节点
			link_type parent = node->parent;
			while (node == parent->right) {
				node = parent;
				parent = parent->parent;
			}
			// 如果当前节点是根节点，那么已经遍历完整个树
			// 否则，下一个节点就是父节点
			if (node->right != parent)
				node = parent;
		}
	}

	// 用于实现迭代器的--操作
	template<typename T, typename Ref, typename Ptr>
	void __rb_tree_iterator<T, Ref, Ptr>::decrement() {
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			// 如果节点是header节点，即end()迭代器
			// 则它的前一个节点是整棵树的最大节点，即右子树中的最右节点
			node = node->right;
		}
		else if (node->left != nullptr) {
			// 如果存在左子树，则前一个节点是左子树中的最右节点
			link_type prev = node->left;
			while (prev->right != nullptr) {
				prev = prev->right;
			}
			node = prev;
		}
		else {
			// 如果不存在左子树，则向上找到第一个比当前节点小的父节点
			link_type parent = node->parent;
			while (node == parent->left) {
				node = parent;
				parent = parent->parent;
			}
			node = parent;
		}
	}


};

#endif /* __SGI_STL_INTERNAL_TREE_H */
