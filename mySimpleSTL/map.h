#pragma once
#ifndef MYSIMPLESTL_MAP_H
#define MYSIMPLESTL_MAP_H

// map的元素都是pair，pair的键值是不允许更改的，但是实值允许更改；
// 更改键值会极大的影响红黑树的特性，所以不允许更改

#include"rb_tree.h"


namespace mySimpleSTL {

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    class map {
    public:
        typedef Key key_type;
        typedef Value mapped_type;
        typedef pair<const Key, Value> value_type;
        typedef Compare key_compare;

        // 定义一个内部类，用来比较pair类型的节点
        class value_compare {
            friend class map<Key, Value, Compare>;
        protected:
            Compare comp;
            value_compare(Compare c) :comp(c) {}
        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        // 使用rb_tree作为底层数据结构
        typedef rb_tree<value_type, value_compare> rep_type;
        rep_type t; // rb_tree对象

    public:
        // 定义一些类型别名，和rb_tree保持一致
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;

    public:
        // 构造函数和析构函数
        map() :t(value_compare(Compare())) {} // 默认构造函数，使用默认的比较函数对象
        explicit map(const Compare& comp) :t(value_compare(comp)) {} // 使用指定的比较函数对象
        template<typename InputIterator>
        map(InputIterator first, InputIterator last) : t(value_compare(Compare())) { // 使用迭代器范围构造
            t.insert_unique(first, last);
        }
        template<typename InputIterator>
        map(InputIterator first, InputIterator last, const Compare& comp) : t(value_compare(comp)) { // 使用迭代器范围和指定的比较函数对象构造
            t.insert_unique(first, last);
        }
        map(const map<Key, Value, Compare>& x) :t(x.t) {} // 拷贝构造函数
        ~map() {} // 析构函数，不需要做什么，因为rb_tree会自动释放内存
        key_compare key_comp() const { return t.key_comp(); } // 返回键的比较函数对象
        value_compare value_comp() const { return t.value_comp(); } // 返回值的比较函数对象
        iterator begin() { return t.begin(); } // 返回第一个节点的迭代器
        const_iterator begin() const { return t.begin(); }
        iterator end() { return t.end(); } // 返回尾后节点的迭代器
        const_iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); } // 判断是否为空
        size_type size() const { return t.size(); } // 返回节点个数
        size_type max_size() const { return t.max_size(); } // 返回最大节点个数

        // 插入操作，使用rb_tree的insert_unique函数
        pair<iterator, bool> insert(const value_type& x) {
            return t.insert_unique(x);
        }
        iterator insert(iterator position, const value_type& x) {
            return t.insert_unique(position, x);
        }
        template<typename InputIterator>
        void insert(InputIterator first, InputIterator last) {
            t.insert_unique(first, last);
        }

        // 删除操作，使用rb_tree的erase函数
        void erase(iterator position) {
            t.erase(position);
        }
        size_type erase(const key_type& x) {
            return t.erase(x);
        }
        void erase(iterator first, iterator last) {
            t.erase(first, last);
        }
        void clear() {
            t.clear();
        }

        // 查找操作，使用rb_tree的find函数
        iterator find(const key_type& x) {
            return t.find(x);
        }
        const_iterator find(const key_type& x) const {
            return t.find(x);
        }

        // 访问操作，使用rb_tree的value函数
        mapped_type& operator[](const key_type& k) {
            iterator i = lower_bound(k); // 找到第一个不小于k的位置
            if (i == end() || key_comp()(k, (*i).first)) { // 如果没有找到或者找到的键不等于k
                i = insert(i, value_type(k, mapped_type())); // 插入一个新的键值对
            }
            return (*i).second; // 返回对应的值的引用
        }

    };
	

};

#endif