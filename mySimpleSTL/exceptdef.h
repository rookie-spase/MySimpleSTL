#pragma once
#ifndef MYSIMPLESTL_EXCEPTDEF_H
#define MYSIMPLESTL_EXCEPTDEF_H

#include <stdexcept>
#include <string>
namespace mySimpleSTL {
	// debug
	template <bool expr>
	struct MySimpleSTL_Debug_Assert {
		static_assert(expr, "MySimpleSTL Debug Assertion Failed!");
	};
#define MYSIMPLESTL_DEBUG(expr) \
    MySimpleSTL_Debug_Assert<static_cast<bool>(expr)>()

	// 长度
	constexpr const
		auto THROW_LENGTH_ERROR_IF = [](bool expr, const std::string& what) -> void {
		if (expr) {
			throw std::length_error(what);
		}
	};

	// 范围
	constexpr const
		auto THROW_RANGE_ERROR_IF = [](bool expr, const std::string& what) -> void {
		if (expr) {
			throw std::out_of_range(what);
		}
	};

	// 运行时
	constexpr const
		auto  THROW_RUNTIME_ERROR_IF = [](bool expr, const std::string& what) -> void {
		if (expr) {
			throw std::runtime_error(what);
		}
	};
};




/* 示例
*     try {
        mySimpleSTL::THROW_RANGE_ERROR_IF(1,"out of range");
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
*/

#endif