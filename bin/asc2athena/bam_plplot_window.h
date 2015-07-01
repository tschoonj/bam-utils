#ifndef BAM_PLPLOT_WINDOW_H
#define BAM_PLPLOT_WINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/notebook.h>
#include <plstream.h>
#include <vector>

namespace BAM {
  class PlPlotDrawingAreaData {
  private:
    std::vector<PLFLT> x;
    std::vector<PLFLT> y;
    std::string title;
    std::string x_axis;
    std::string y_axis;
    std::string notebook_label;
  public:
    PlPlotDrawingAreaData(std::vector<double> &x_orig,
                          std::vector<double> &y_orig,
                          std::string title = "title",
                          std::string x_axis = "x",
                          std::string y_axis = "y",
                          std::string notebook_label = "label") :
                          x(x_orig),
                          y(y_orig),
                          title(title),
                          x_axis(x_axis),
                          y_axis(y_axis),
                          notebook_label(notebook_label) {}
    virtual ~PlPlotDrawingAreaData() {}
    friend class PlPlotDrawingArea;
    friend class PlPlotWindow;
  };
  class PlPlotDrawingArea : public Gtk::DrawingArea {
  private:
    plstream pls;
    PlPlotDrawingAreaData data;
  protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  public:
    PlPlotDrawingArea(const PlPlotDrawingAreaData &data);
    virtual ~PlPlotDrawingArea() {}
    friend class PlPlotWindow;
  };
  class PlPlotWindow : public Gtk::Window {
  private:
    std::vector<PlPlotDrawingArea *> drawingareas;
    Gtk::Notebook notebook;
    void create();
  public:
    PlPlotWindow(std::vector<double> &x_orig,
                 std::vector<double> &y_orig,
                 std::string title = "title",
                 std::string x_axis = "x",
                 std::string y_axis = "y");
    PlPlotWindow(std::vector<PlPlotDrawingAreaData> &data);
    ~PlPlotWindow() {}
  };


}
#endif
