#include <iostream>
#include "tokenizer.hpp"

int main()
{
	std::cout << group("these", "are", "some", "items") << '\n';
	std::cout << group("only one here!") << '\n';
	std::cout << StringPrefix << '\n';
	return 0;
}
