#pragma once
#ifndef MYSIMPLESTL_ITERATOR_H
#define MYSIMPLESTL_ITERATOR_H

#include<cstddef>
 // 这个文件实现iterator

namespace mySimpleSTL {

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};     // 可以向前向后移动
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};// 可以指针加减移动


	template <class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category                             iterator_category;
		typedef T                                    value_type;
		typedef Pointer                              pointer;
		typedef Reference                            reference;
		typedef Distance                             difference_type;
	};


	// 用于检查某一个类型是否具有对iterator_category的定义
	template<typename T>
	struct has_iterator {
	private:
		struct two { char a; char b; };
		template<typename U> static constexpr two test(...);
		template<typename U> static constexpr char test(typename U::iterator_category* = 0);
	public:
		constexpr const static bool value = (sizeof(test<T>(0)) == sizeof(char));
	/*
	* 如果类型中具有iterator_category，那么就会进入到第二个test函数，返回的是char类型。
	* 如果没有iterator_catory，那么就会进入到第一个类型，返回2个char
	* 最后的value，如果返回的是一个char，那么就为1.否则为假
	* 
	* class MyClass {
		public:
			typedef mySimpleSTL::bidirectional_iterator_tag iterator_category;
		};
	int main() {
		std::cout << std::boolalpha;
		std::cout << mySimpleSTL::has_iterator<MyClass>::value << std::endl; // 输出true
		std::cout << mySimpleSTL::has_iterator<int>::value << std::endl; // 输出false
		return 0;
		}
	* 
	*/
	};


	template <class Iterator, bool>
	struct iterator_traits_impl {};

	template <class Iterator>
	struct iterator_traits_impl<Iterator, true>
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::difference_type   difference_type;
		/*
		* 示例：
		    typedef mySimpleSTL::iterator<mySimpleSTL::output_iterator_tag, int> output_iterator;
			typedef mySimpleSTL::iterator_traits_impl<output_iterator, mySimpleSTL::has_iterator<output_iterator>::value> traits;
			traits::value_type e{};
	*/
	};
	/*
	* 第二个参数是判断第一个模板类型是不是一个iterator，如果是那么因为偏特化进入第二个struct
	* 如果不是那么就进入第一个空的struct
	*/


	template <class Iterator, bool>
	struct iterator_traits_helper {};

	//template <class Iterator>
	//struct iterator_traits_helper<Iterator, true>
	//	: public iterator_traits_impl<Iterator,
	//	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	//	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>	// 如果iterator是input或者是output这其中之一，那么就进入iterator_traits_helper
	//{};
			// 这个版本应该是想的有继承关系就可以，但是我这里使用不行，我还是自己重新加了几个把

	template <class Iterator>
	struct iterator_traits_helper<Iterator, true>
		: public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value
		>
	{};
			

	// 萃取迭代器的特性
	template <class Iterator>
	struct iterator_traits
		: public iterator_traits_helper<Iterator, has_iterator<Iterator>::value> {};

	// 针对指针的偏特化版本
	template<typename T>
	struct iterator_traits<T*> {
		typedef typename random_access_iterator_tag iterator_category;
		typedef typename T							value_type;
		typedef typename T*							pointer;
		typedef typename T&							reference;
		typedef typename ptrdiff_t					difference_type;
	};


	// const T*
	template<typename T>
	struct iterator_traits<const T*> {
		typedef typename random_access_iterator_tag iterator_category;
		typedef typename  T							value_type;
		typedef typename const T*					pointer;
		typedef typename const T&					reference;
		typedef typename ptrdiff_t					difference_type;
	};
	
	// 判断一个容器里面的iterator_category的种类
	template <class T, class U, bool = has_iterator<iterator_traits<T>>::value>
	struct has_iterator_of
		: public  mySimpleSTL::bool_constant<std::is_convertible<
		typename  mySimpleSTL::iterator_traits<T>::iterator_category, U>::value>
	{};

	template <class T, class U>
	struct has_iterator_of<T, U, false> : public helper_bool_false {};

	/*
	* 示例
	using namespace mySimpleSTL;
template <typename T>
class MyContainer {
public:
    typedef T value_type;

    class iterator {
    public:
        // 定义迭代器类型
        typedef mySimpleSTL::random_access_iterator_tag  iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t difference_type;
    };

};

   
int main() {



    MyContainer<int> c;

    std::cout << "is random access iterator: "
        << has_iterator_of<decltype(c)::iterator, mySimpleSTL::input_iterator_tag >::value
        << std::endl;  // 输出1，即true

    return 0;
};
	* 
	*/


	// 简化has_iterator_of;
	// 将has_iterator_of<decltype(c)::iterator, mySimpleSTL::input_iterator_tag>
	// 变为is_input_iterator<decltype(c)>
	template <class T>
	using is_input_iterator = has_iterator_of<T, input_iterator_tag>;

	template <class T>
	using is_output_iterator = has_iterator_of<T, output_iterator_tag>;

	template <class T>
	using is_forward_iterator = has_iterator_of<T, forward_iterator_tag>;

	template <class T>
	using is_bidirectional_iterator = has_iterator_of<T, bidirectional_iterator_tag>;

	template <class T>
	using is_random_access_iterator = has_iterator_of<T, random_access_iterator_tag>;

	/*
	* 示例
	*     std::cout << "is input access iterator: "
        << has_iterator_of<MyContainer<int>, mySimpleSTL::input_iterator_tag>::value
        << std::endl;  // 输出1，即true
    // 现在，成功做到了
	*/

	// 萃取 category_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&)				// 这里未写参数，导致在使用mySimpleSTL::iterator_category(first)的时候无法使用
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	};
	// 萃取 value_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::value_type
		value_type()
	{
		return static_cast<iterator_traits<Iterator>::value_type>(0);
	};
	// 萃取 pointer
	template<typename Iterator>
	typename iterator_traits<Iterator>::pointer
		pointer_type()
	{
		return static_cast<iterator_traits<Iterator>::pointer>(nullptr);
	};
	// 萃取 distance_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::difference_type
		distance_type()
	{
		return static_cast<iterator_traits<Iterator>::difference_type>(0);
	};

	/*
	* 示例：
	*     
	* template <typename T>
class MyContainer {
public:
    typedef T value_type;

    class iterator {
    public:
        // 定义迭代器类型
        typedef mySimpleSTL::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t difference_type;
    };

    // ...
};
	using traits = mySimpleSTL::iterator_traits<MyContainer<int>::iterator>;
    std::cout << "Category: " << typeid(traits::iterator_category).name() << std::endl;
    std::cout << "Value type: " << typeid(traits::value_type).name() << std::endl;
    std::cout << "Pointer type: " << typeid(traits::pointer).name() << std::endl;
    std::cout << "Distance type: " << typeid(traits::difference_type).name() << std::endl;
	*/


	// 函数，计算两个迭代器之间的距离
	// 分别有input_iterator_tag,random_access_iterator_tag
	template<typename Iter>
	typename iterator_traits<Iter>::difference_type
		distance_dispatch(Iter bg, Iter ed, input_iterator_tag)				// 针对只能前进的iterator，比如链表
	{
		typename iterator_traits<Iter>::difference_type n{};
		while (bg++ != ed) {
			n++;
		};
		return n;
	};
	// random_access_iterator_tag
	template<typename Iter>
	typename iterator_traits<Iter>::difference_type
		distandistance_dispatchce(Iter bg, Iter ed, random_access_iterator_tag)
	{
		return ed - bg;
	};

	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		return distance_dispatch(first, last, iterator_category(first));
	}

	// 就是简单的判断两个迭代器之间的距离，这段因为需要传入
	/*
	* 示例：
	*     
	vector<int> vec{ 1, 2, 3, 4, 5 };

    // test random_access_iterator_tag version of distance
    auto dist = distance<>(&vec[0], &vec[3], mySimpleSTL::random_access_iterator_tag{});
    cout << "Distance using random_access_iterator_tag: " << dist << endl;
	*/


	// advance函数，分别对应三个迭代器类型
	// input,random,bidriction
	template <typename inputIter, typename Distance>
	void advance_dispatch(inputIter& bg, Distance n, input_iterator_tag) {
		while (n--)
			bg++;
	};
	template <typename RandomIter, typename Distance>
	void advance_dispatch(RandomIter& bg, Distance n, random_access_iterator_tag) {
		bg += n;
	};
	template <typename BidirectionalIterator, typename Distance>
	void advance_dispatch(BidirectionalIterator& bg, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (--n)bg++;
		else
			while (++n)bg--;
	};

	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n)
	{
		advance_dispatch(i, n, iterator_category(i));
	}

	/*
	* 示例：
	* template<typename T>
class MyIterator {
public:
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef mySimpleSTL::random_access_iterator_tag iterator_category;

    // Constructor
    MyIterator(pointer ptr) : _ptr(ptr) {}

    // Operator overloads
    reference operator*() const { return *_ptr; }
    pointer operator->() { return _ptr; }

    bool operator==(const MyIterator<T>& other) const {
        return _ptr == other._ptr;
    }
    bool operator!=(const MyIterator<T>& other) const {
        return !(*this == other);
    }

    // Prefix increment operator
    MyIterator<T>& operator++() {
        ++_ptr;
        return *this;
    }

    // Postfix increment operator
    MyIterator<T> operator++(int) {
        MyIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    pointer _ptr;
};



int main() {

    int arr[] = { 1, 2, 3, 4, 5 };

    MyIterator<int> iter(&arr[0]);

    mySimpleSTL::advance_dispatch(iter, 3, mySimpleSTL::input_iterator_tag{});

    std::cout << *iter << std::endl; // prints 4
    return 0;
};


这段代码不能用random_access，因为在MyIterator里面没有定义+=符号，定义了就可以使用
	* 
	*/

	// 反向迭代器
	// 是通过正向迭代器来实现的
	// 所需要的成员有：一个正向迭代器，迭代器5钟型别，构造，{* -> ++ -- + += - -= [] }
	// 上面括号中的操作符需要在全局重载一次
	template<typename Iterator>
	class reverse_iterator {
	private:
		Iterator current;
	public:
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef Iterator                                              iterator_type;
		typedef reverse_iterator<Iterator>                            self;

	public:

		template <class Iterator>
		friend bool operator==(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend bool operator<(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend bool operator!=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend bool operator>(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend bool operator<=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend bool operator>=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
	public:
		reverse_iterator() : current() {}
		explicit reverse_iterator(Iterator it) : current(it) {}
		reverse_iterator(const reverse_iterator& other) : current(other.current) {}

	public:

		self operator++() {
			--current;
			return *this;
		}

		self operator++(int) {
			self temp = *this;
			--current;
			return temp;
		}

		self operator--() {
			++current;
			return *this;
		}

		self operator--(int) {
			self temp = *this;
			++current;
			return temp;
		}

		reference operator*() const {
			Iterator temp = current;
			return *--temp;
		}

		pointer operator->() const {
			return &(operator*());
		}

		self operator+(difference_type n) const {
			return self(current - n);
		}

		self& operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		self operator-(difference_type n) const {
			return self(current + n);
		}

		self& operator-=(difference_type n) {
			current += n;
			return *this;
		}

		reference operator[](difference_type n) const {
			return *(*this + n);
		}
	};


	// {- == < != > >= <= != }
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return lhs.current == rhs.current;
	}

	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return lhs.current > rhs.current;
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return rhs < lhs;
	}

	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs < rhs);
	};

	/*
	 * reverse_iterator内部包含一个正向迭代器，具有以下5种迭代器型别：
	 * - iterator_category
	 * - value_type
	 * - difference_type
	 * - pointer
	 * - reference
	 *
	 * reverse_iterator还定义了自定义的内部operator，包括：
	 * - operator++()
	 * - operator++(int)
	 * - operator--()
	 * - operator--(int)
	 * - operator*()
	 * - operator->()
	 * - operator+()
	 * - operator+=()
	 * - operator-()
	 * - operator-=()
	 * - operator[]()
	 *
	 * 此外，reverse_iterator还定义了自定义的外部operator，并且这些operator是reverse_iterator的友元函数。
	* 
	* 
	* 
	* 示例：
	*     std::vector<int> v{ 1, 2, 3, 4, 5 };
    mySimpleSTL::reverse_iterator<std::vector<int>::iterator> rit(v.end());
    for (auto it = rit; it != mySimpleSTL::reverse_iterator<std::vector<int>::iterator>(v.begin()); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
	*/



	// 补，heap_algo需要用到value_type和distance_type
	// 萃取某个迭代器的 distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// 萃取某个迭代器的 value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

};
#endif
