#include"BASE_INCLUDE.H"
#include<iostream>
#include<chrono>

using namespace std;
using namespace mySimpleSTL;






// vector 完好
#include"vector.h"
#include<vector>
void test_vector() {

    std::cout << "-------------vector功能测试-------------" << std::endl;

    {
        mySimpleSTL::vector<int> v1;
        cout << "v1.size(): " << v1.size() << endl;  // expected output: v1.size(): 0
        cout << "v1.capacity(): " << v1.capacity() << endl;  // expected output: v1.capacity(): 0

        v1.push_back(1);
        v1.push_back(2);
        v1.push_back(3);
        


        cout << "v1.size(): " << v1.size() << endl;  // expected output: v1.size(): 3
        cout << "v1.capacity(): " << v1.capacity() << endl;  // expected output: v1.capacity(): 4

        mySimpleSTL::vector<int> v2 = { 4, 5, 6 };
        cout << "v2.size(): " << v2.size() << endl;  // expected output: v2.size(): 3
        cout << "v2.capacity(): " << v2.capacity() << endl;  // expected output: v2.capacity(): 3

        v2.pop_back();
        cout << "v2.size(): " << v2.size() << endl;  // expected output: v2.size(): 2
        cout << "v2.capacity(): " << v2.capacity() << endl;  // expected output: v2.capacity(): 3

        mySimpleSTL::vector<int>::iterator it = v1.insert(v1.begin() + 1, 10);
        cout << "v1[0]: " << v1[0] << ", *it: " << *it << ", v1[2]: " << v1[2] << endl;  // expected output: v1[0]: 1, *it: 10, v1[2]: 3

        v1.resize(5, 20);
        cout << "v1.size(): " << v1.size() << ", v1[3]: " << v1[3] << ", v1[4]: " << v1[4] << endl;  // expected output: v1.size(): 5, v1[3]: 20, v1[4]: 20

        v1.resize(2);
        cout << "v1.size(): " << v1.size() << ", v1[0]: " << v1[0] << ", v1[1]: " << v1[1] << endl;  // expected output: v1.size(): 2, v1[0]: 1, v1[1]: 10

        v2.clear();
        cout << "v2.size(): " << v2.size() << endl;  // expected output: v2.size(): 0
    }
    std::cout << "-------------vector功能测试-------------" << std::endl;

    std::cout << "-------------vector分配析构测试-------------" << std::endl;

    {
        int* p;

        {
            mySimpleSTL::vector<int> e;

            e.push_back(1);

            p = e.begin();

            cout << *p << endl;
        }

        if (p != nullptr)
            cout << *p;
      

        //结果：
//    1
//    - 572662307
//可见，在区域结束之后，我的vector确实将空间析构并且释放了        * 

    }

    std::cout << std::endl << "-------------vector分配析构测试-------------" << std::endl;
    std::cout << "-------------vector性能测试-------------" << std::endl;
    {
        mySimpleSTL::vector<int> v;
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v.push_back(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "mySimpleSTL::vector push_back所需时间: " << elapsed.count() << " 毫秒" << endl;

        std::vector<int> v1;
        auto start1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v1.push_back(i);
        }
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed1 = end1 - start1;
        cout << "std::vector push_back所需时间: " << elapsed1.count() << " 毫秒" << endl;
        //输出结果：
        //    mySimpleSTL::vector push_back所需时间 : 8.5542 毫秒
        //    std::vector push_back所需时间 : 62.4015 毫秒
        //    分析：
        //    我的vector速度比标准库的快，最简单的原因是我的vector很轻量，而标准库的vector可能是因为东西太多的原因
        std::cout << std::endl << "-------------vector性能测试-------------" << std::endl;



    }


}

// deque完好
#include<deque>
#include"deque.h"
void test_deque() {
    {

    std::cout << std::endl << "-------------deque功能测试-------------" << std::endl;

    mySimpleSTL::deque<int> val;
    
    for (int i = 0; i < 20; ++i) {
        val.push_back(i);
    }


    val.erase(val.begin()+11);
    val.erase(val.begin()+11);
    val.erase(val.begin()+11);

    for (auto e : val)
        cout << e << " ";
    std::cout << std::endl << "-------------deque功能测试-------------" << std::endl;

    }

    {
        std::cout << std::endl << "-------------deque性能测试-------------" << std::endl;
        mySimpleSTL::deque<int> v;
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v.push_back(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "mySimpleSTL::deque push_back所需时间: " << elapsed.count() << " 毫秒" << endl;

        std::deque<int> v1;
        auto start1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v1.push_back(i);
        }
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed1 = end1 - start1;
        cout << "std::deque push_back所需时间: " << elapsed1.count() << " 毫秒" << endl;
        std::cout << std::endl << "-------------deque性能测试-------------" << std::endl;

    }

    
}

