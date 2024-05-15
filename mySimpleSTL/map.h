#pragma once
#ifndef MYSIMPLESTL_MAP_H
#define MYSIMPLESTL_MAP_H

// map��Ԫ�ض���pair��pair�ļ�ֵ�ǲ�������ĵģ�����ʵֵ������ģ�
// ���ļ�ֵ�Ἣ���Ӱ�����������ԣ����Բ��������

#include"rb_tree.h"


namespace mySimpleSTL {

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    class map {
    public:
        typedef Key key_type;
        typedef Value mapped_type;
        typedef pair<const Key, Value> value_type;
        typedef Compare key_compare;

        // ����һ���ڲ��࣬�����Ƚ�pair���͵Ľڵ�
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
        // ʹ��rb_tree��Ϊ�ײ����ݽṹ
        typedef rb_tree<value_type, value_compare> rep_type;
        rep_type t; // rb_tree����

    public:
        // ����һЩ���ͱ�������rb_tree����һ��
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;
        typedef typename rep_type::difference_type difference_type;

    public:
        // ���캯������������
        map() :t(value_compare(Compare())) {} // Ĭ�Ϲ��캯����ʹ��Ĭ�ϵıȽϺ�������
        explicit map(const Compare& comp) :t(value_compare(comp)) {} // ʹ��ָ���ıȽϺ�������
        template<typename InputIterator>
        map(InputIterator first, InputIterator last) : t(value_compare(Compare())) { // ʹ�õ�������Χ����
            t.insert_unique(first, last);
        }
        template<typename InputIterator>
        map(InputIterator first, InputIterator last, const Compare& comp) : t(value_compare(comp)) { // ʹ�õ�������Χ��ָ���ıȽϺ���������
            t.insert_unique(first, last);
        }
        map(const map<Key, Value, Compare>& x) :t(x.t) {} // �������캯��
        ~map() {} // ��������������Ҫ��ʲô����Ϊrb_tree���Զ��ͷ��ڴ�
        key_compare key_comp() const { return t.key_comp(); } // ���ؼ��ıȽϺ�������
        value_compare value_comp() const { return t.value_comp(); } // ����ֵ�ıȽϺ�������
        iterator begin() { return t.begin(); } // ���ص�һ���ڵ�ĵ�����
        const_iterator begin() const { return t.begin(); }
        iterator end() { return t.end(); } // ����β��ڵ�ĵ�����
        const_iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); } // �ж��Ƿ�Ϊ��
        size_type size() const { return t.size(); } // ���ؽڵ����
        size_type max_size() const { return t.max_size(); } // �������ڵ����

        // ���������ʹ��rb_tree��insert_unique����
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

        // ɾ��������ʹ��rb_tree��erase����
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

        // ���Ҳ�����ʹ��rb_tree��find����
        iterator find(const key_type& x) {
            return t.find(x);
        }
        const_iterator find(const key_type& x) const {
            return t.find(x);
        }

        // ���ʲ�����ʹ��rb_tree��value����
        mapped_type& operator[](const key_type& k) {
            iterator i = lower_bound(k); // �ҵ���һ����С��k��λ��
            if (i == end() || key_comp()(k, (*i).first)) { // ���û���ҵ������ҵ��ļ�������k
                i = insert(i, value_type(k, mapped_type())); // ����һ���µļ�ֵ��
            }
            return (*i).second; // ���ض�Ӧ��ֵ������
        }

    };
	

};

#endif