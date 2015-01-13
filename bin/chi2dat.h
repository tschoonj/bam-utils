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
#include <iomanip>
#include <iostream>





class Chi2DatWindow : public Gtk::Window {
	Chi2DatWindow();
	virtual ~Chi2DatWindow();

	Gtk::Grid grid;
	
	

}



#endif
