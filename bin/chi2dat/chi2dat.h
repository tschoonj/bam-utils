#ifndef CHI2DATWINDOW_H
#define CHI2DATWINDOW_H

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





class Chi2DatWindow : public Gtk::Window {
public:
	Chi2DatWindow();
	virtual ~Chi2DatWindow();

	Gtk::Grid grid;
	Gtk::Button convert;
	class ChiFilesColumns : public Gtk::TreeModel::ColumnRecord {
		public:
			ChiFilesColumns() {
				add(col_filename_full);
				add(col_filename_basename);
				add(col_filename_dirname);
				add(col_chi);
			}
			Gtk::TreeModelColumn<Glib::ustring> col_filename_full;
			Gtk::TreeModelColumn<Glib::ustring> col_filename_basename;
			Gtk::TreeModelColumn<Glib::ustring> col_filename_dirname;
			Gtk::TreeModelColumn<Chi> col_chi;
	};
	ChiFilesColumns chi_files_columns;
	Gtk::Button open;
	Gtk::TreeView tv;
	Glib::RefPtr<Gtk::ListStore> model;
	Gtk::ScrolledWindow sw;

	Gtk::Entry start_value;
	Gtk::Entry step_size;
	Gtk::Entry nsteps;

	Gtk::RadioButton RadioButton1, RadioButton2, RadioButton3;	
	Gtk::Entry button1_entry1;
	Gtk::Entry button2_entry1;
	Gtk::Entry button2_entry2;
	Gtk::Entry button3_entry1;
	Gtk::Label button1_label1;
	Gtk::Label button2_label1;
	Gtk::Label button2_label2;
	Gtk::Label button3_label1;
	
	bool on_delete_event(GdkEventAny* event);	
	void on_open_clicked();
	void on_convert_clicked();
	bool on_backspace_clicked(GdkEventKey *key);
	double get_double_from_entry(Gtk::Entry entry);
	template<typename T> T get_value_from_entry(Gtk::Entry &entry);
	void on_radio_toggled();
};



#endif
