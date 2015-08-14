#ifndef BAM_PLPLOT_WINDOW_H
#define BAM_PLPLOT_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/notebook.h>
#include <gtkmm-plplot/gtkmm-plplot.h>
#include <vector>

namespace BAM {
  class PlPlotWindow : public Gtk::Window {
  private:
    std::vector<Gtk::PLplot::Canvas *> drawingareas;
    std::vector<std::string> notebook_labels;
    Gtk::Notebook notebook;
    void create();
  public:
    PlPlotWindow(std::vector<double> &x_orig,
                 std::vector<double> &y_orig,
                 std::string title = "title",
                 std::string x_axis = "x",
                 std::string y_axis = "y");
    PlPlotWindow(const std::vector<Gtk::PLplot::PlotData2D> &data,
                 std::string title = "title",
                 std::string x_axis = "x",
                 std::string y_axis = "y",
                 const std::vector<std::string> &notebook_label = std::vector<std::string>()
    );
    ~PlPlotWindow() {}
  };
}
#endif
