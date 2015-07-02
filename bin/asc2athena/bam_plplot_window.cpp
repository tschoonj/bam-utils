#include "bam_plplot_window.h"
#include <algorithm>

BAM::PlPlotDrawingArea::PlPlotDrawingArea(const PlPlotDrawingAreaData &data) :
  data(data) {
  pls.sdev("extcairo");
  pls.init();
}
bool BAM::PlPlotDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  pls.cmd(PLESC_DEVINIT, cr->cobj());
  pls.env(*(data.x.begin()), *(data.x.end()-1),
          *std::min_element(data.y.begin(), data.y.end()),
          *std::max_element(data.y.begin(), data.y.end()),
          0, 0);
  pls.lab(data.x_axis.c_str(), data.y_axis.c_str(), data.title.c_str());
  pls.line(data.x.size(), &data.x[0], &data.y[0]);

  return true;
}

BAM::PlPlotWindow::PlPlotWindow(std::vector<double> &x_orig,
                                std::vector<double> &y_orig,
                                std::string title,
                                std::string x_axis,
                                std::string y_axis)
                                {
  drawingareas.push_back(Gtk::manage(new BAM::PlPlotDrawingArea(BAM::PlPlotDrawingAreaData(x_orig,
    y_orig, title, x_axis, y_axis))));
  create();
}

BAM::PlPlotWindow::PlPlotWindow(std::vector<BAM::PlPlotDrawingAreaData> &data) {
 for (unsigned int i = 0 ; i < data.size() ; i++)
  drawingareas.push_back(Gtk::manage(new BAM::PlPlotDrawingArea(data[i])));
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
      notebook.append_page(*drawingareas[i], drawingareas[i]->data.notebook_label);
      drawingareas[i]->show();
    }
    set_default_size(720, 580);
    add(notebook);
    notebook.show();
  }
}
