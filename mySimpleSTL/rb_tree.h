#pragma once
#ifndef MYSIMPLESTL_RB_TREE_H
#define MYSIMPLESTL_RB_TREE_H

#include "iterator.h"
#include "allocator.h"


// ���������ת�͵�ʱ����鷳����ֱ������һ�����ˡ�

namespace mySimpleSTL {

	// �ڵ����
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

	
	// ���������,ͬ��Ҳ�ǽ�2��������һ��
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


	// ������������
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




		// �ͽڵ㴴��������صĺ���
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
		link_type header;  // hearder�ڵ�     hader�ڵ��root�ڵ㻥Ϊparent������һ��ʵ���ַ�
							// ��header����ָ��ָ���������ߵĽڵ㣬��ָ��ָ��������ұ����Ľڵ�
		size_type node_count;  // Ԫ�ظ���
		Compare key_compare;  // �ȽϺ�������
	private:
		link_type& root() const { return header->parent; };
		link_type& mostleft()const { return header->left; };
		link_type& mostright()const { return header->right; };

		reference& front() { return mostleft()->val; };
		reference& back() { return mostright()->val; }
		
		// ��������: left,right,parent,value,key,color,minimum,maximum
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
		bool empty()const { return node_count == 0; };  // nodecoung������header
		size_type size()const { return node_count; };
		size_type max_size() const { return size_type(-1); };

	public:
		inline iterator find(const value_type& val);



	public:
		typedef mySimpleSTL::allocator<rb_tree_node> alloc_node;
		typedef mySimpleSTL::allocator<T> alloc_T;
	private:
		void init();   // ���init,clear��ʵ�ֵĻ�����ַ��Ŵ���LINK2019
		void clear();  // clear()�����������еĽڵ��ͷţ����ǲ��ͷ�header.�������������ͷ�header.
	public:
		rb_tree(const Compare& comp = Compare())
			:node_count(0), key_compare(comp) {
			init();
		}
		~rb_tree() { clear(); put_node(header); }

		rb_tree& operator=(const rb_tree& rv);


		// ���ڲ���Ԫ�صĺ������Լ����ߺ���
	private:
		iterator __insert(link_type x, link_type y, const value_type& val);		// ����ִ�в���ĺ���
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






	// -------------------���½�Ϊʵ�ֲ���--------------------- 
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

		// ���x���������ӣ���ô�ҵ�x�ĺ�̽ڵ�y��Ȼ�󽻻�x��y��ֵ��Ȼ��ɾ��y
		if (x->left != nullptr && x->right != nullptr) {
			link_type y = x->right;
			while (y->left != nullptr) {
				y = y->left;
			}
			x->val = y->val;
			x = y;
		}

		// ���xֻ��һ�����ӻ���û�ж��ӣ���ôֱ��ɾ��x
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

		// ���x�Ǻ�ɫ�ڵ㣬��ô��Ҫ����ƽ��
		if (x->color == __rb_tree_black) {
			__rb_tree_rebalance(x->parent, root());
		}

		// �ͷ�x
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

	// ����ֵ�������ظ�
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


	// insert��ֵ�������ظ������������Ч
	template<typename T, typename Compare>
	inline pair<typename mySimpleSTL::rb_tree<T, Compare>::iterator, bool>
		mySimpleSTL::rb_tree<T, Compare>::insert_unique(const value_type& val)
	{
		
		link_type y = header;
		link_type x = root();
		bool comp = true;
		
		//  ��insert_euqalһ����������
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
		link_type z = create_node(val);  // �����½ڵ�z

		if (y == header || x != nullptr || key_compare(val, y->val)) {
			// ���뵽y�����
			y->left = z;
			if (y == header) {
				root() = z;
				mostright() = z;
			}
			else if (y == mostleft()) {
				mostleft() = z;  // ����mostleft
			}
		}
		else {
			// ���뵽y���Ҳ�
			y->right = z;
			if (y == mostright()) {
				mostright() = z;  // ����mostright
			}
		}

		z->parent = y;
		z->left = nullptr;
		z->right = nullptr;
		__rb_tree_rebalance(z, header->parent);  // ����ƽ������

		++node_count;  // �ڵ���������
		return iterator(z);  // ���ص�����ָ���½ڵ�
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
		alloc_T::construct(&(header->val),T());		// �����ȷ,�����ĵ�˼·û��
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

		// ʹ��һ��ջ���洢��Ҫɾ���Ľڵ�
		mySimpleSTL::stack<link_type> s;
		s.push(root()); // �����ڵ�ѹ��ջ��
		while (!s.empty()) { // ��ջ��Ϊ��ʱ
			link_type curr = s.top(); // ȡ��ջ��Ԫ��
			s.pop(); // ����ջ��Ԫ��
			if (curr->left != nullptr) { // ��������ӽڵ�
				s.push(curr->left); // �����ӽڵ�ѹ��ջ��
			}
			if (curr->right != nullptr) { // ��������ӽڵ�
				s.push(curr->right); // �����ӽڵ�ѹ��ջ��
			}
			destroy_node(curr); // ɾ����ǰ�ڵ�
			--node_count; // ��node_count��һ
		}

		mostleft() = header; // ��header�ڵ����ָ��ָ���Լ�
		mostright() = header; // ��header�ڵ����ָ��ָ���Լ�
	}




	// ����ʵ�ֵ�������++����
	template<typename T, typename Ref, typename Ptr>
	void __rb_tree_iterator<T, Ref, Ptr>::increment() {
		
		if (node->right != nullptr) {
			// ��������ҽڵ㣬����һ���ڵ����������е�����ڵ�
			node = node->right;
			
			while (node->left != nullptr) {
				node = node->left;
			}
		}
		else {
			// ����������ҽڵ㣬�������ҵ���һ���ȵ�ǰ�ڵ��ĸ��ڵ�
			link_type parent = node->parent;
			while (node == parent->right) {
				node = parent;
				parent = parent->parent;
			}
			// �����ǰ�ڵ��Ǹ��ڵ㣬��ô�Ѿ�������������
			// ������һ���ڵ���Ǹ��ڵ�
			if (node->right != parent)
				node = parent;
		}
	}

	// ����ʵ�ֵ�������--����
	template<typename T, typename Ref, typename Ptr>
	void __rb_tree_iterator<T, Ref, Ptr>::decrement() {
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			// ����ڵ���header�ڵ㣬��end()������
			// ������ǰһ���ڵ��������������ڵ㣬���������е����ҽڵ�
			node = node->right;
		}
		else if (node->left != nullptr) {
			// �����������������ǰһ���ڵ����������е����ҽڵ�
			link_type prev = node->left;
			while (prev->right != nullptr) {
				prev = prev->right;
			}
			node = prev;
		}
		else {
			// ������������������������ҵ���һ���ȵ�ǰ�ڵ�С�ĸ��ڵ�
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
