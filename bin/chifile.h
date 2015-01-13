#ifndef CHIFILE_H
#define CHIFILE_H

#include <iostream>
#include <vector>
#include <string>


#define DEG2RAD(deg) (deg*M_PI/180.0)
#define RAD2DEG(rad) (rad*180.0/M_PI)

class Chi {
public:
	Chi(std::string filename);
	virtual ~Chi() {};

private:
	std::string x_title;		
	std::string y_title;		
	std::vector<double> x_values;
	std::vector<double> y_values;

};

#endif
