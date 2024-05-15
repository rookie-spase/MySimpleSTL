#pragma once
#ifndef MYSIMPLESTL_TYPE_TRAITS_H
#define MYSIMPLESTL_TYPE_TRAITS_H
#include<iostream>
#include<tuple>
namespace mySimpleSTL {

	 //����ģ������ڳ���
	template<typename T, T val>
	struct helper_integral_constant {
		static constexpr T value = val;
	};

	
	template <bool b>
	using bool_constant = helper_integral_constant<bool, b>;
	// ����bool����
	using helper_bool_false = helper_integral_constant<bool, false>; // helper_bool_false::value����false
	using helper_bool_true = helper_integral_constant<bool, true>;
	
	




	// �����ж�һ�������ǲ���pair
	// ������pair�������
	template<typename T1, typename T2>
	class pair;
	// һ�㴫������ͣ�����Ϊ��false���̳�false
	template<typename T>
	class is_pair :public helper_bool_false {};
	// pair���ػ����������Ǹ�pair��ʱ��̳�true����
	template<typename T1, typename T2>
	class is_pair<std::pair<T1, T2>> : public helper_bool_true {};

};
#endif