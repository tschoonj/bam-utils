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
			add(col_element);
			add(col_shell);
			add(col_atomic_number);
			add(col_shell_index);
		}
		Gtk::TreeModelColumn<Glib::ustring> col_filename_full;
		Gtk::TreeModelColumn<Glib::ustring> col_filename_basename;
		Gtk::TreeModelColumn<Glib::ustring> col_filename_dirname;
		Gtk::TreeModelColumn<BAM::ASC *>    col_asc;
		Gtk::TreeModelColumn<Glib::ustring> col_element;
		Gtk::TreeModelColumn<Glib::ustring> col_shell;
		Gtk::TreeModelColumn<int> col_atomic_number;
		Gtk::TreeModelColumn<int> col_shell_index;
	};
	AscFilesColumns asc_files_columns;

	class AscFilesElementColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		AscFilesElementColumns() {
			add(col_element);
			add(col_atomic_number);
		}
		Gtk::TreeModelColumn<Glib::ustring> col_element;
		Gtk::TreeModelColumn<int> col_atomic_number;
	};
	AscFilesElementColumns asc_files_element_columns;

	class AscFilesShellColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		AscFilesShellColumns() {
			add(col_shell);
			add(col_shell_index);
		}
		Gtk::TreeModelColumn<Glib::ustring> col_shell;
		Gtk::TreeModelColumn<int> col_shell_index;
	};
	AscFilesShellColumns asc_files_shell_columns;

	Gtk::TreeView tv;
	Glib::RefPtr<Gtk::ListStore> model, shell_model, element_model;
	Gtk::ScrolledWindow sw;


	bool on_delete_event(GdkEventAny* event);
	void on_open_clicked();
	//void on_save_clicked();
	bool on_backspace_clicked(GdkEventKey *key);
	void on_asc_file_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_shell_changed(const Glib::ustring& path, const Gtk::TreeModel::iterator& iter);
	void on_element_changed(const Glib::ustring& path, const Gtk::TreeModel::iterator& iter);
	void check_save_status();
};



#endif
