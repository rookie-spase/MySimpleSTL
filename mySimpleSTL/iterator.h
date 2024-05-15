#pragma once
#ifndef MYSIMPLESTL_ITERATOR_H
#define MYSIMPLESTL_ITERATOR_H

#include<cstddef>
 // ����ļ�ʵ��iterator

namespace mySimpleSTL {

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};     // ������ǰ����ƶ�
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};// ����ָ��Ӽ��ƶ�


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


	// ���ڼ��ĳһ�������Ƿ���ж�iterator_category�Ķ���
	template<typename T>
	struct has_iterator {
	private:
		struct two { char a; char b; };
		template<typename U> static constexpr two test(...);
		template<typename U> static constexpr char test(typename U::iterator_category* = 0);
	public:
		constexpr const static bool value = (sizeof(test<T>(0)) == sizeof(char));
	/*
	* ��������о���iterator_category����ô�ͻ���뵽�ڶ���test���������ص���char���͡�
	* ���û��iterator_catory����ô�ͻ���뵽��һ�����ͣ�����2��char
	* ����value��������ص���һ��char����ô��Ϊ1.����Ϊ��
	* 
	* class MyClass {
		public:
			typedef mySimpleSTL::bidirectional_iterator_tag iterator_category;
		};
	int main() {
		std::cout << std::boolalpha;
		std::cout << mySimpleSTL::has_iterator<MyClass>::value << std::endl; // ���true
		std::cout << mySimpleSTL::has_iterator<int>::value << std::endl; // ���false
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
		* ʾ����
		    typedef mySimpleSTL::iterator<mySimpleSTL::output_iterator_tag, int> output_iterator;
			typedef mySimpleSTL::iterator_traits_impl<output_iterator, mySimpleSTL::has_iterator<output_iterator>::value> traits;
			traits::value_type e{};
	*/
	};
	/*
	* �ڶ����������жϵ�һ��ģ�������ǲ���һ��iterator���������ô��Ϊƫ�ػ�����ڶ���struct
	* ���������ô�ͽ����һ���յ�struct
	*/


	template <class Iterator, bool>
	struct iterator_traits_helper {};

	//template <class Iterator>
	//struct iterator_traits_helper<Iterator, true>
	//	: public iterator_traits_impl<Iterator,
	//	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	//	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>	// ���iterator��input������output������֮һ����ô�ͽ���iterator_traits_helper
	//{};
			// ����汾Ӧ��������м̳й�ϵ�Ϳ��ԣ�����������ʹ�ò��У��һ����Լ����¼��˼�����

	template <class Iterator>
	struct iterator_traits_helper<Iterator, true>
		: public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value
		>
	{};
			

	// ��ȡ������������
	template <class Iterator>
	struct iterator_traits
		: public iterator_traits_helper<Iterator, has_iterator<Iterator>::value> {};

	// ���ָ���ƫ�ػ��汾
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
	
	// �ж�һ�����������iterator_category������
	template <class T, class U, bool = has_iterator<iterator_traits<T>>::value>
	struct has_iterator_of
		: public  mySimpleSTL::bool_constant<std::is_convertible<
		typename  mySimpleSTL::iterator_traits<T>::iterator_category, U>::value>
	{};

	template <class T, class U>
	struct has_iterator_of<T, U, false> : public helper_bool_false {};

	/*
	* ʾ��
	using namespace mySimpleSTL;
template <typename T>
class MyContainer {
public:
    typedef T value_type;

    class iterator {
    public:
        // �������������
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
        << std::endl;  // ���1����true

    return 0;
};
	* 
	*/


	// ��has_iterator_of;
	// ��has_iterator_of<decltype(c)::iterator, mySimpleSTL::input_iterator_tag>
	// ��Ϊis_input_iterator<decltype(c)>
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
	* ʾ��
	*     std::cout << "is input access iterator: "
        << has_iterator_of<MyContainer<int>, mySimpleSTL::input_iterator_tag>::value
        << std::endl;  // ���1����true
    // ���ڣ��ɹ�������
	*/

	// ��ȡ category_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&)				// ����δд������������ʹ��mySimpleSTL::iterator_category(first)��ʱ���޷�ʹ��
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	};
	// ��ȡ value_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::value_type
		value_type()
	{
		return static_cast<iterator_traits<Iterator>::value_type>(0);
	};
	// ��ȡ pointer
	template<typename Iterator>
	typename iterator_traits<Iterator>::pointer
		pointer_type()
	{
		return static_cast<iterator_traits<Iterator>::pointer>(nullptr);
	};
	// ��ȡ distance_type
	template<typename Iterator>
	typename iterator_traits<Iterator>::difference_type
		distance_type()
	{
		return static_cast<iterator_traits<Iterator>::difference_type>(0);
	};

	/*
	* ʾ����
	*     
	* template <typename T>
class MyContainer {
public:
    typedef T value_type;

    class iterator {
    public:
        // �������������
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


	// ��������������������֮��ľ���
	// �ֱ���input_iterator_tag,random_access_iterator_tag
	template<typename Iter>
	typename iterator_traits<Iter>::difference_type
		distance_dispatch(Iter bg, Iter ed, input_iterator_tag)				// ���ֻ��ǰ����iterator����������
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

	// ���Ǽ򵥵��ж�����������֮��ľ��룬�����Ϊ��Ҫ����
	/*
	* ʾ����
	*     
	vector<int> vec{ 1, 2, 3, 4, 5 };

    // test random_access_iterator_tag version of distance
    auto dist = distance<>(&vec[0], &vec[3], mySimpleSTL::random_access_iterator_tag{});
    cout << "Distance using random_access_iterator_tag: " << dist << endl;
	*/


	// advance�������ֱ��Ӧ��������������
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
	* ʾ����
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


��δ��벻����random_access����Ϊ��MyIterator����û�ж���+=���ţ������˾Ϳ���ʹ��
	* 
	*/

	// ���������
	// ��ͨ�������������ʵ�ֵ�
	// ����Ҫ�ĳ�Ա�У�һ�������������������5���ͱ𣬹��죬{* -> ++ -- + += - -= [] }
	// ���������еĲ�������Ҫ��ȫ������һ��
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
	 * reverse_iterator�ڲ�����һ���������������������5�ֵ������ͱ�
	 * - iterator_category
	 * - value_type
	 * - difference_type
	 * - pointer
	 * - reference
	 *
	 * reverse_iterator���������Զ�����ڲ�operator��������
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
	 * ���⣬reverse_iterator���������Զ�����ⲿoperator��������Щoperator��reverse_iterator����Ԫ������
	* 
	* 
	* 
	* ʾ����
	*     std::vector<int> v{ 1, 2, 3, 4, 5 };
    mySimpleSTL::reverse_iterator<std::vector<int>::iterator> rit(v.end());
    for (auto it = rit; it != mySimpleSTL::reverse_iterator<std::vector<int>::iterator>(v.begin()); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
	*/



	// ����heap_algo��Ҫ�õ�value_type��distance_type
	// ��ȡĳ���������� distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// ��ȡĳ���������� value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

};
#endif
