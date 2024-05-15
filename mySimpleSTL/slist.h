#pragma once
#ifndef MYSIMPLESTL_SLIST_H
#define MYSIMPLESTL_SLIST_H

#include "iterator.h"
#include "allocator.h"

namespace mySimpleSTL {

    // 节点设计
    template <typename T>
    struct slist_node {
        T data;
        slist_node* next;
    };

    // 迭代器设计
    template <typename T>
    struct slist_iterator_base {
        using iterator_category = mySimpleSTL::forward_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        slist_node<T>* node;

        void incr() {
            node = node->next;
        }

        bool operator==(const slist_iterator_base& other) const {
            return this->node == other.node;
        }

        bool operator!=(const slist_iterator_base& other) const {
            return !(*this == other);
        }
    };

    template <typename T, typename Ref, typename Ptr>
    struct slist_iterator : public slist_iterator_base<T> {
        using self = slist_iterator;
        using iterator = slist_iterator<T, T&, T*>;
        using const_iterator = slist_iterator<T, const T&, const T*>;
        using value_type = T;
        using reference = Ref;
        using pointer = Ptr;
        using link_type = slist_node<T>*;

        slist_iterator(link_type node) {
            this->node = node;
        }

        slist_iterator() {
            this->node = nullptr;
        }

        slist_iterator(const iterator& other) {
            this->node = other.node;
        }

        reference operator*() const {
            return this->node->data;
        }

        pointer operator->() const {
            return &(operator*());
        }

        self& operator++() {
            this->incr();
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            this->incr();
            return tmp;
        }
    };

    // 使用 mySimpleSTL::allocator
    template <typename T>
    class slist {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using iterator = slist_iterator<T, T&, T*>;
        using const_iterator = slist_iterator<T, const T&, const T*>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    private:
        using node = slist_node<T>;
        using node_allocator = mySimpleSTL::allocator<node>;

        node* head;

    private:
        static node* create_node(const T& val) {
            node* n = node_allocator::allocate();
            try {
                mySimpleSTL::allocator<T>::construct(&n->data, val);
                n->next = nullptr;
            }
            catch (...) {
                node_allocator::deallocate(n);
            }

            return n;
        }

        static void destory_node(node* n) {
            mySimpleSTL::allocator<T>::destroy(&n->data);
            node_allocator::deallocate(n);
        }

        void clear() {
            while (head->next != nullptr) {
                node* tmp = head->next;
                head->next = tmp->next;
                destory_node(tmp);
            }
        }

    public:
        slist() {
            head = create_node(T());
        }

        ~slist() {
            clear();
            destory_node(head);
        }

        iterator begin() {
            return iterator(head->next);
        }

        const_iterator begin() const {
            return const_iterator(head->next);
        }

        iterator end() {
            return iterator(nullptr);
        }

        const_iterator end() const {
            return const_iterator(nullptr);
        }

        bool empty() const {
            return head->next == nullptr;
        }

        size_type size() const {
            size_type count = 0;
            for (node* n = head->next; n != nullptr; n = n->next) {
                ++count;
            }
            return count;
        }

        void swap(slist& other) {
            mySimpleSTL::swap(head, other.head);
        }

        reference front() {
            return head->next->data;
        }

        void push_front(const T& value) {
            slist_node<T>* n = create_node(value);
            n->next = head->next;
            head->next = n;
        }

        void pop_front() {
            node* first = head->next;
            head->next = first->next;
            destory_node(first);
        }


    };

};

#endif


/*
* 实例代码
*     mySimpleSTL::slist<int>::iterator it;
    {
        mySimpleSTL::slist<int> e;
        e.push_front(111);


        it = e.begin();

        cout << *it;
    }
   

    cout << *it;        // 如果输出的不是111而是类似于-441231231的数字，表示释放成功
                        // 测试成功
    

*/