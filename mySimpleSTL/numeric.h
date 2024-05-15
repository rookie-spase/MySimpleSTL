#pragma once
#ifndef MYSIMPLESTL_NUMERIC_H
#define MYSIMPLESTL_NUMERIC_H

// 数值算法


namespace mySimpleSTL {

/*****************************************************************************************/
// accumulate
// 版本1：以初值 init 对每个元素进行累加
// 版本2：以初值 init 对每个元素进行二元操作
/*****************************************************************************************/

template<typename Iter,typename T>
T accumulate(Iter bg, Iter ed, T init)
{
	while (bg != ed) {
		init += *bg++;
	};
	return init;
};
template<typename Iter, typename T,typename op>
T accumulate(Iter bg, Iter ed, T init,op e)
{
	while (bg != ed) {
		init = e(init,*bg++);
	};
	return init;
}

/*****************************************************************************************/
// adjacent_difference
// 版本1：计算相邻元素的差值，结果保存到以 result 为起始的区间上
// 版本2：自定义相邻元素的二元操作
/*****************************************************************************************/


template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	auto prev = *first;
	while (++first != last) {
		auto curr = *first;
		*++result = curr - prev; // 计算相邻元素的差值
		prev = curr;
	}
	return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last) return result;
	*result = *first;
	auto prev = *first;
	while (++first != last) {
		auto curr = *first;
		*++result = binary_op(curr, prev); // 自定义相邻元素的二元操作
		prev = curr;
	}
	return ++result;
}




/*****************************************************************************************/
// inner_product
// 版本1：以 init 为初值，计算两个区间的内积   
// 版本2：自定义 operator+ 和 operator*
/*****************************************************************************************/

template<typename Iter,typename T>
T inner_product(Iter bg, Iter ed, Iter bg_2, T init) {
	while (bg != ed) {
		init += (*bg++) * (*bg_2++);
	}
	return init;
}
template<typename Iter, typename T, class BinaryOp1, class BinaryOp2>
T inner_product(Iter bg, Iter ed, Iter bg_2, T init, BinaryOp1 op1, BinaryOp2 op2) {
	while (bg != ed) {
		init = op1(init,op2(*bg++,*bg_2++));
	}
	return init;
}

/*****************************************************************************************/
// iota
// 填充[first, last)，以 value 为初值开始递增
/*****************************************************************************************/

template<typename Iter,typename T>
void iota(Iter bg, Iter ed, T val) {
	while (bg != ed) {
		*bg++ = val++;
	};
}

/*****************************************************************************************/
// partial_sum
// 版本1：计算局部累计求和，结果保存到以 result 为起始的区间上
// 版本2：进行局部进行自定义二元操作
/*****************************************************************************************/


template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	auto value = *first;
	while (++first != last) {
		value = value + *first;
		*++result = value;
	}
	return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last) return result;
	*result = *first;
	auto value = *first;
	while (++first != last) {
		value = binary_op(value, *first);
		*++result = value;
	}
	return ++result;
}



/*
示例：
	//测试accumulate
	vector<int> v1{ 1,2,3,4,5 };
	int sum = mySimpleSTL::accumulate(v1.begin(), v1.end(), 0);
	cout << "sum:" << sum << endl;
	int product = mySimpleSTL::accumulate(v1.begin(), v1.end(), 1, multiplies<int>());
	cout << "product:" << product << endl;

	//测试adjacent_difference
	vector<int> v2{ 1,2,4,7,11,16 };
	vector<int> v3(v2.size());
	mySimpleSTL::adjacent_difference(v2.begin(), v2.end(), v3.begin());
	for (auto i : v3) {
		cout << i << " ";
	}
	cout << endl;
	mySimpleSTL::adjacent_difference(v2.begin(), v2.end(), v3.begin(), plus<int>());
	for (auto i : v3) {
		cout << i << " ";
	}
	cout << endl;

	//测试inner_product
	vector<int> v4{ 1,2,3,4,5 };
	vector<int> v5{ 1,2,3,4,5 };
	int result = mySimpleSTL::inner_product(v4.begin(), v4.end(), v5.begin(), 0);
	cout << "result:" << result << endl;
	result = mySimpleSTL::inner_product(v4.begin(), v4.end(), v5.begin(), 0, plus<int>(), multiplies<int>());
	cout << "result:" << result << endl;

	//测试iota
	vector<int> v6(5);
	mySimpleSTL::iota(v6.begin(), v6.end(), 1);
	for (auto i : v6) {
		cout << i << " ";
	}
	cout << endl;

	//测试partial_sum
	vector<int> v7{ 1,2,4,7,11,16 };
	vector<int> v8(v7.size());
	mySimpleSTL::partial_sum(v7.begin(), v7.end(), v8.begin());
	for (auto i : v8) {
		cout << i << " ";
	}
	cout << endl;
	mySimpleSTL::partial_sum(v7.begin(), v7.end(), v8.begin(), multiplies<int>());
	for (auto i : v8) {
		cout << i << " ";
	}
	cout << endl;
*/

}




#endif