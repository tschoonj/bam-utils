#include "bam_plplot_window.h"
#include <algorithm>

BAM::PlPlotDrawingArea::PlPlotDrawingArea(
  std::vector<double> &x_orig,
  std::vector<double> &y_orig,
  std::string title) :
  x(x_orig),
  y(y_orig),
  title(title) {
  pls.sdev("extcairo");
  pls.init();
}
bool BAM::PlPlotDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  pls.cmd(PLESC_DEVINIT, cr->cobj());
  pls.env(*(x.begin()), *(x.end()-1),
          *std::min_element(y.begin(), y.end()),
          *std::max_element(y.begin(), y.end()),
          0, 0);
  pls.lab( "Energy (eV)", "Absorbance", title.c_str());
  pls.line(x.size(), &x[0], &y[0]);

  return true;
}

BAM::PlPlotWindow::PlPlotWindow(std::vector<double> &x_orig,
                                std::vector<double> &y_orig,
                                std::string title) :
                                drawingarea(x_orig, y_orig, title)
                                {
  set_default_size(720, 540);
  add(drawingarea);
  set_title("Preview");
  drawingarea.show();
}
