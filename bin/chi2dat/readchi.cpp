#include "chifile.h"
#include <iostream>

int main (int argc, char *argv[]) {

	if (argc != 2) {
		std::cout << "No argument found" << std::endl;
		return 1;	
	}


	try {
		Chi chi(argv[1]);	
	}
	catch (std::exception &e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	return 0;
}
