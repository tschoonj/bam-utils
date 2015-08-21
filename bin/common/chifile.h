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
	Chi() {};
	virtual ~Chi() {};
	double *GetXPtr() {
		if (x_values.size() == 0)
			return 0;
		return &x_values[0];
	}
	double *GetYPtr() {
		if (y_values.size() == 0)
			return 0;
		return &y_values[0];
	}
	int GetSize() {
		return x_values.size();
	}
	std::vector<double> GetXVector() {
		return x_values;
	}
	std::vector<double> GetYVector() {
		return y_values;
	}
	std::string GetXTitle() {
		return x_title;
	}
	std::string GetYTitle() {
		return y_title;
	}
private:
	std::string x_title;
	std::string y_title;
	std::vector<double> x_values;
	std::vector<double> y_values;

};

#endif
