#include "bam_plplot_window.h"
#include <algorithm>

BAM::PlPlotWindow::PlPlotWindow(
  std::vector<double> &x_orig,
  std::vector<double> &y_orig,
  std::string title,
  std::string x_axis,
  std::string y_axis) :
  BAM::PlPlotWindow::PlPlotWindow(
    std::vector<Gtk::PLplot::PlotData2D>{Gtk::PLplot::PlotData2D(x_orig, y_orig)},
    title, x_axis, y_axis) {
}

BAM::PlPlotWindow::PlPlotWindow(
  const std::vector<Gtk::PLplot::PlotData2D> &data,
  std::string title,
  std::string x_axis,
  std::string y_axis,
  const std::vector<std::string> &_notebook_labels) :
  notebook_labels(_notebook_labels) {

  for (unsigned int i = 0 ; i < data.size() ; i++) {
    drawingareas.push_back(
      Gtk::manage(
        new Gtk::PLplot::Canvas(
          Gtk::PLplot::Plot2D(data[i], x_axis, y_axis, title)
        )
      )
    );
  }
  create();
}

void BAM::PlPlotWindow::create() {
  set_title("Preview");
  if (drawingareas.size() == 1) {
    set_default_size(720, 540);
    add(*drawingareas[0]);
    drawingareas[0]->show();
  }
  else {
    for (unsigned int i = 0 ; i < drawingareas.size() ; i++) {
      notebook.append_page(*drawingareas[i], notebook_labels[i]);
      drawingareas[i]->show();
    }
    set_default_size(720, 580);
    add(notebook);
    notebook.show();
  }
}
