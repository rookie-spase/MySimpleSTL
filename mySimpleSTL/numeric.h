#pragma once
#ifndef MYSIMPLESTL_NUMERIC_H
#define MYSIMPLESTL_NUMERIC_H

// ��ֵ�㷨


namespace mySimpleSTL {

/*****************************************************************************************/
// accumulate
// �汾1���Գ�ֵ init ��ÿ��Ԫ�ؽ����ۼ�
// �汾2���Գ�ֵ init ��ÿ��Ԫ�ؽ��ж�Ԫ����
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
// �汾1����������Ԫ�صĲ�ֵ��������浽�� result Ϊ��ʼ��������
// �汾2���Զ�������Ԫ�صĶ�Ԫ����
/*****************************************************************************************/


template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	auto prev = *first;
	while (++first != last) {
		auto curr = *first;
		*++result = curr - prev; // ��������Ԫ�صĲ�ֵ
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
		*++result = binary_op(curr, prev); // �Զ�������Ԫ�صĶ�Ԫ����
		prev = curr;
	}
	return ++result;
}




/*****************************************************************************************/
// inner_product
// �汾1���� init Ϊ��ֵ����������������ڻ�   
// �汾2���Զ��� operator+ �� operator*
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
// ���[first, last)���� value Ϊ��ֵ��ʼ����
/*****************************************************************************************/

template<typename Iter,typename T>
void iota(Iter bg, Iter ed, T val) {
	while (bg != ed) {
		*bg++ = val++;
	};
}

/*****************************************************************************************/
// partial_sum
// �汾1������ֲ��ۼ���ͣ�������浽�� result Ϊ��ʼ��������
// �汾2�����оֲ������Զ����Ԫ����
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
ʾ����
	//����accumulate
	vector<int> v1{ 1,2,3,4,5 };
	int sum = mySimpleSTL::accumulate(v1.begin(), v1.end(), 0);
	cout << "sum:" << sum << endl;
	int product = mySimpleSTL::accumulate(v1.begin(), v1.end(), 1, multiplies<int>());
	cout << "product:" << product << endl;

	//����adjacent_difference
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

	//����inner_product
	vector<int> v4{ 1,2,3,4,5 };
	vector<int> v5{ 1,2,3,4,5 };
	int result = mySimpleSTL::inner_product(v4.begin(), v4.end(), v5.begin(), 0);
	cout << "result:" << result << endl;
	result = mySimpleSTL::inner_product(v4.begin(), v4.end(), v5.begin(), 0, plus<int>(), multiplies<int>());
	cout << "result:" << result << endl;

	//����iota
	vector<int> v6(5);
	mySimpleSTL::iota(v6.begin(), v6.end(), 1);
	for (auto i : v6) {
		cout << i << " ";
	}
	cout << endl;

	//����partial_sum
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