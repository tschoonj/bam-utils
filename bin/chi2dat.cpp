#include "chi2dat.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <glibmm/miscutils.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>
#include <cmath>
#include <glibmm/convert.h>

Chi2DatWindow::Chi2DatWindow() : convert("Convert!") {
	Gtk::Label *label;

	//initialize window properly
	set_border_width(12);
	set_default_size(500, 400);
	grid.set_column_spacing(5);
        grid.set_row_spacing(5);
        grid.set_row_homogeneous(false);
        grid.set_column_homogeneous(false);
	add(grid);

	//treeview
	model = Gtk::ListStore::create(chi_files_columns);
	tv.set_model(model);
	tv.append_column("Filename", chi_files_columns.col_filename_basename);
	sw.add(tv);
	sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	grid.attach(sw, 0, 0, 1, 3);
	sw.set_vexpand();
	sw.set_hexpand();

	label = Gtk::manage(new Gtk::Label("Start value"));
	grid.attach(*label, 1, 0, 1, 1);
	start_value.set_text("5.0");
	grid.attach(start_value, 2, 0, 1, 1);

	label = Gtk::manage(new Gtk::Label("Step size"));
	grid.attach(*label, 1, 1, 1, 1);
	step_size.set_text("0.5");
	grid.attach(step_size, 2, 1, 1, 1);

	label = Gtk::manage(new Gtk::Label("# steps"));
	grid.attach(*label, 1, 2, 1, 1);
	nsteps.set_text("1000");
	grid.attach(nsteps, 2, 2, 1, 1);


	open.set_image_from_icon_name("document-open");
        open.set_vexpand(false);
        open.set_hexpand(false);
        open.set_valign(Gtk::ALIGN_CENTER);
        open.set_halign(Gtk::ALIGN_CENTER);
	open.signal_clicked().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_open_clicked));
	grid.attach(open, 0, 3, 1, 1);

        convert.set_vexpand(false);
        convert.set_hexpand(false);
        convert.set_valign(Gtk::ALIGN_CENTER);
        convert.set_halign(Gtk::ALIGN_CENTER);
	convert.set_sensitive(false);
	convert.signal_clicked().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_convert_clicked));
	grid.attach(convert, 1, 3, 2, 1);


	signal_delete_event().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_delete_event));
	tv.signal_key_press_event().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_backspace_clicked));
	show_all_children();
}

Chi2DatWindow::~Chi2DatWindow() {}

bool Chi2DatWindow::on_delete_event(GdkEventAny* event) {
        std::cout << "Window delete event" << std::endl;
        get_application()->remove_window(*this);
        return true;
}

template<typename T> T Chi2DatWindow::get_value_from_entry(Gtk::Entry &entry) {
	std::istringstream ss(entry.get_text());
	T rv;
	ss >> rv;
	if (ss.fail()) {
		throw std::runtime_error("could not convert entry contents to a number");
	}
	return rv;
}

void Chi2DatWindow::on_convert_clicked() {
        std::cout << "Convert clicked" << std::endl;

	double start_value_d;
	double step_size_d;
	int nsteps_i;

	try {
		start_value_d = get_value_from_entry<double>(start_value);
		std::cout << "start_value_d: " << start_value_d << std::endl;
	}
	catch (std::exception &e) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("Start value: ")+e.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                mdialog.run();
		return;
	}
	try {
		step_size_d = get_value_from_entry<double>(step_size);
		std::cout << "step_size_d: " << step_size_d << std::endl;
	}
	catch (std::exception &e) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("Step size: ")+e.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                mdialog.run();
		return;
	}
	try {
		nsteps_i = get_value_from_entry<int>(nsteps);
		std::cout << "nsteps_i: " << nsteps_i << std::endl;
	}
	catch (std::exception &e) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("# steps: ")+e.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                mdialog.run();
		return;
	}


	Gtk::TreeModel::Children kids = model->children();
        for (Gtk::TreeModel::Children::iterator iter = kids.begin() ; iter != kids.end() ; ++iter) {
                Gtk::TreeModel::Row row = *iter;
		Chi chi = row[chi_files_columns.col_chi];
		double *x_values_orig = chi.GetXPtr();
		double *y_values = chi.GetYPtr();
		std::vector<double> x_values(chi.GetSize());
		gsl_interp_accel *acc = gsl_interp_accel_alloc ();
    		gsl_interp *interp = gsl_interp_alloc (gsl_interp_cspline, chi.GetSize());
		for (int i = 0 ; i < chi.GetSize() ; i++) {
			x_values[i] = RAD2DEG(2.0*asin(x_values_orig[i]*0.15406/(4.0*M_PI)));
		}
		
		gsl_interp_init (interp, &x_values[0], chi.GetYPtr(), chi.GetSize());

		//try writing to file
		std::string datfile = Glib::locale_from_utf8(row[chi_files_columns.col_filename_full]);
		datfile.replace(datfile.end()-3, datfile.end(), "dat");
	
		std::cout << "Filename: " << datfile << std::endl;

		std::ofstream fs(datfile.c_str());
		if (!fs.is_open()) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Could not open ")+datfile+Glib::ustring(" for writing"), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                	mdialog.run();
			return;
		}

    		for (int i = 0 ; i <= nsteps_i ; i++) {
			double x = start_value_d + i * step_size_d;
			double y;
			int e = gsl_interp_eval_e(interp, &x_values[0], chi.GetYPtr(), x, acc, &y);
			if (e == GSL_EDOM)
				continue;
			//std::cout << "x: " << x << " y: " << y << std::endl;
			fs << x << "\t" << y << std::endl;
		}

		fs.close();
		gsl_interp_free (interp);
		gsl_interp_accel_free (acc);
        }
	
	

}

