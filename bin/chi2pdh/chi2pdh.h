#ifndef CHI2PDHWINDOW_H
#define CHI2PDHWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/radiobutton.h>
#include <iomanip>
#include <iostream>
#include "chifile.h"
#include "bam_plplot_window.h"

class Chi2PdhWindow : public Gtk::Window {
public:
	Chi2PdhWindow();
	virtual ~Chi2PdhWindow();

	Gtk::Grid grid;
	Gtk::Button convert;
	class ChiFilesColumns : public Gtk::TreeModel::ColumnRecord {
		public:
			ChiFilesColumns() {
				add(col_filename_full);
				add(col_filename_basename);
				add(col_filename_dirname);
				add(col_chi);
				add(col_plplotwindow);
			}
			Gtk::TreeModelColumn<std::string> col_filename_full;
			Gtk::TreeModelColumn<std::string> col_filename_basename;
			Gtk::TreeModelColumn<std::string> col_filename_dirname;
			Gtk::TreeModelColumn<Chi> col_chi;
			Gtk::TreeModelColumn<BAM::PlPlotWindow *> col_plplotwindow;
	};
	ChiFilesColumns chi_files_columns;
	Gtk::Button open;
	Gtk::TreeView tv;
	Glib::RefPtr<Gtk::ListStore> model;
	Gtk::ScrolledWindow sw;

	bool on_delete_event(GdkEventAny* event);
	void on_open_clicked();
	void on_convert_clicked();
	bool on_backspace_clicked(GdkEventKey *key);
	void on_file_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
};



#endif
