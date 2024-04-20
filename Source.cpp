#include <iostream>
#include "MultiSet.h"

int main() {
	try {
		MultiSet ms(8, 3);

		ms.add(1);
		ms.add(1);
		ms.add(2);
		ms.add(0);
		ms.add(2);
		ms.countNumInSet(77);
		ms.printMem();
		ms.print();
		//ms.writeToBinaryFile("test.dat");

		MultiSet ms2(5, 2);
		ms2.add(1);
		ms2.add(1);

		MultiSet ms3 = intersection(ms, ms2);
		MultiSet m4 = ms - ms2;
		MultiSet m5 = addition(ms2);
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}
}
