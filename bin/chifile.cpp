#include "chifile.h"
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <cmath>

const double lambda1 = 1;
const double lambda2 = 1.5406;


Chi::Chi(std::string filename) {
	std::ifstream fs;
	fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fs.open(filename.c_str());
	std::string rubbish;
	std::getline(fs, rubbish);
	std::getline(fs, x_title);
	std::getline(fs, y_title);
	std::string nrows_s;
	std::getline(fs, nrows_s);
	std::stringstream tempss;
	tempss << nrows_s;
	int nrows_i;
	tempss >> nrows_i;
	x_values.resize(nrows_i);
	y_values.resize(nrows_i);
	
	for (int i = 0 ; i < nrows_i ; i++) {
		std::string data;
		std::stringstream ss;
		std::getline(fs, data);
		ss << data;
		double x_value, y_value;
		ss >> x_value >> y_value;
		x_values[i] = RAD2DEG(2.0*asin(lambda2*sin(DEG2RAD(x_value)/2.0)/lambda1));
		y_values[i] = y_value;
		std::cout << "x_values[i]: " << x_values[i] << " y_values[i]: " << y_values[i] << std::endl;
	}


	fs.close();
}