// stack完好
#include"stack.h"
#include<stack>
void test_stack() {

    {
        std::cout << std::endl << "-------------stack功能测试-------------" << std::endl;

        mySimpleSTL::stack<int> eee;

        for (int i = 0; i < 100; ++i)
            eee.push(i);

        cout << "Stack's size == " << eee.size() << " " << endl;


        while (!eee.empty()) {
            cout << eee.top() << " ";
            eee.pop();
        }
        std::cout << std::endl << "-------------stack功能测试-------------" << std::endl;

    }

    {
        {
            std::cout << std::endl << "-------------stack性能测试-------------" << std::endl;
            mySimpleSTL::stack<int> v;
            auto start = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000000; ++i) {
                v.push(i);
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;
            cout << "mySimpleSTL::stack push_back所需时间: " << elapsed.count() << " 毫秒" << endl;

            std::stack<int> v1;
            auto start1 = chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000000; ++i) {
                v1.push(i);
            }
            auto end1 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed1 = end1 - start1;
            cout << "std::stack push_back所需时间: " << elapsed1.count() << " 毫秒" << endl;
            std::cout << std::endl << "-------------stack性能测试-------------" << std::endl;

        }
    }
}

// queue完好
#include"queue.h"
#include<queue>
void test_queue() {
    {
        std::cout << std::endl << "-------------queue功能测试-------------" << std::endl;

        mySimpleSTL::queue<int> que;

        for (int i = 0; i < 10; ++i)
            que.push(i);

        cout << "queue's size is " << que.size() << endl;
        while (!que.empty()) {
            cout << que.front() << " ";
            que.pop();
        }
        std::cout << std::endl << "-------------queue功能测试-------------" << std::endl;

    }

    {
        std::cout << std::endl << "-------------queue性能测试-------------" << std::endl;
        mySimpleSTL::queue<int> v;
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v.push(i);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "mySimpleSTL::queue push_back所需时间: " << elapsed.count() << " 毫秒" << endl;

        std::queue<int> v1;
        auto start1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            v1.push(i);
        }
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed1 = end1 - start1;
        cout << "std::queue push_back所需时间: " << elapsed1.count() << " 毫秒" << endl;
        std::cout << std::endl << "-------------queue性能测试-------------" << std::endl;

    }
}

// list完好
#include"list.h"
#include<list>
void test_list() {
    {



    std::cout << "-------------List Functionality Test-------------" << std::endl;
    // 创建 mySimpleSTL::list
    mySimpleSTL::list<int> myList;

    // Push 元素
    myList.push_front(1);
    myList.push_front(2);
    myList.push_front(3);
    myList.push_front(0);

    // 访问元素
    std::cout << "Front element: " << myList.front() << std::endl;
    std::cout << "Back element: " << myList.back() << std::endl;

    // 迭代器遍历
    std::cout << "Elements in the list: ";
    for (auto it = myList.begin(); it != myList.end(); ++it) {
        std::cout << *it << " ";
    }  
    std::cout << std::endl;

    // 删除元素
    myList.pop_front();

    // 判断列表是否为空
    std::cout << "Is list empty? " << (myList.empty() ? "Yes" : "No") << std::endl;

    std::cout << "-------------List Functionality Test-------------" << std::endl;
  }




    {
        // 创建 mySimpleSTL::list 和 std::list
        mySimpleSTL::list<int> mylist;
        std::list<int> stdlist;

        // 计时开始
        auto start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            mylist.push_back(i);
        }

        // 计时结束
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "mySimpleSTL::list push_back time: " << elapsed.count() << " milliseconds" << std::endl;

        // 计时开始
        start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            stdlist.push_back(i);
        }

        // 计时结束
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;

        std::cout << "std::list push_back time: " << elapsed.count() << " milliseconds" << std::endl;

        std::cout << "-------------List Performance Test-------------" << std::endl;

    }
}

// priority_queue完好
#include"priority_queue.h"
void test_priority() {
    {
        std::cout << "-------------Priority Queue Functionality Test-------------" << std::endl;

        // 创建 mySimpleSTL::priority_queue
        mySimpleSTL::priority_queue<int> myPriorityQueue;

        // Push 元素
        myPriorityQueue.push(3);
        myPriorityQueue.push(1);
        myPriorityQueue.push(4);
        myPriorityQueue.push(2);

        // 访问元素
        std::cout << "Top element: " << myPriorityQueue.top() << std::endl;

        // Pop 元素
        myPriorityQueue.pop();

        // 判断优先队列是否为空
        std::cout << "Is priority queue empty? " << (myPriorityQueue.empty() ? "Yes" : "No") << std::endl;

        std::cout << "-------------Priority Queue Functionality Test-------------" << std::endl;

    }

    {
        std::cout << "-------------Priority Queue Performance Test-------------" << std::endl;

        // 创建 mySimpleSTL::priority_queue 和 std::priority_queue
        mySimpleSTL::priority_queue<int> myPriorityQueue;
       
        // 计时开始
        auto start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            myPriorityQueue.push(i);
        }

        // 计时结束
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "mySimpleSTL::priority_queue push time: " << elapsed.count() << " milliseconds" << std::endl;
        std::cout << "-------------Priority Queue Performance Test-------------" << std::endl;

    }

}

