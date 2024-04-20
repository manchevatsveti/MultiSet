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
		std::cout << (int)ms.countNumInSet(2) << std::endl;;
		ms.print();
		ms.printMem();
		ms.writeToBinaryFile("test.dat");

		MultiSet msBinary(1, 1);
		msBinary.readFromBinaryFile("test.dat");

		MultiSet ms2(5, 2);
		ms2.add(1);
		ms2.add(1);

		MultiSet ms3 = intersection(ms, ms2);
		ms3.print();

		MultiSet m4 = ms - ms2;
		m4.print();

		MultiSet m5 = addition(ms);
		std::cout << (int)m5.countNumInSet(2) << std::endl;;

	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}
