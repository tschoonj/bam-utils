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

Chi2DatWindow::Chi2DatWindow() : convert("Convert!"),
																 RadioButton1("q → 2θ<sub>c</sub>"),
																 RadioButton2("2θ<sub>m</sub> → 2θ<sub>c</sub>"),
																 RadioButton3("2θ<sub>m</sub> → q"),
																 open("Load chi files") {
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
	tv.set_tooltip_column(0);
	sw.add(tv);
	sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	grid.attach(sw, 0, 0, 1, 10);
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

	//radiobuttons and friends
	Gtk::RadioButton::Group group = RadioButton1.get_group();
	RadioButton2.set_group(group);
	RadioButton3.set_group(group);
	dynamic_cast<Gtk::Label*>(RadioButton1.get_child())->set_use_markup();
	dynamic_cast<Gtk::Label*>(RadioButton2.get_child())->set_use_markup();
	dynamic_cast<Gtk::Label*>(RadioButton3.get_child())->set_use_markup();

	grid.attach(RadioButton1, 1, 3, 2, 1);
	button1_label1.set_markup("λ<sub>c</sub>");
	button1_label1.set_margin_start(10);
	grid.attach(button1_label1, 1, 4, 1, 1);
	button1_entry1.set_text("0.15405929");
	grid.attach(button1_entry1, 2, 4, 1, 1);


	grid.attach(RadioButton2, 1, 5, 2, 1);
	button2_label1.set_markup("λ<sub>c</sub>");
	button2_label1.set_margin_start(10);
	grid.attach(button2_label1, 1, 6, 1, 1);
	button2_entry1.set_text("0.15405929");
	grid.attach(button2_entry1, 2, 6, 1, 1);
	button2_label2.set_markup("λ<sub>m</sub>");
	button2_label2.set_margin_start(10);
	grid.attach(button2_label2, 1, 7, 1, 1);
	button2_entry2.set_text("1.0");
	grid.attach(button2_entry2, 2, 7, 1, 1);

	grid.attach(RadioButton3, 1, 8, 2, 1);
	button3_label1.set_markup("λ<sub>m</sub>");
	button3_label1.set_margin_start(10);
	grid.attach(button3_label1, 1, 9, 1, 1);
	button3_entry1.set_text("1.0");
	grid.attach(button3_entry1, 2, 9, 1, 1);

	RadioButton1.set_active();
	button2_entry1.set_sensitive(false);
	button2_label1.set_sensitive(false);
	button2_entry2.set_sensitive(false);
	button2_label2.set_sensitive(false);
	button3_entry1.set_sensitive(false);
	button3_label1.set_sensitive(false);

	RadioButton1.signal_toggled().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_radio_toggled));
	RadioButton2.signal_toggled().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_radio_toggled));
	RadioButton3.signal_toggled().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_radio_toggled));


	//open.set_image_from_icon_name("document-open");
        open.set_vexpand(false);
        open.set_hexpand(false);
        open.set_valign(Gtk::ALIGN_CENTER);
        open.set_halign(Gtk::ALIGN_CENTER);
	open.signal_clicked().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_open_clicked));
	grid.attach(open, 0, 10, 1, 1);

        convert.set_vexpand(false);
        convert.set_hexpand(false);
        convert.set_valign(Gtk::ALIGN_CENTER);
        convert.set_halign(Gtk::ALIGN_CENTER);
	convert.set_sensitive(false);
	convert.signal_clicked().connect(sigc::mem_fun(*this, &Chi2DatWindow::on_convert_clicked));
	grid.attach(convert, 1, 10, 2, 1);


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
		Gtk::MessageDialog mdialog(*this, Glib::ustring("Start value: "), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
		mdialog.set_secondary_text(Glib::ustring(e.what()));
                mdialog.run();
		return;
	}
	try {
		step_size_d = get_value_from_entry<double>(step_size);
		std::cout << "step_size_d: " << step_size_d << std::endl;
	}
	catch (std::exception &e) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing Step size: "), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
		mdialog.set_secondary_text(Glib::ustring(e.what()));
                mdialog.run();
		return;
	}
	try {
		nsteps_i = get_value_from_entry<int>(nsteps);
		std::cout << "nsteps_i: " << nsteps_i << std::endl;
	}
	catch (std::exception &e) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing #steps: "), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
		mdialog.set_secondary_text(Glib::ustring(e.what()));
                mdialog.run();
		return;
	}

	double lambda_c, lambda_m;

	if (RadioButton1.get_active()) {
		try {
			lambda_c = get_value_from_entry<double>(button1_entry1);
		}
		catch (std::exception &e) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing λ<sub>c</sub>"), true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
			mdialog.set_secondary_text(Glib::ustring(e.what()));
                	mdialog.run();
			return;
		}
	}
	else if (RadioButton2.get_active()) {
		try {
			lambda_c = get_value_from_entry<double>(button2_entry1);
		}
		catch (std::exception &e) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing λ<sub>c</sub>"), true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
			mdialog.set_secondary_text(Glib::ustring(e.what()));
                	mdialog.run();
			return;
		}
		try {
			lambda_m = get_value_from_entry<double>(button2_entry2);
		}
		catch (std::exception &e) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing λ<sub>m</sub>"), true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
			mdialog.set_secondary_text(Glib::ustring(e.what()));
                	mdialog.run();
			return;
		}
	}
	else if (RadioButton3.get_active()) {
		try {
			lambda_m = get_value_from_entry<double>(button3_entry1);
		}
		catch (std::exception &e) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Error processing λ<sub>m</sub>"), true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
			mdialog.set_secondary_text(Glib::ustring(e.what()));
                	mdialog.run();
			return;
		}
	}


	Gtk::TreeModel::Children kids = model->children();
	int converted = 0;
	int failed = 0;
        for (Gtk::TreeModel::Children::iterator iter = kids.begin() ; iter != kids.end() ; ++iter) {
                Gtk::TreeModel::Row row = *iter;
		Chi chi = row[chi_files_columns.col_chi];
		double *x_values_orig = chi.GetXPtr();
		double *y_values = chi.GetYPtr();
		std::vector<double> x_values(chi.GetSize());
		gsl_interp_accel *acc = gsl_interp_accel_alloc ();
    		gsl_interp *interp = gsl_interp_alloc (gsl_interp_cspline, chi.GetSize());
		for (int i = 0 ; i < chi.GetSize() ; i++) {
			if (RadioButton1.get_active()) {
				x_values[i] = RAD2DEG(2.0*asin(x_values_orig[i]*lambda_c/(4.0*M_PI)));
			}
			else if (RadioButton2.get_active()) {
				x_values[i] = RAD2DEG(2.0*asin((lambda_c/lambda_m)*sin(DEG2RAD(x_values_orig[i])/2.0)));
			}
			else if (RadioButton3.get_active()) {
				x_values[i] = 4*M_PI*sin(DEG2RAD(x_values_orig[i])/2.0)/lambda_m;
			}
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
			failed++;
			continue;
		}

		int lines_printed = 0;

    		for (int i = 0 ; i <= nsteps_i ; i++) {
			double x = start_value_d + i * step_size_d;
			double y;
			int e = gsl_interp_eval_e(interp, &x_values[0], chi.GetYPtr(), x, acc, &y);
			if (e == GSL_EDOM) {
				lines_printed++;
				continue;
			}
			//std::cout << "x: " << x << " y: " << y << std::endl;
			fs << x << "\t" << y << std::endl;
		}

		fs.close();
		gsl_interp_free (interp);
		gsl_interp_accel_free (acc);

		if (lines_printed == 0) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("No lines were printed to ")+datfile+Glib::ustring(" due to interpolation errros. Check the input values."), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
                	mdialog.run();
			failed++;
			continue;
		}


		converted++;
        }

	if (failed == 0) {
		Gtk::MessageDialog mdialog(*this, Glib::ustring("All files have been successfully converted"), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_CLOSE, true);
               	mdialog.run();
	}
	else if (converted > 0) {
		//some files were converted
		std::ostringstream ss;
		ss << converted << " files were converted while " << failed << "files could not be converted";
		Gtk::MessageDialog mdialog(*this, Glib::ustring(ss.str()), false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE, true);
               	mdialog.run();

	}
	else {
		// no files were converted
		Gtk::MessageDialog mdialog(*this, Glib::ustring("No files have been converted"), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
               	mdialog.run();
	}
	return;
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

void Chi2DatWindow::on_radio_toggled() {
	if (RadioButton1.get_active()) {
		button1_entry1.set_sensitive();
		button1_label1.set_sensitive();
		button2_entry1.set_sensitive(false);
		button2_label1.set_sensitive(false);
		button2_entry2.set_sensitive(false);
		button2_label2.set_sensitive(false);
		button3_entry1.set_sensitive(false);
		button3_label1.set_sensitive(false);
	}
	else if (RadioButton2.get_active()) {
		button1_entry1.set_sensitive(false);
		button1_label1.set_sensitive(false);
		button2_entry1.set_sensitive();
		button2_label1.set_sensitive();
		button2_entry2.set_sensitive();
		button2_label2.set_sensitive();
		button3_entry1.set_sensitive(false);
		button3_label1.set_sensitive(false);
	}
	else if (RadioButton3.get_active()) {
		button1_entry1.set_sensitive(false);
		button1_label1.set_sensitive(false);
		button2_entry1.set_sensitive(false);
		button2_label1.set_sensitive(false);
		button2_entry2.set_sensitive(false);
		button2_label2.set_sensitive(false);
		button3_entry1.set_sensitive();
		button3_label1.set_sensitive();
	}
}