void Chi2DatWindow::on_open_clicked() {
	Gtk::FileChooserDialog dialog(*this, "Please select a number of AXIL results files", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_select_multiple();
        Glib::RefPtr<Gtk::FileFilter> filter_chi = Gtk::FileFilter::create();
        filter_chi->set_name("CHI files");
        filter_chi->add_pattern("*.chi");
        filter_chi->add_pattern("*.CHI");
        dialog.add_filter(filter_chi);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("Select", Gtk::RESPONSE_OK);	

	int result = dialog.run();
        std::vector<std::string> filenames;

	switch(result) {
                case(Gtk::RESPONSE_OK):
                        std::cout << "Open clicked." << std::endl;
                        filenames = dialog.get_filenames();

                        break;
                case(Gtk::RESPONSE_CANCEL):
                default:
                        return;
        }

        dialog.hide();

	for (std::vector<std::string>::iterator it = filenames.begin() ; it != filenames.end() ; ++it)  {
                std::cout << "Selected file: " << *it << std::endl;
		try {
			Chi chi(*it);
			Gtk::TreeModel::Row row = *(model->append());
			row[chi_files_columns.col_filename_full] = *it;
			row[chi_files_columns.col_filename_basename] = Glib::path_get_basename(*it);
			row[chi_files_columns.col_filename_dirname] = Glib::path_get_dirname(*it);
			row[chi_files_columns.col_chi] = chi;
		}
		catch (std::exception &e) {
                	std::cerr << "Exception caught: " << e.what() << std::endl;
			Gtk::MessageDialog mdialog(*this, "Error reading in "+*it, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                        mdialog.set_secondary_text(Glib::ustring("I/O error: ")+e.what());
                        mdialog.run();
                        continue;
        	}
		catch (...) {
			Gtk::MessageDialog mdialog(*this, "Error reading in "+*it, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                        mdialog.set_secondary_text("Unknown exception type");
                        mdialog.run();
                        continue;
		}
	}
	//count children of model -> if greater than one, set the convert button sensitive
	if (model->children().size() > 0) {
		convert.set_sensitive();
	}
	return;
}

bool Chi2DatWindow::on_backspace_clicked(GdkEventKey *event) {
        if (event->keyval == gdk_keyval_from_name("BackSpace") ||
            event->keyval == gdk_keyval_from_name("Delete")) {
                Glib::RefPtr<Gtk::TreeSelection> selection = tv.get_selection();
                std::vector<Gtk::TreeModel::Path> paths = selection->get_selected_rows();
                for (std::vector<Gtk::TreeModel::Path>::reverse_iterator rit = paths.rbegin() ; rit != paths.rend() ; ++rit) {
                        Gtk::TreeModel::Row row = *(model->get_iter(*rit));
                        model->erase(row);
                }
                if (model->children().size() > 0) {
			convert.set_sensitive();
                }
                else {
			convert.set_sensitive(false);
                }
                return TRUE;
        }
        return FALSE;	
}
