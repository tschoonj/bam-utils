#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "chi2pdh.h"



int main(int argc, char **argv) {
	Glib::set_application_name("chi2pdh");
#if defined(G_OS_WIN32)
        setlocale(LC_ALL,"English_United States");
        //g_setenv("LANG","en_US",TRUE);
#else
        Glib::setenv("LANG","en_US",TRUE);
#endif
        gtk_disable_setlocale();
        setbuf(stdout,NULL);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "de.bam.chi2pdh");

	Chi2PdhWindow window;

	return app->run(window);
}
