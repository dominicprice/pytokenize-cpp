#include <iostream>
#include <sstream>
#include "tokenize.hpp"

//#define CATCH_EXCEPTIONS

int main()
{
	using namespace tokenize_py;

	std::stringstream code;
	code << "for i in range(2): print(i)";

#ifdef CATCH_EXCEPTIONS
	try {
#endif
		auto tokens = tokenize(code);
		for (const auto& token : tokens) {
			std::cout << token << '\n';
		}
#ifdef CATCH_EXCEPTIONS
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
		return 1;
	}
#endif
	return 0;
}
