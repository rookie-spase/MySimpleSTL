#pragma once
#ifndef MYSIMPLESTL_SET_H
#define MYSIMPLESTL_SET_H
#include"rb_tree.h"

namespace mySimpleSTL {

	template<typename T, typename Compare = std::less<T>>
	class set {
	public:
		typedef T key_type;
		typedef Compare key_compare;

	private:
		typedef rb_tree<key_type,key_compare> rep_type;
		rep_type t;

	public:
		typedef typename rep_type::const_pointer pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::const_reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::iterator const_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename T value_type;

		set() : t() {}

		key_compare key_comp() const { return t.key_comp(); }
		iterator begin()  { return t.begin(); }
		iterator end()  { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }

		pair<iterator, bool> insert(const value_type& x) {
			pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
			return pair<iterator, bool>(p.first, p.second);
		}


		void erase(const key_type& x) {
			return t.__erase(x);
		}

		iterator find(const key_type& x)  { return t.find(x); }
	};

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator==(const set<Key, Compare>& x, const set<Key, Compare>& y) {
		return x.t == y.t;
	}

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator<(const set<Key, Compare>& x, const set<Key, Compare>& y) {
		return x.t < y.t;
	}


}

#endif