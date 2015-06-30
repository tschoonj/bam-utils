#ifndef ASC2ATHENAWINDOW_H
#define ASC2ATHENAWINDOW_H

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
#include "bam_file_asc.h"





class Asc2AthenaWindow : public Gtk::Window {
public:
	Asc2AthenaWindow();
	virtual ~Asc2AthenaWindow();

	Gtk::Grid grid;
	Gtk::Button saveas;
	Gtk::Button open;
	class AscFilesColumns : public Gtk::TreeModel::ColumnRecord {
		public:
			AscFilesColumns() {
				add(col_filename_full);
				add(col_filename_basename);
				add(col_filename_dirname);
				add(col_asc);
			}
			Gtk::TreeModelColumn<Glib::ustring> col_filename_full;
			Gtk::TreeModelColumn<Glib::ustring> col_filename_basename;
			Gtk::TreeModelColumn<Glib::ustring> col_filename_dirname;
			Gtk::TreeModelColumn<BAM::ASC *>    col_asc;
	};
	AscFilesColumns asc_files_columns;
	Gtk::TreeView tv;
	Glib::RefPtr<Gtk::ListStore> model;
	Gtk::ScrolledWindow sw;


	bool on_delete_event(GdkEventAny* event);
	void on_open_clicked();
	//void on_save_clicked();
	bool on_backspace_clicked(GdkEventKey *key);
	void on_asc_file_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
};



#endif
