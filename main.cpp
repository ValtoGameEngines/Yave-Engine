
#include <iostream>
#include <n/types.h>
#include <n/core/Collection.h>
#include <n/core/List.h>
#include <n/core/Map.h>
#include <n/core/Range.h>
#include <n/core/Timer.h>

using namespace n;
using namespace n::core;


struct CC
{
	using iterator = int*;
	using const_iterator = const int*;

	iterator begin() { return 0; }
	iterator end() { return 0; }


	const_iterator begin() const { return 0; }
	const_iterator end() const { return 0; }

	CC() {
	}

	CC(const CC &) {
		std::cout << "CPY !" << std::endl;
	}
};

int main(int, char **) {

	Array<int> l;
	l << 1 << 2 << 3 << 5 << 6;

	l.insert(4, l.find(5));

	for(auto x: l) {
		std::cout << x << " ";
	}
	std::cout << std::endl;

	auto e = l.end();
	auto b = l.begin();
	b--;
	for(auto i = reverseIterator(--e); i != reverseIterator(b); i++) {
		std::cout << *i << " ";
	}
	std::cout << std::endl << std::endl;

	for(int i : range(l)) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	for(int i : range(l).reverse()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;


	for(int i : range(1, 7).reverse()) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	return 0;
}
