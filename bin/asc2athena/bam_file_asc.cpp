#include "bam_file_asc.h"
#include "bam_exception.h"
#include "bam_math.h"

#include <sstream>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <xraylib.h>
#include <cmath>

BAM::ASCSingle::ASCSingle(std::string line) {
	std::istringstream iss(line);
	//this constructor will throw an exception if something went wrong!!
	iss.exceptions (std::istringstream::failbit | std::istringstream::badbit);
	try {
		iss >> Energy >> Ioni_1 >> Ioni_2 >> Ioni_3 >> BESSY >> Encoder >> Energy_Encoder;
		//std::cout << Energy << " " << Ioni_1 << " " << Ioni_2 << " " << Ioni_3 << " " << BESSY << " " << Encoder << " " << Energy_Encoder << std::endl;
	}
	catch (std::istringstream::failure &e) {
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
			fs.clear();
			std::string line;
			std::getline(fs, line);
			singles.push_back(BAM::ASCSingle(line));
		}
		catch (std::ifstream::failure &e) {
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
		catch (std::exception &e) {
			//since gcc 5.1.0, std::ios_base::failure exceptions are no longer caught by the eponymously named handler, but std::exception still works...
			if (fs.eof()) {
				std::cout << "EOF reached" << std::endl;
				break;
			}
			std::cerr << "std:exception -> " << e.what() << std::endl;
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

	//std::cout << "Before deriv" << std::endl;
	if (std::count(y_ref.begin(), y_ref.end(), double(0.0)) != y_ref.size()) {
		std::vector<double> y_deriv = BAM::deriv(x, y_ref);
		std::vector<double>::iterator y_deriv_max_iter = std::max_element(y_deriv.begin(), y_deriv.end());
		unsigned int y_deriv_max_offset = std::distance(y_deriv.begin(), y_deriv_max_iter);
		x_ref_deriv.push_back(x[y_deriv_max_offset]);
		//std::cout << "x[" << y_deriv_max_offset << "]: " << x[y_deriv_max_offset] << std::endl;
		//std::cout << "y_sample[" << y_deriv_max_offset << "]: " << y_sample[y_deriv_max_offset] << std::endl;
		//std::cout << "y_ref[" << y_deriv_max_offset << "]: " << y_ref[y_deriv_max_offset] << std::endl;
	}
	//std::cout << "After deriv" << std::endl;
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

void BAM::ASC::write(std::string new_filename, int atomic_number, int shell) {
	if (new_filename == "") {
		new_filename = filename.substr(0, filename.find_last_of(".")) + ".post.asc";
	}
	std::vector<double> x_print(x);
	std::vector<double> y_print(y_sample);
	if (atomic_number != -1 && shell != -1 && x_ref_deriv.size() != 0) {
		double energy_offset = x_ref_deriv[0] - EdgeEnergy(atomic_number, shell)*1000.0;
		std::cout << "energy_offset: " << energy_offset << std::endl;
		std::transform(x_print.begin(), x_print.end(), x_print.begin(),
             std::bind2nd(std::minus<double>(), energy_offset));
	}

	std::ofstream stream;
	stream.exceptions (std::ofstream::failbit | std::ofstream::badbit);
	stream.open(new_filename.c_str());
	stream << "Energy (eV)    mu Sample" << std::endl;
	for (unsigned int i = 0 ; i < x_print.size() ; i++) {
		stream << x_print[i] << "    " << y_print[i] << std::endl;
	}
	stream.close();
	std::cout << "Successfully written to " << new_filename << std::endl;
	return;
}