// map完好
#include"map.h"
#include<map>
void test_map() {
    {
        std::cout << "-------------map Functionality Test-------------" << std::endl;

        mySimpleSTL::map<int, int> abc;


        std::cout << "map'size is:" << abc.size() << std::endl;
        std::cout << "map is empty:" << boolalpha << abc.empty() << std::endl;

        for (int i = 0; i < 100; ++i)
            abc.insert({ i,i });

        std::cout << "size of this map :" << abc.size() << std::endl;
        std::cout << "map is empty:" << boolalpha << abc.empty() << std::endl;
        std::cout << "second value of element : " << std::endl;
        for (auto e : abc)
            cout << e.second << " ";
        std::cout << "-------------map Functionality Test-------------" << std::endl;
    }

    {
        {
            std::cout << "-------------map Performance Test-------------" << std::endl;

            // 创建 mySimpleSTL::list 和 std::list
            mySimpleSTL::map<int,int> mylist;
            std::map<int,int> stdlist;

            // 计时开始
            auto start = std::chrono::high_resolution_clock::now();

            // 插入十万个元素
            for (int i = 0; i < 100000; ++i) {
                mylist.insert({i,i});
            }

            // 计时结束
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;

            std::cout << "mySimpleSTL::map push_back time: " << elapsed.count() << " milliseconds" << std::endl;

            // 计时开始
            start = std::chrono::high_resolution_clock::now();

            // 插入十万个元素
            for (int i = 0; i < 100000; ++i) {
                stdlist.insert({i,i});
            }

            // 计时结束
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;

            std::cout << "std::map push_back time: " << elapsed.count() << " milliseconds" << std::endl;

            std::cout << "-------------map Performance Test-------------" << std::endl;

        }
    }

};

// slist完好
#include"slist.h"
void test_slist() {

    {
        std::cout << "------------------------------------------slist--------------------------------------------" << std::endl;
        mySimpleSTL::slist<int> e;

        for (int i = 0; i < 100; ++i)
            e.push_front(i);


        std::cout << "slist's size is :" << e.size() << std::endl;
        std::cout << "The front element of this list is " << e.front() << std::endl;

        std::cout << "All elements:" << std::endl;
        for (auto w : e) {
            cout << w << " ";
        }
        std::cout << std::endl << "------------------------------------------slist--------------------------------------------" << std::endl;
    }
    {
        std::cout << "-------------map Performance Test-------------" << std::endl;

        // 创建 mySimpleSTL::list 和 std::list
        mySimpleSTL::slist<int> mylist;

        // 计时开始
        auto start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            mylist.push_front(i);
        }

        // 计时结束
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "mySimpleSTL::map push_back time: " << elapsed.count() << " milliseconds" << std::endl;
        std::cout << "-------------slist Performance Test-------------" << std::endl;


    }
}

// set完好
#include"set.h"
#include<set>
void test_set(){

    {
        std::cout << "----------------------------------------set-------------------------------------------" << std::endl;
        mySimpleSTL::set<int> st;



        for (int i = 0; i < 100; ++i) {
            st.insert(rand() % 100);
        }
        std::cout << "set's size is " << st.size() << " Right Now" << std::endl;


        mySimpleSTL::set<int>::iterator it = st.find(99);
        if (it != st.end()) {   // find我本实现的是返回Nullptr，但是为了方便辨认出这是迭代器，还是返回了end
            int tmp = *it;
            st.erase(tmp);
            std::cout << "deleted A element named 100" << std::endl;
            std::cout << "set's size is " << st.size() << " Right Now" << std::endl;
        }

        for (auto e : st)
            cout << e << " ";
        std::cout << std::endl << "----------------------------------------set-------------------------------------------" << std::endl;
    }

    
    {
        // 创建 mySimpleSTL::list 和 std::list
        mySimpleSTL::set<int> mylist;
        std::set<int> stdlist;

        // 计时开始
        auto start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            mylist.insert(i);
        }

        // 计时结束
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "mySimpleSTL::set push_back time: " << elapsed.count() << " milliseconds" << std::endl;

        // 计时开始
        start = std::chrono::high_resolution_clock::now();

        // 插入十万个元素
        for (int i = 0; i < 100000; ++i) {
            stdlist.insert(i);
        }

        // 计时结束
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;

        std::cout << "std::set push_back time: " << elapsed.count() << " milliseconds" << std::endl;

        std::cout << "-------------set Performance Test-------------" << std::endl;

    }
    
    
}

void test_all() {
    test_vector();
    test_deque();
    test_stack();
    test_queue();
    test_list();
    test_priority();
    test_map();
    test_slist();
    test_set();
}




int main() {
    test_all();

    return 0;
}
