#include "asc2athena.h"
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

Asc2AthenaWindow::Asc2AthenaWindow() : saveas("Save"),
																       open("Load asc files") {
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
	model = Gtk::ListStore::create(asc_files_columns);
	tv.set_model(model);
	tv.append_column("Filename", asc_files_columns.col_filename_basename);
	tv.set_tooltip_column(0);
  tv.signal_row_activated().connect(sigc::mem_fun(*this, &Asc2AthenaWindow::on_asc_file_activated));
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
	open.signal_clicked().connect(sigc::mem_fun(*this, &Asc2AthenaWindow::on_open_clicked));
	grid.attach(open, 0, 1, 1, 1);

	//saveas.set_image_from_icon_name("document-save");
  saveas.set_vexpand(false);
  saveas.set_hexpand(false);
  saveas.set_valign(Gtk::ALIGN_CENTER);
  saveas.set_halign(Gtk::ALIGN_CENTER);
	saveas.set_sensitive(false);
	//saveas.signal_clicked().connect(sigc::mem_fun(*this, &Asc2AthenaWindow::on_convert_clicked));
	grid.attach(saveas, 1, 1, 1, 1);


	signal_delete_event().connect(sigc::mem_fun(*this, &Asc2AthenaWindow::on_delete_event));
	tv.signal_key_press_event().connect(sigc::mem_fun(*this, &Asc2AthenaWindow::on_backspace_clicked));
	show_all_children();
}

Asc2AthenaWindow::~Asc2AthenaWindow() {}

bool Asc2AthenaWindow::on_delete_event(GdkEventAny* event) {
        //std::cout << "Window delete event" << std::endl;
        get_application()->remove_window(*this);
        return true;
}

void Asc2AthenaWindow::on_open_clicked() {
	Gtk::FileChooserDialog dialog(*this, "Please select a number of ASC files", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_select_multiple();
        Glib::RefPtr<Gtk::FileFilter> filter_asc= Gtk::FileFilter::create();
        filter_asc->set_name("ASC files");
        filter_asc->add_pattern("*.asc");
        filter_asc->add_pattern("*.ASC");
        dialog.add_filter(filter_asc);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();
  std::vector<std::string> filenames;

	switch(result) {
    case(Gtk::RESPONSE_OK):
      //std::cout << "Open clicked." << std::endl;
      filenames = dialog.get_filenames();
      break;
    case(Gtk::RESPONSE_CANCEL):
    default:
      return;
  }

  dialog.hide();

	for (std::vector<std::string>::iterator it = filenames.begin() ; it != filenames.end() ; ++it)  {
    //std::cout << "Selected file: " << *it << std::endl;
		try {
			BAM::ASC *asc = new BAM::ASC(*it);
			Gtk::TreeModel::Row row = *(model->append());
			row[asc_files_columns.col_filename_full] = *it;
			row[asc_files_columns.col_filename_basename] = Glib::path_get_basename(*it);
			row[asc_files_columns.col_filename_dirname] = Glib::path_get_dirname(*it);
			row[asc_files_columns.col_asc] = asc;
		}
		catch (BAM::Exception &e) {
      std::cerr << "BAM Exception caught: " << e.what() << std::endl;
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
	//count children of model -> if greater than one, set the saveas button sensitive
	if (model->children().size() > 0) {
		saveas.set_sensitive();
	}
	return;
}

bool Asc2AthenaWindow::on_backspace_clicked(GdkEventKey *event) {
  if (event->keyval == gdk_keyval_from_name("BackSpace") ||
      event->keyval == gdk_keyval_from_name("Delete")) {
    Glib::RefPtr<Gtk::TreeSelection> selection = tv.get_selection();
    std::vector<Gtk::TreeModel::Path> paths = selection->get_selected_rows();
    for (std::vector<Gtk::TreeModel::Path>::reverse_iterator rit = paths.rbegin() ; rit != paths.rend() ; ++rit) {
      Gtk::TreeModel::Row row = *(model->get_iter(*rit));
      BAM::ASC *asc = row[asc_files_columns.col_asc];
      delete asc;
      model->erase(row);
    }
    if (model->children().size() > 0) {
		  saveas.set_sensitive();
    }
    else {
			saveas.set_sensitive(false);
    }
    return true;
  }
  return false;
}

void Asc2AthenaWindow::on_asc_file_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {

  //file was double-clicked -> open plot window
  //first determine which asc file was activated
	Gtk::TreeModel::Row row = *(model->get_iter(path));
  std::cout << "File double-clicked: " << row[asc_files_columns.col_filename_full] << std::endl;
  BAM::ASC *asc = row[asc_files_columns.col_asc];
  asc->plot(*this);

}
