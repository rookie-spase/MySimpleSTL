#pragma once
#ifndef MYSIMPLESTL_VECTOR_H
#define MYSIMPLESTL_VECTOR_H

#include <initializer_list>

#include "iterator.h"
#include "util.h"
#include "exceptdef.h"
#include "allocator.h"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
* 不同点：
*   vector的第二参数，原版有,他没有第二参数
* 机制：
	-  成员有三个start,finish,end_of_storage
	- construct()->allocate_and_fill()->allocated and then uninitialized_fill();
	- 两倍等比增长机制,insert_aux实现
	- insert,insert_aux,
	- 迭代器就是指针
	- 针对vector<bool>具有特化,但可以直接用bitset，不实现了
* 危险：
	- 重新分配后，所有指向原来的地方都会失效
	- 缩减方法：reserve(),shrink_to_fit(),
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace mySimpleSTL {


    template<typename T>
    class vector {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T* iterator;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef mySimpleSTL::allocator<T>                      allocator_type;
        typedef mySimpleSTL::allocator<T>                      data_allocator;

    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;
    private:
        void insert_aux(iterator position, const T& x);
        void deallocate();
        void fill_initialize(size_type n, const T& value);
        iterator allocate_and_fill(size_type n, const T& value);
    public:
        iterator begin();
        iterator end();
        size_type size() const;
        size_type capacity() const;
        bool empty();
        reference operator[](size_type n);
        reference front();
        reference back();
    public:
        void push_back(const T& x);
        void pop_back();
        iterator erase(iterator position);
        void resize(size_type new_size, const T& x);
        void resize(size_type new_size);
        void clear();
        iterator insert(iterator position, const T& x);
        void insert(iterator position, size_type n, const T& x);
    public:
        vector() {
            start = nullptr;
            finish = nullptr;
            end_of_storage = nullptr;
        }

        vector(size_type n, const T& value) {
            fill_init(n, value);
        }

        vector(int n, const T& value) {
           fill_initialize(n, value);
        }

        vector(long n, const T& value) {
            fill_initialize(n, value);
        }

        explicit vector(size_type n) {
            fill_initialize(n, T());
        }

        ~vector() {
            destroy(start, finish);
            deallocate();
        }
        vector(std::initializer_list<value_type> ilist)
        {
            range_init(ilist.begin(), ilist.end());
        }
    private:
        void      fill_init(size_type n, const value_type& value);
        void      init_space(size_type size, size_type cap);
        template <class Iter>
        void      range_init(Iter first, Iter last);
    public:
        typename vector::iterator erase(iterator position, iterator last);
        void swap(vector& x);
    };


// ---------------------------- 以下为实现的部分 --------------------------------------
    template<typename T>
    void vector<T>::swap(vector<T>& x) {
        mySimpleSTL::swap(start, x.start);
        mySimpleSTL::swap(finish, x.finish);
        mySimpleSTL::swap(end_of_storage, x.end_of_storage);
    };

    // range_init 函数
    template <class T>
    template <class Iter>
    void vector<T>::
        range_init(Iter first, Iter last)
    {
        const size_type len = mySimpleSTL::distance(first, last);
        const size_type init_size = mySimpleSTL::max(len, static_cast<size_type>(16));
        init_space(len, init_size);
        mySimpleSTL::uninitialized_copy(first, last, start);
    }
    // fill_init 函数
    template <class T>
    void vector<T>::
        fill_init(size_type n, const value_type& value)
    {
        const size_type init_size = mySimpleSTL::max(static_cast<size_type>(16), n);
        init_space(n, init_size);
        mySimpleSTL::uninitialized_fill_n(start, n, value);
    }
    // init_space 函数
    template <class T>
    void vector<T>::init_space(size_type size, size_type cap)
    {
        try
        {
            start = data_allocator::allocate(cap);
            finish = start + size;
            end_of_storage = start + cap;
        }
        catch (...)
        {
            start = nullptr;
            finish = nullptr;
            end_of_storage = nullptr;
            throw;
        }
    }


    template<typename T>
    typename vector<T>::iterator vector<T>::erase(iterator position, iterator last)
    {
        if (last != end()) 
            copy(last, finish, position);
        
        finish = finish - (last - position);
        destroy(finish, last);
        return position;
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::begin() {
        return start;
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::end() {
        return finish;
    }

    template<typename T>
    typename vector<T>::size_type vector<T>::size() const {
        return size_type(const_cast<vector<T>*>(this)->end() - const_cast<vector<T>*>(this)->begin());
    }

    template<typename T>
    typename vector<T>::size_type vector<T>::capacity() const {
        return size_type(const_cast<vector<T>*>(this)->end_of_storage - const_cast<vector<T>*>(this)->begin());

    }

    template<typename T>
    bool vector<T>::empty()  {
        return (begin() == end());
    }

    template<typename T>
    typename vector<T>::reference vector<T>::operator[](size_type n) {
        return *(begin() + n);
    }

    template<typename T>
    typename vector<T>::reference vector<T>::front() {
        return *begin();
    }

    template<typename T>
    typename vector<T>::reference vector<T>::back() {
        return *(end() - 1);
    }

    template<typename T>
    void vector<T>::push_back(const T& x) {
        if (finish != end_of_storage) {
            construct(finish, x);
            ++finish;
        }
        else {
            insert_aux(end(), x);
        }
    }

    template<typename T>
    void vector<T>::pop_back() {
        --finish;
        destroy(finish);
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::erase(iterator position) {
        if (position + 1 != end()) {
            copy(position + 1, finish, position);
        }
        --finish;
        destroy(finish);
        return position;
    }

    template<typename T>
    void vector<T>::resize(size_type new_size, const T& x) {            // resize只是将指针转变了而已
        if (new_size < size()) {
            erase(begin() + new_size, end());
        }
        else {
            insert(end(), new_size - size(), x);
        }
    }

    template<typename T>
    void vector<T>::resize(size_type new_size) {
        resize(new_size, T());
    }

    template<typename T>
    void vector<T>::clear() {
        erase(begin(), end());
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::insert(iterator position, const T& x) {
        size_type n = position - begin();
        if (finish != end_of_storage && position == end()) {
            construct(finish, x);
            ++finish;
        }
        else {
            insert_aux(position, x);
        }
        return begin() + n;
    }

    template<typename T>
    void vector<T>::insert(iterator position, size_type n, const T& x) {
        if (n != 0) {
            if (size_type(end_of_storage - finish) >= n) {
                T x_copy = x;
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if (elems_after > n) {
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                }
                else {
                    uninitialized_fill_n(finish, n - elems_after, x_copy);
                    finish += n - elems_after;
                    uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            }
            else {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(start, position, new_start);
                    new_finish = uninitialized_fill_n(new_finish, n, x);
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch (...) {
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }
                destroy(begin(), end());
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

    template<typename T>
    void vector<T>::deallocate() {
        if (start) {
            data_allocator::deallocate(start, end_of_storage - start);
        }
    }

    template<typename T>
    void vector<T>::fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }


    template<typename T>
    typename vector<T>::iterator vector<T>::allocate_and_fill(size_type n, const T& value) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, value);
        return result;
    }

    template<typename T>
    void vector<T>::insert_aux(iterator position, const T& x) {
        if (finish != end_of_storage) {
            construct(finish, *(finish - 1));
            ++finish;
            T x_copy = x;
            copy_backward(position, finish - 2, finish - 1);
            *position = x_copy;
        }
        else {
            const size_type old_size = size();
            const size_type new_size = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch (...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }
};
#endif