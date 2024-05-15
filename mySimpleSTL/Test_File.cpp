#include "Test_File.h"

using namespace mySimpleSTL;

void mySimpleSTL::aaa()
{
	vector<int> num;
	for (int i = 0; i < 100; ++i) {
		num.push_back(i);
	}

	for (auto e : num)
		std::cout << e << " ";

}
