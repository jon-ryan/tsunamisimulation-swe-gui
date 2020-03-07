#ifndef SWE_GUI_GUI_H
#define SWE_GUI_GUI_H

#include <gtkmm/window.h>
#include <gtkmm/stack.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/application.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/main.h>
#include <gtkmm/revealer.h>
#include <ctime>
#include <thread>
#include <mutex>
#include <giomm.h>
#include <unistd.h>

#include "scenario.h"

class scenario;


/**
 * Defines the GUI class
 * @class Gui
 */
class Gui : public Gtk::Window {
protected:
  // member variables

  // file system variables
  // directory
  std::string directory = "";
  // file name
  std::string filename = "";

  // set up new parameters
  // scenario to load
  int scenario_id = 0; // 0 = from file, 1 = Radial Dam break, 2 = artificial scenario
  // boundary condition
  bool uses_walls = false;
  // generate output
  bool generate_output = true;

  // variables used during simulation
  // progress
  float progress = 0;
  // global time remaining
  int time_remaining = 0;

  // time start
  time_t time_start;
  // time end
  time_t time_end;
  // diff time in seconds
  int diff_seconds;

  Glib::Dispatcher dispatcher_progressbar_update;
  Glib::Dispatcher dispatcher_show_result_page;
  Glib::Dispatcher dispatcher_set_result_duration;

  std::mutex progress_bat_mtx;

  std::string worker_message;
  double worker_target_fraction;
  double worker_actual_fraction;
  int worker_time_remaining;

  double simulation_fraction;
  int simulation_time_remaining;

  double main_thread_fraction;
  std::string main_thread_message;

  std::mutex result_duration_mtx;
  std::string result_duration_string;


  // scenario reference
  scenario *scen;

  // ui
  Glib::RefPtr<Gtk::Builder> ui;
  // main window
  Gtk::Window* main_window;
  // stack
  Gtk::Stack* gui_stack;


  //page 0 - welcome page
  Gtk::Button* page_0_button_start_new;
  Gtk::Button* page_0_button_load_checkpoint;
  Gtk::Button* page_0_button_how_to;
  Gtk::Button* page_0_button_about;
  Gtk::Button* page_0_button_quit;

  // page 1 - set up new page
  Gtk::SpinButton* page_1_spin_button_x_dim;
  Gtk::SpinButton* page_1_spin_button_y_dim;
  Gtk::SpinButton* page_1_spin_button_sim_time;
  Gtk::SpinButton* page_1_spin_button_num_time_steps;
  Gtk::SpinButton* page_1_spin_button_num_threads;
  Gtk::ComboBoxText* page_1_combobox_output;
  Gtk::SpinButton* page_1_spin_button_coarseness_factor;
  Gtk::ComboBoxText* page_1_combobox_boundary; // 0 = Outflow, 1 = Wall
  Gtk::ComboBoxText* page_1_combobox_scenario;
  Gtk::Revealer* page_1_revealer_scenario_input;
  Gtk::Revealer* page_1_revealer_result_output;
  Gtk::FileChooser* page_1_file_chooser_bathymetry;
  Gtk::FileChooser* page_1_file_chooser_displacement;
  Gtk::FileChooser* page_1_file_chooser_output_directory;
  Gtk::Entry* page_1_entry_output_file_name;

  Gtk::Label* page_1_label_error;
  Gtk::Button* page_1_button_submit;
  Gtk::Button* page_1_button_back;

  // page 2 - load checkpoint page
  Gtk::FileChooser* page_2_file_chooser_load_checkpoint;
  Gtk::Label* page_2_label_error;
  Gtk::Button* page_2_button_submit;
  Gtk::Button* page_2_button_back;
  Gtk::FileChooser* page_2_file_chooser_load_output_file;
  Gtk::SpinButton* page_2_spin_button_num_threads;

  //page 3 - computing simulation
  Gtk::Label* page_3_label_time_remaining_dynamic;
  Gtk::ProgressBar* page_3_progress_bar_progress;
  Gtk::Button* page_3_button_cancel;

  // page 4 - result page
  Gtk::Button* page_4_button_set_up_new;
  Gtk::Button* page_4_button_quit;
  Gtk::Button* page_4_button_open_file_location;
  Gtk::Button* page_4_button_paraview;
  Gtk::Label* page_4_label_time_spent_dynamic;
  Gtk::Label* page_4_label_output_path;
  Gtk::Label* page_4_label_paraview_error;

  // page 5 - error page
  Gtk::Label* page_5_label_error_message;
  Gtk::Button* page_5_button_return_main_menu;
  Gtk::Button* page_5_button_quit;

  // page 6 - how to page
  Gtk::Button* page_6_button_back;

  // page 7 - about page
  Gtk::Button* page_7_button_back;


  // signal handlers
  // page independent

  void on_close_application();                    // close application
  void on_return_to_main_menu();                  // return to main menu

  void on_page_0_start_new_clicked();             // page 0 start new simulation
  void on_page_0_load_checkpoint_clicked();       // page 0 load checkpoint
  void on_page_0_how_to_clicked();                // page 0 how to
  void on_page_0_about_clicked();                 // page 0 about

  void on_page_1_reveal_result_output();        // page 1 reveal number of checkpoints child
  void on_page_1_reveal_scenario_input();         // page 1 reveal file chooser for scenario input
  void on_page_1_submit_button_clicked();         // page 1 submit selection

  void on_page_2_submit_button_clicked();         // page 2 submit selection

  void on_page_3_button_cancel_clicked();         // page 3 cancel button
  void page_3_animation_worker();             // page 3 animating progress bar
  void page_3_handler();                          // page 3 handler

  void on_page_4_open_location_button_clicked();  // page 4 open file directory button
  void on_page_4_open_paraview_button_clicked();  // page 4 open paraview button

  // Declare simulation's run() as friend to allow for callbacks
  //friend void simulation::run();

  void on_notification_from_animation_worker_thread();
  void on_notification_to_change_to_result_page();
  void on_notification_to_set_result_duration();


public:
  Gui(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

  virtual ~Gui();

  // update function
  void update_progress(float progress, int time_remaining);    // page 3 progress bar
  void show_error_page(std::string error);
  bool was_canceled();
  void notify_progress_bar_update();
  void update_widgets();
  void notify_page_change_to_result();
};


#endif //SWE_GUI_GUI_H
