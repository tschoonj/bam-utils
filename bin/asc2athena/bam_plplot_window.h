#ifndef BAM_PLPLOT_WINDOW_H
#define BAM_PLPLOT_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <plstream.h>
#include <vector>

namespace BAM {
  class PlPlotDrawingArea : public Gtk::DrawingArea {
  private:
    plstream pls;
    std::vector<PLFLT> x;
    std::vector<PLFLT> y;
    std::string title;
  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  public:
    PlPlotDrawingArea(std::vector<double> &x_orig,
                      std::vector<double> &y_orig,
                      std::string title = "title");
    virtual ~PlPlotDrawingArea() {}
  };
  class PlPlotWindow : public Gtk::Window {
  private:
    PlPlotDrawingArea drawingarea;
  public:
    PlPlotWindow(std::vector<double> &x_orig,
                 std::vector<double> &y_orig,
                 std::string title = "title");
    ~PlPlotWindow() {}
  };


}
#endif
