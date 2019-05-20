#include <gtkmm.h>
#include "include/gokturkgui.h"

using namespace Gtk;

int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv,"www.cyber-warrior.org");

    auto builder = Gtk::Builder::create_from_file("ui/gui.glade");

    GokturkGui *mainwnd = nullptr;

    builder->get_widget_derived("m_window1", mainwnd);
    auto result = app->run(*mainwnd);

    delete mainwnd;


    return result;
}
