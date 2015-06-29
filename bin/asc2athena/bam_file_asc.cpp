#include "bam_file_asc.h"

#include <sstream>
#include <fstream>
#include <exception>
#include <cstdlib>


BAM::ASCSingle::ASCSingle(std::string line) {
	std::istringstream iss(line);
	//this constructor will throw an exception if something went wrong!!
	iss.exceptions (std::istringstream::failbit | std::istringstream::badbit | std::istringstream::eofbit);
	try {
		iss >> Energy >> Ioni_1 >> Ioni_2 >> Ioni_3 >> BESSY >> Encoder >> Energy_Encoder;
		std::cout << Energy << " " << Ioni_1 << " " << Ioni_2 << " " << Ioni_3 << " " << BESSY << " " << Encoder << " " << Energy_Encoder << std::endl;
	}
	catch (std::ios_base::failure &e) {
		if (iss.eof())
			throw BAM::Exception("eof IO failure");
		if (iss.bad())
			throw BAM::Exception("bad failure");
		if (iss.fail())
			throw BAM::Exception("fail failure");
	}
}

BAM::ASC::ASC(std::string filename) {
	std::ifstream fs;
	fs.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
	fs.open(filename.c_str());
	std::string rubbish;
	std::getline(fs, rubbish);
	
	while (1) {
		try {
			std::string line;
			std::getline(fs, line);
			singles.push_back(BAM::ASCSingle(line));
		}
		catch (std::ios_base::failure &e) {
			if (fs.eof()) {
				std::cout << "EOF reached" << std::endl;
				break;
			}
			std::cerr << "std::io_stream::failure -> " << e.what() << std::endl;
			exit(1);
		}
		catch (BAM::Exception &e) {
			std::cerr << "BAM:Exception -> " << e.what() << std::endl;
			exit(1);
		}
		catch (...) {
			std::cerr << "unknown exception" << std::endl;
			exit(1);
		}
	}
	fs.close();
}
