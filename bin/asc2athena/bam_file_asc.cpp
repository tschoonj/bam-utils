#include "bam_file_asc.h"
#include "bam_exception.h"
#include "bam_math.h"

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

	if (std::count(y_ref.begin(), y_ref.end(), double(0.0)) != y_ref.size()) {
		std::vector<double> y_deriv = BAM::deriv(x, y_ref);
		std::vector<double>::iterator y_deriv_max_iter = std::max_element(y_deriv.begin(), y_deriv.end());
		unsigned int y_deriv_max_offset = std::distance(y_deriv.begin(), y_deriv_max_iter);
		x_ref_deriv.push_back(x[y_deriv_max_offset]);
		std::cout << "x[" << y_deriv_max_offset << "]: " << x[y_deriv_max_offset] << std::endl;
		std::cout << "y_ref[" << y_deriv_max_offset << "]: " << y_ref[y_deriv_max_offset] << std::endl;

		/*gsl_interp_accel *acc = gsl_interp_accel_alloc();
  	gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, x.size());

  	gsl_spline_init(spline, &x[0], &y_ref[0], x.size());


		for (unsigned int i = 0 ; i < x.size() ; i++) {
			//x_deriv[i] = x[0] + (x[x.size()-1] - x[0])/(99999 - 0)*(i - 0);
			//std::cout << "x_deriv[" << i << "]: " << x_deriv[i] << std::endl;
			if (gsl_spline_eval_deriv2_e(spline, x[i], acc, &y_deriv[i]) != GSL_SUCCESS) {
				std::cerr << "gsl_spline_eval_deriv2_e returned an error at " << i << " -> " << x[i] << std::endl;
				exit(1);
			}
			std::cout << "x[" << i << "]: " << x[i] << std::endl;
			std::cout << "y_deriv[" << i << "]: " << y_deriv[i] << std::endl;
		}
		gsl_spline_free(spline);
  	gsl_interp_accel_free(acc);
		*/
		/*
		std::cout << "y_ref minimum: " << *std::min_element(y_ref.begin(), y_ref.end()) << std::endl;
		std::cout << "y_ref maximum: " << *std::max_element(y_ref.begin(), y_ref.end()) << std::endl;

		for (unsigned int i = std::distance(y_ref.begin(), std::min_element(y_ref.begin(), y_ref.end()))+20;
		                  i < std::distance(y_ref.begin(), std::max_element(y_ref.begin(), y_ref.end()));
											i++) {
			std::cout << "y_ref[" << i << "]: " << y_ref[i] << std::endl;
			std::cout << "y_deriv[" << i << "]: " << y_deriv[i] << std::endl;
			if (y_deriv[i-1] > y_deriv[i]) {
				x_ref_deriv.push_back(0.5*(x[i-1]+x[i]));
				std::cout << "y_ref[" << i+1 << "]: " << y_ref[i+1] << std::endl;
			  std::cout << "y_deriv[" << i+1 << "]: " << y_deriv[i+1] << std::endl;
				std::cout << "second deriv root found at: " << x_ref_deriv[0] << std::endl;
				std::cout << "second deriv root found for : " << y_ref[i] << std::endl;
				break;
			}
		}*/
	}
}

void BAM::ASC::plot(Gtk::Window &parent) {
	if (plplotwindow) {
		plplotwindow->show();
		return;
	}

	if (std::count(y_ref.begin(), y_ref.end(), double(0.0)) != y_ref.size()) {
		std::vector<BAM::PlPlotDrawingAreaData> area_data;
		area_data.push_back(BAM::PlPlotDrawingAreaData(x, y_sample, filename, "Energy (eV)", "Absorbance", "Sample data"));
		area_data.push_back(BAM::PlPlotDrawingAreaData(x, y_ref, filename, "Energy (eV)", "Absorbance", "Reference foil data"));
	  plplotwindow = new BAM::PlPlotWindow(area_data);
  }
	else {
	  plplotwindow = new BAM::PlPlotWindow(x, y_sample, filename, "Energy (eV)", "Absorbance");
	}
  plplotwindow->set_transient_for(parent);
	plplotwindow->property_destroy_with_parent() = true;

  plplotwindow->show();
}
