#include <iostream>
#include "MultiSet.h"

int main() {
	//std::cout << (((3 << 6) & 81) >> 6);
	MultiSet ms;
	ms.add(3);
	std::cout <<ms.countNumInSet(3);
}
