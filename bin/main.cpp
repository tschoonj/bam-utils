#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "chi2dat.h"



int main(int argc, char **argv) {
	Glib::set_application_name("chi2dat");
#if defined(G_OS_WIN32)
        setlocale(LC_ALL,"English_United States");
        //g_setenv("LANG","en_US",TRUE);
#else
        Glib::setenv("LANG","en_US",TRUE);
#endif
        gtk_disable_setlocale();
        setbuf(stdout,NULL);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "de.bam.chi2dat");

	Chi2DatWindow window;

	return app->run(window);
}
