#pragma once
#ifndef MYSIMPLESTL_TYPE_TRAITS_H
#define MYSIMPLESTL_TYPE_TRAITS_H
#include<iostream>
#include<tuple>
namespace mySimpleSTL {

	 //定义模板编译期常量
	template<typename T, T val>
	struct helper_integral_constant {
		static constexpr T value = val;
	};

	
	template <bool b>
	using bool_constant = helper_integral_constant<bool, b>;
	// 定义bool常量
	using helper_bool_false = helper_integral_constant<bool, false>; // helper_bool_false::value就是false
	using helper_bool_true = helper_integral_constant<bool, true>;
	
	




	// 用来判断一个类型是不是pair
	// 声明有pair这个东西
	template<typename T1, typename T2>
	class pair;
	// 一般传入的类型，被认为是false，继承false
	template<typename T>
	class is_pair :public helper_bool_false {};
	// pair的特化，当传入是个pair的时候继承true类型
	template<typename T1, typename T2>
	class is_pair<std::pair<T1, T2>> : public helper_bool_true {};

};
#endif