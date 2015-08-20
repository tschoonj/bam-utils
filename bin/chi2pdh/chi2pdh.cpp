#include "chi2pdh.h"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <glibmm/miscutils.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <cmath>
#include <glibmm/convert.h>

Chi2PdhWindow::Chi2PdhWindow() : convert("Convert!"),
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
	grid.attach(sw, 0, 0, 2, 1);
	sw.set_vexpand();
	sw.set_hexpand();

	//open.set_image_from_icon_name("document-open");
  open.set_vexpand(false);
  open.set_hexpand(false);
  open.set_valign(Gtk::ALIGN_CENTER);
  open.set_halign(Gtk::ALIGN_CENTER);
	open.signal_clicked().connect(sigc::mem_fun(*this, &Chi2PdhWindow::on_open_clicked));
	grid.attach(open, 0, 1, 1, 1);

  convert.set_vexpand(false);
  convert.set_hexpand(false);
  convert.set_valign(Gtk::ALIGN_CENTER);
  convert.set_halign(Gtk::ALIGN_CENTER);
	convert.set_sensitive(false);
	convert.signal_clicked().connect(sigc::mem_fun(*this, &Chi2PdhWindow::on_convert_clicked));
	grid.attach(convert, 1, 1, 1, 1);

	signal_delete_event().connect(sigc::mem_fun(*this, &Chi2PdhWindow::on_delete_event));
	tv.signal_key_press_event().connect(sigc::mem_fun(*this, &Chi2PdhWindow::on_backspace_clicked));
	show_all_children();
}

Chi2PdhWindow::~Chi2PdhWindow() {}

bool Chi2PdhWindow::on_delete_event(GdkEventAny* event) {
        std::cout << "Window delete event" << std::endl;
        get_application()->remove_window(*this);
        return true;
}

void Chi2PdhWindow::on_convert_clicked() {
  std::cout << "Convert clicked" << std::endl;

	Gtk::TreeModel::Children kids = model->children();
	int converted = 0;
	int failed = 0;
  for (auto &iter : kids) {
    Gtk::TreeModel::Row row = *iter;
		Chi chi = row[chi_files_columns.col_chi];

		//try writing to file
		std::string pdhfile = Glib::locale_from_utf8(row[chi_files_columns.col_filename_full]);
		pdhfile.replace(pdhfile.end()-3, pdhfile.end(), "pdh");

		std::cout << "Filename: " << pdhfile << std::endl;

		std::ofstream fs(pdhfile.c_str());
		if (!fs.is_open()) {
			Gtk::MessageDialog mdialog(*this, Glib::ustring("Could not open ") + pdhfile + Glib::ustring(" for writing"), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
      mdialog.run();
			failed++;
			continue;
		}

		std::vector<double> x_vector(chi.GetXVector());
		std::vector<double> y_vector(chi.GetYVector());

		//look for first value in X that is not zero
		auto not_zero = std::find_if(y_vector.begin(), y_vector.end(), [](double x_val){return x_val != 0.0;});
		size_t diff = std::distance(y_vector.begin(), not_zero);
		int number_of_values = y_vector.size() - diff;

		//print junk
		fs << "\t TREA_Wasser" << std::endl;
		fs << std::endl;
		fs << "\t " << number_of_values;
		for (int i = 0 ; i < 7 ; i++)
			fs << "\t" << 0;
		fs << std::endl;
		fs << "0\t0\t0\t1\t0" << std::endl;
		fs << "0\t0\t0\t0\t0" << std::endl;

		for (int i = diff ; i < x_vector.size() ; i++)
			fs << x_vector[i] << " " << y_vector[i] << " " << sqrt(y_vector[i])/5.0 << std::endl;

		fs.close();

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

void Chi2PdhWindow::on_open_clicked() {
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

bool Chi2PdhWindow::on_backspace_clicked(GdkEventKey *event) {
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
