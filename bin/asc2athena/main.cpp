#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "asc2athena.h"



int main(int argc, char **argv) {
	Glib::set_application_name("asc2athena");
#if defined(G_OS_WIN32)
  setlocale(LC_ALL,"English_United States");
	//set PLPLOT_LIB
	gchar *installation_dir = g_win32_get_package_installation_directory_of_module(NULL);
	std::string path_to_plplot(Glib::build_filename(installation_dir, "Share", "plplot"));
	Glib::setenv("PLPLOT_LIB", path_to_plplot, true);
	g_free(installation_dir);
#else
  Glib::setenv("LANG","en_US", true);
#endif
  gtk_disable_setlocale();
  setbuf(stdout,NULL);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "de.bam.asc2athena");

	Asc2AthenaWindow window;

	return app->run(window);
}
