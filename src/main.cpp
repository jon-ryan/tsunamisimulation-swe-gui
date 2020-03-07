// includes
#include <iostream>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/notebook.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>

// include GUI class
#include "gui.h"

int main(int argc, char *argv[]) {
    // create the application
    auto app = Gtk::Application::create(argc, argv, "org.swe.gui");

    auto refBuilder = Gtk::Builder::create_from_file("user_interface.glade");
    // create the gui class
    Gui *gui = nullptr;
    refBuilder->get_widget_derived("main_window", gui);

    app->run(*gui);
  return 0;
}

