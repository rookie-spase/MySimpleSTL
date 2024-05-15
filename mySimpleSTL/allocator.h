#pragma once
#ifndef MYSIMPLESTL_ALLOCATOR_H
#define MYSIMPLESTL_ALLOCATOR_H
#include "construct.h"
namespace mySimpleSTL {
    template <class T>
    class allocator {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;            // 5种基本要求

        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
    public:
                                                        // 分配，释放，构造，析构
        static T* allocate();
        static T* allocate(size_type n);

        static void deallocate(T* ptr);
        static void deallocate(T* ptr, size_type n);

        static void construct(T* ptr);
        static void construct(T* ptr, const T& value);
        static void construct(T* ptr, T&& value);

        template <class... Args>
        static void construct(T* ptr, Args&& ...args);

        static void destroy(T* ptr);
        static void destroy(T* first, T* last);
    };


    // 剩下的都是gpt写的

    template<class T>
    T* allocator<T>::allocate() {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<class T>
    T* allocator<T>::allocate(size_type n) {
        if (n == 0) return nullptr;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<class T>
    void allocator<T>::deallocate(T* ptr) {
        if (ptr == nullptr) return;
        ::operator delete(ptr);
    }

    template<class T>
    void allocator<T>::deallocate(T* ptr, size_type n) {
        if (ptr == nullptr || n == 0) return;
        ::operator delete(ptr, n * sizeof(T));
    }

    template<class T>
    void allocator<T>::construct(T* ptr) {
        mySimpleSTL::construct(ptr);
    }

    template<class T>
    void allocator<T>::construct(T* ptr, const T& value) {
        mySimpleSTL::construct(ptr, value);
    }

    template<class T>
    void allocator<T>::construct(T* ptr, T&& value) {
        mySimpleSTL::construct(ptr, mySimpleSTL::move(value));
    }

    template<class T>
    template<class... Args>
    void allocator<T>::construct(T* ptr, Args&& ...args) {
        mySimpleSTL::construct(ptr, mySimpleSTL::forward<Args>(args)...);
    }

    template<class T>
    void allocator<T>::destroy(T* ptr) {
        mySimpleSTL::destroy(ptr);
    }

    template<class T>
    void allocator<T>::destroy(T* first, T* last) {
        mySimpleSTL::destroy(first, last);
    }


    /*
    示例：
    mySimpleSTL::allocator<int> alloc;
    int* p1 = alloc.allocate();
    alloc.construct(p1, 10);
    cout << *p1 << endl;
    alloc.destroy(p1);
    alloc.deallocate(p1);

    mySimpleSTL::allocator<std::string> alloc2;
    std::string* p2 = alloc2.allocate();
    alloc2.construct(p2, "hello world");
    cout << *p2 << endl;
    alloc2.destroy(p2);
    alloc2.deallocate(p2);

    mySimpleSTL::allocator<std::vector<int>> alloc3;
    std::vector<int>* p3 = alloc3.allocate();
    alloc3.construct(p3, std::initializer_list<int>{1, 2, 3, 4, 5});
    cout << (*p3).front() << endl;
    alloc3.destroy(p3);
    alloc3.deallocate(p3);

    */
};

#endif