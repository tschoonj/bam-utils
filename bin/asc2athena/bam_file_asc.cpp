#include "bam_file_asc.h"

#include <sstream>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>

BAM::ASCSingle::ASCSingle(std::string line) {
	std::istringstream iss(line);
	//this constructor will throw an exception if something went wrong!!
	iss.exceptions (std::istringstream::failbit | std::istringstream::badbit | std::istringstream::eofbit);
	try {
		iss >> Energy >> Ioni_1 >> Ioni_2 >> Ioni_3 >> BESSY >> Encoder >> Energy_Encoder;
		//std::cout << Energy << " " << Ioni_1 << " " << Ioni_2 << " " << Ioni_3 << " " << BESSY << " " << Encoder << " " << Energy_Encoder << std::endl;
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

BAM::ASC::ASC(std::string filename) : plplotwindow(0), filename(filename) {
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
				//std::cout << "EOF reached" << std::endl;
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

  for (unsigned int i = 0 ; i < singles.size() ; i++) {
		x.push_back(singles[i].Energy_Encoder*1000.0);
		y_sample.push_back(-1.0*log(singles[i].Ioni_2/singles[i].Ioni_1));
		//the third ionization is sometimes not connected and returns then zero
		if (singles[i].Ioni_3 > 0.0)
			y_ref.push_back(-1.0*log(singles[i].Ioni_3/singles[i].Ioni_2));
		else
			y_ref.push_back(0.0);
	}
	gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, x.size());

  gsl_spline_init(spline, &x[0], &y_ref[0], x.size());

	//calculate the second derivative for a lot of values...
	/*std::vector<double> x_deriv(100000), y_deriv(100000);
  for (unsigned int i = 0 ; i < 100000 ; i++) {
		x_deriv[i] = x[0] + (x[x.size()-1] - x[0])/(99999 - 0)*(i - 0);
		//std::cout << "x_deriv[" << i << "]: " << x_deriv[i] << std::endl;
		if (gsl_spline_eval_deriv2_e(spline, x_deriv[i], acc, &y_deriv[i]) != GSL_SUCCESS) {
			std::cerr << "gsl_spline_eval_deriv2_e returned an error at " << i << " -> " << x_deriv[i] << std::endl;
			exit(1);
		}
	}*/
	std::vector<double> y_deriv(x.size());
	for (unsigned int i = 0 ; i < x.size() ; i++) {
		//x_deriv[i] = x[0] + (x[x.size()-1] - x[0])/(99999 - 0)*(i - 0);
		//std::cout << "x_deriv[" << i << "]: " << x_deriv[i] << std::endl;
		if (gsl_spline_eval_deriv2_e(spline, x[i], acc, &y_deriv[i]) != GSL_SUCCESS) {
			std::cerr << "gsl_spline_eval_deriv2_e returned an error at " << i << " -> " << x[i] << std::endl;
			exit(1);
		}
	}
	for (unsigned int i = 1 ; i < x.size(); i++) {
		if (y_deriv[i-1] * y_deriv[i] < 0) {
			x_ref_deriv.push_back(x[i-1]);
			std::cout << "second deriv root found at: " << x[i-1] << std::endl;
		}
	}


	gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
}

void BAM::ASC::plot(Gtk::Window &parent) {
	if (plplotwindow) {
		plplotwindow->show();
		return;
	}

	plplotwindow = new BAM::PlPlotWindow(x, y_ref, filename);
  plplotwindow->set_transient_for(parent);
	plplotwindow->property_destroy_with_parent() = true;

  plplotwindow->show();
}
