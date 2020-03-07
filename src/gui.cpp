#include "gui.h"
#include "simulation.h"

#include <iostream>
#include <string>
#include <gtkmm/cssprovider.h>
#include <array>
#include <unistd.h>


/**
 * Constructor of the GUI class
 * Gets references to specific UI elements and sets up signal handlers for buttons
 */
Gui::Gui(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder) :
    Gtk::Window(cobject),
    time_remaining(-1) {

    // get reference to builder
    this->ui = refBuilder;


    // lambda function for checking widgets
    auto const check_widget = [this](const std::string &widget_name, auto &widget_pointer) {
        ui->get_widget(widget_name,widget_pointer);
        if (!widget_pointer) std::cerr << "Unable to load widget '" << widget_name << "'" << std::endl;
    };


    // get the GtkBuilder-instantiated elements

    check_widget("main_window", main_window);                                                   // get main window

    check_widget("gui_stack", gui_stack);                                                       // get the gui stack

    check_widget("page_0_button_start_new", page_0_button_start_new);                           // get page 0 start new button
    check_widget("page_0_button_load_checkpoint", page_0_button_load_checkpoint);               // get page 0 load checkpoint button
    check_widget("page_0_button_how_to", page_0_button_how_to);                                 // get page 0 how to button
    check_widget("page_0_button_about", page_0_button_about);                                   // get page 0 about button
    check_widget("page_0_button_quit", page_0_button_quit);                                     // get page 0 quit button

    check_widget("page_1_spin_button_x_dim", page_1_spin_button_x_dim);                         // get page 1 x dim
    check_widget("page_1_spin_button_y_dim", page_1_spin_button_y_dim);                         // get page 1 y dim
    check_widget("page_1_spin_button_sim_time", page_1_spin_button_sim_time);                   // get page 1 sim time
    check_widget("page_1_spin_button_num_time_steps", page_1_spin_button_num_time_steps);       // get page 1 num timesteps
    check_widget("page_1_spin_button_num_threads", page_1_spin_button_num_threads);             // get page 1 num threads
    check_widget("page_1_combobox_output", page_1_combobox_output);                             // get page 1 output combobox
    check_widget("page_1_spin_button_coarseness_factor", page_1_spin_button_coarseness_factor); // get page 1 coarseness factor
    check_widget("page_1_combobox_boundary", page_1_combobox_boundary);                         // get page 1 combo box for boundary selection
    check_widget("page_1_combobox_scenario", page_1_combobox_scenario);                         // get page 1 combobox for scenario selectoin
    check_widget("page_1_revealer_scenario_input", page_1_revealer_scenario_input);             // get page 1 revealer for scenario input
    check_widget("page_1_revealer_result_output", page_1_revealer_result_output);               // get page 1 revealer for
    check_widget("page_1_file_chooser_bathymetry", page_1_file_chooser_bathymetry);             // get page 1 bathymetry filechooser
    check_widget("page_1_file_chooser_displacement", page_1_file_chooser_displacement);         // get page 1 displacement filechooser
    check_widget("page_1_file_chooser_output_directory", page_1_file_chooser_output_directory); // get page 1 output directory filechooser
    check_widget("page_1_entry_output_file_name", page_1_entry_output_file_name);               // get page 1 output file name
    check_widget("page_1_label_error", page_1_label_error);                                     // get page 1 error label
    check_widget("page_1_button_submit", page_1_button_submit);                                 // get page 1 submit button
    check_widget("page_1_button_back", page_1_button_back);                                     // get page 1 back button

    check_widget("page_2_file_chooser_load_checkpoint", page_2_file_chooser_load_checkpoint);   // get page 2 select checkpoint file
    check_widget("page_2_label_error", page_2_label_error);                                     // get page 2 error label
    check_widget("page_2_button_submit", page_2_button_submit);                                 // get page 2 submit button
    check_widget("page_2_button_back", page_2_button_back);                                     // get page 2 back button
    check_widget("page_2_file_chooser_load_output_file", page_2_file_chooser_load_output_file); // get page 2 select output file
    check_widget("page_2_spin_button_num_threads", page_2_spin_button_num_threads);             // get page 2 spin button num threads

    check_widget("page_3_progress_bar_progress", page_3_progress_bar_progress);                 // get page 3 progress bar
    check_widget("page_3_button_cancel", page_3_button_cancel);                                 // get page 3 test button
    check_widget("page_3_label_time_remaining_dynamic", page_3_label_time_remaining_dynamic);   // get page 3 time remaining label

    check_widget("page_4_button_open_file_location", page_4_button_open_file_location);         // get page 4 open file location button
    check_widget("page_4_button_quit", page_4_button_quit);                                     // get page 4 page_4_button_quit
    check_widget("page_4_button_set_up_new", page_4_button_set_up_new);                         // get page 4 page_4_button_set_up_new
    check_widget("page_4_label_time_spent_dynamic", page_4_label_time_spent_dynamic);           // get page_4_label_time_spent_dynamic
    check_widget("page_4_label_output_path", page_4_label_output_path);                         // get page_4_label_output_path
    check_widget("page_4_label_paraview_error", page_4_label_paraview_error);                   // get page 4 label paraview error
    check_widget("page_4_button_paraview", page_4_button_paraview);                             // get page 4 button open paraview

    check_widget("page_5_button_return_main_menu", page_5_button_return_main_menu);             // get page 5 return to menu button
    check_widget("page_5_button_quit", page_5_button_quit);                                     // get page 5 quit button
    check_widget("page_5_label_error_message", page_5_label_error_message);                     // get page 5 label

    check_widget("page_6_button_back", page_6_button_back);                                     // get page 6 back button

    check_widget("page_7_button_back", page_7_button_back);                                     // get page 7 back button

    // connect signals
    page_0_button_start_new->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_0_start_new_clicked));                     // connect page 0 start new button
    page_0_button_load_checkpoint->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_0_load_checkpoint_clicked));         // connect page 0 load checkpoint button
    page_0_button_how_to->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_0_how_to_clicked));                           // connect page 0 how to button
    page_0_button_about->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_0_about_clicked));                             // connect page 0 about
    page_0_button_quit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_close_application));                                 // connect page 0 quit button
    page_1_combobox_scenario->signal_changed().connect(sigc::mem_fun(*this, &Gui::on_page_1_reveal_scenario_input));                // connect page 1 scenario combobox
    page_1_combobox_output->signal_changed().connect(sigc::mem_fun(*this, &Gui::on_page_1_reveal_result_output));                   // connect page 1 output combobox
    page_1_button_submit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_1_submit_button_clicked));                    // connect page 1 submit button
    page_1_button_back->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                               // connect page 1 back button
    page_2_button_submit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_2_submit_button_clicked));                    // connect page 2 submit button
    page_2_button_back->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                               // connect page 2 back button
    page_3_button_cancel->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_3_button_cancel_clicked));                    // connect page 3 test button
    page_4_button_open_file_location->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_4_open_location_button_clicked)); // connect page 4 open file dir button
    page_4_button_set_up_new->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                         // connect page 4 return to main menu
    page_4_button_paraview->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_page_4_open_paraview_button_clicked));           // connect page 4 open paraview
    page_4_button_quit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_close_application));                                 // connect page 4 quit
    page_5_button_return_main_menu->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                   // connect page 5 return to menu button
    page_5_button_quit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_close_application));                                 // connect page 5 quit button
    page_6_button_back->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                               // connect page 6 back button
    page_7_button_back->signal_clicked().connect(sigc::mem_fun(*this, &Gui::on_return_to_main_menu));                               // connect page 7 back button

    dispatcher_progressbar_update.connect(sigc::mem_fun(*this, &Gui::on_notification_from_animation_worker_thread));                // connect progressbar animation dispatcher
    dispatcher_show_result_page.connect(sigc::mem_fun(*this, &Gui::on_notification_to_change_to_result_page));                      // connect change to result page dispatcher
    dispatcher_set_result_duration.connect(sigc::mem_fun(*this, &Gui::on_notification_to_set_result_duration));                     // connect set result duration dispatcher

    // show the gui
    show_all();

    // make page 1 / 2 error label invisible
    page_1_label_error->set_visible(false);
    page_2_label_error->set_visible(false);
    page_4_label_paraview_error->set_visible(false);

    // checkpoints not implemented yet
    // disable checkpoint submit button
    page_2_button_submit->set_sensitive(false);
}

/**
 * Destructor of the GUI class
 * Deletes all pointers to the UI elements
 */
Gui::~Gui () {

    // page 0
    delete page_0_button_load_checkpoint;
    delete page_0_button_start_new;
    delete page_0_button_how_to;
    delete page_0_button_about;
    delete page_0_button_quit;

    // page 1
    delete page_1_spin_button_x_dim;
    delete page_1_spin_button_y_dim;
    delete page_1_spin_button_sim_time;
    delete page_1_spin_button_num_time_steps;
    delete page_1_spin_button_num_threads;
    delete page_1_combobox_output;
    delete page_1_spin_button_coarseness_factor;
    delete page_1_combobox_boundary;
    delete page_1_combobox_scenario;
    delete page_1_revealer_scenario_input;
    delete page_1_revealer_result_output;
    delete page_1_file_chooser_bathymetry;
    delete page_1_file_chooser_displacement;
    delete page_1_file_chooser_output_directory;
    delete page_1_entry_output_file_name;
    delete page_1_label_error;
    delete page_1_button_submit;
    delete page_1_button_back;

    // page 2
    delete page_2_button_submit;
    delete page_2_file_chooser_load_checkpoint;
    delete page_2_label_error;
    delete page_2_button_back;
    delete page_2_file_chooser_load_output_file;
    delete page_2_spin_button_num_threads;

    // page 3
    delete page_3_progress_bar_progress;
    delete page_3_button_cancel;
    delete page_3_label_time_remaining_dynamic;

    // page 4
    delete page_4_label_output_path;
    delete page_4_label_time_spent_dynamic;
    delete page_4_button_open_file_location;
    delete page_4_button_quit;
    delete page_4_button_set_up_new;
    delete page_4_label_paraview_error;
    delete page_4_button_paraview;

    // page 5
    delete page_5_label_error_message;
    delete page_5_button_return_main_menu;
    delete page_5_button_quit;

    // top level widgets
    delete gui_stack;
    delete main_window;
}

/**
 * Page independent method to close the application
 */
void Gui::on_close_application() {
    main_window->close();
}

/**
 * Page independent method to return to the main menu
 */
void Gui::on_return_to_main_menu() {

    // reset page 1 values to null
    page_1_spin_button_x_dim->set_value(0);
    page_1_spin_button_y_dim->set_value(0);
    page_1_spin_button_sim_time->set_value(0);
    page_1_spin_button_num_threads->set_value(0);
    page_1_spin_button_num_time_steps->set_value(0);
    page_1_combobox_output->set_active(0);
    page_1_combobox_scenario->set_active(0);
    page_1_spin_button_coarseness_factor->set_value(1);
    page_1_file_chooser_bathymetry->unselect_all();
    page_1_file_chooser_displacement->unselect_all();
    page_1_file_chooser_output_directory->unselect_all();
    page_1_entry_output_file_name->set_text("");

    // rest page 2 values to null
    page_2_file_chooser_load_checkpoint->unselect_all();
    page_2_file_chooser_load_output_file->unselect_all();
    page_2_spin_button_num_threads->set_value(0);

    page_1_label_error->set_visible(false);
    page_2_label_error->set_visible(false);
    gui_stack->set_visible_child("page_welcome");

    // reset page 4
    page_4_label_paraview_error->set_visible(false);
}

/**
 * Button is located on the first page
 * Method is called when the user chooses to start a new simulation
 */
void Gui::on_page_0_start_new_clicked() {
    gui_stack->set_visible_child("page_set_up_new");
}

/**
 * Button is located on the first page
 * Method is called when the user selects the option to load a checkpoint
 */
void Gui::on_page_0_load_checkpoint_clicked() {
    gui_stack->set_visible_child("page_load_checkpoint");
}

/**
 * Button is located on first page
 * Method is called when the user selects the button to view the "how-to" page
 */
void Gui::on_page_0_how_to_clicked() {
    gui_stack->set_visible_child("page_how_to");
}

/**
 * Button is located on first page
 * method is called when the user selects the button to view the "about" page
 */
void Gui::on_page_0_about_clicked() {
    gui_stack->set_visible_child("page_about");
}

/**
 * Reveals the 'number of checkpoints' entry option, if the output type 'save as file' ist chosen
 */
void Gui::on_page_1_reveal_result_output() {
    page_1_revealer_result_output->set_reveal_child(page_1_combobox_output->get_active_id() == "0");
}

/**
 * Reveals the file chooser for scenario setup from files
 */
void Gui::on_page_1_reveal_scenario_input() {
    page_1_revealer_scenario_input->set_reveal_child(page_1_combobox_scenario->get_active_id() == "0");
}

/**
 * Called when the user inputs some data and wants to start a new simulation
 * Method checks for valid input and if positive starts a new simulation
 */
void Gui::on_page_1_submit_button_clicked() {

    // check if the user wants to generate an output
    if (page_1_combobox_output->get_active_id() == "0") {
        generate_output = true;
        page_4_label_output_path->set_text("<b>View the computed data in your filebrowser:</b>");
        page_4_label_output_path->set_use_markup(true);
        page_4_button_open_file_location->set_sensitive(true);
        page_4_button_paraview->set_sensitive(true);
    } else {
        generate_output = false;
        page_4_label_output_path->set_text("<b>No output has been generated</b>");
        page_4_label_output_path->set_use_markup(true);
        page_4_button_open_file_location->set_sensitive(false);
        page_4_button_paraview->set_sensitive(false);
    }

    // check the boundary conditions
    uses_walls = page_1_combobox_boundary->get_active_id() != "0";

    if (page_1_combobox_scenario->get_active_id() == "0") {
        scenario_id = 0;
    } else if (page_1_combobox_scenario->get_active_id() == "1") {
        scenario_id = 1;
    } else {
        scenario_id = 2;
    }


    // check for valid input values
    if (page_1_spin_button_x_dim->get_value() < 1 ||
        page_1_spin_button_y_dim->get_value() < 1 ||
        page_1_spin_button_sim_time->get_value() < 1) {
        // Display error message that every input has to be provided
        page_1_label_error->set_text("<span color=\"red\">Please make sure that every field contains a value!</span>");
        page_1_label_error->set_use_markup(true);
        page_1_label_error->set_visible(true);
        return;
    }



    // check bathymetry file
    if (scenario_id == 0 && !verify_input(page_1_file_chooser_bathymetry->get_filename())) {
        // Display error message that the bathymetry file is not valid
        page_1_label_error->set_text("<span color=\"red\">Bathymetry file is not valid!</span>");
        page_1_label_error->set_use_markup(true);
        page_1_label_error->set_visible(true);
        return;
    } else if (scenario_id == 0 && !verify_input(page_1_file_chooser_displacement->get_filename())) {
        // Display error message that the displacement file is not valid
        page_1_label_error->set_text("<span color=\"red\">Displacement file is not valid!</span>");
        page_1_label_error->set_use_markup(true);
        page_1_label_error->set_visible(true);
        return;
    }

    // check for valid output settings
    if (generate_output &&
            (page_1_file_chooser_output_directory->get_filename().empty() ||
            page_1_entry_output_file_name->get_text().empty())) {
        // Display error message that not output directories were given
        page_1_label_error->set_text("<span color=\"red\">Output directory or output filename not specified!</span>");
        page_1_label_error->set_use_markup(true);
        page_1_label_error->set_visible(true);
        return;
    }


    // construct scenario
    if (scenario_id == 0) {
        // from file
        scen = new file_scenario {file_scenario::create(page_1_file_chooser_bathymetry->get_filename(),
                                                            page_1_file_chooser_displacement->get_filename())};
    } else if (scenario_id == 1) {
        // radial dam break
        scen = new radial_dambreak_obstacle_scenario {};
    } else {
        // artificial scenario
        scen = new artificial_tsunami_scenario {};
    }


    // construct sim_options
    sim_options sim_opt {std::array<std::size_t, 2>{static_cast<unsigned long>(page_1_spin_button_x_dim->get_value()),
                                                    static_cast<unsigned long>(page_1_spin_button_y_dim->get_value())},
                         uses_walls,
                         static_cast<float>(page_1_spin_button_sim_time->get_value()),
                         static_cast<int>(page_1_spin_button_num_threads->get_value())};

    // construct output options
    output_options out_opt {generate_output,
                            page_1_file_chooser_output_directory->get_filename() + "/" + page_1_entry_output_file_name->get_text() + ".nc",
                            page_1_file_chooser_output_directory->get_filename() + "/" + page_1_entry_output_file_name->get_text() + "-checkpoint.nc",
                            static_cast<size_t>(page_1_spin_button_num_time_steps->get_value()),
                            static_cast<float>(page_1_spin_button_coarseness_factor->get_value()),
                            *this};

    // construct simulation
    simulation sim {simulation::create(*scen, sim_opt)};

    // set the output directory
    directory = page_1_file_chooser_output_directory->get_filename();
    // set filename
    filename = page_1_entry_output_file_name->get_text();

    // display progress page
    gui_stack->set_visible_child("page_progress");

    // call page 3 handler
    Gui::page_3_handler();

    // set time for start
    time(&time_start);

    // start simulation in separate thread
    std::thread simulation_thread (&simulation::run, sim, out_opt);
    simulation_thread.detach();
}

/**
 * Button is located on the third page
 * Invoked when the user wants to load a checkpoint
 */
void Gui::on_page_2_submit_button_clicked() {
    if (page_2_file_chooser_load_checkpoint->get_filename().empty() ||
    page_2_file_chooser_load_output_file->get_filename().empty()) {
        page_2_label_error->set_visible(true);
        return;
    } else {
        page_2_label_error->set_visible(false);
    }

    // set the directory path
    directory = page_2_file_chooser_load_output_file->get_filename();

    // cut off the file name from the directory path
    directory.erase(std::find_if(directory.rbegin(), directory.rend(), [](int ch) {
        return ch == '/';
    }).base(), directory.end());

    // extract filename
    filename = page_2_file_chooser_load_output_file->get_filename();
    int name_len = filename.length();
    int tmp_pos = 0;
    // get position of last '/' character. searching string from the back
    for (int i {name_len}; i > 0; i--) {
        if(filename.at(i) == '/') {
            tmp_pos = i+1;
            break;
        }
    }
    // trim string
    filename = filename.substr(tmp_pos);

    // set output label
    page_4_label_output_path->set_text("<b>View the computed data in your filebrowser:</b>");
    page_4_label_output_path->set_use_markup(true);
    page_4_button_open_file_location->set_sensitive(true);
    page_4_button_paraview->set_sensitive(true);
    // call page 3 handler
    Gui::page_3_handler();

    // set time for start
    time(&time_start);

/*

    // hide label done
    page_3_label_done->set_visible(false);
    // display progress page
    gui_stack->set_visible_child("page_progress");

    // create simulation object
    simulation sim {load_checkpoint(page_2_file_chooser_load_checkpoint->get_filename(),
            page_2_file_chooser_load_output_file->get_filename(),
            page_2_spin_button_num_threads->get_value())};

    // start simulation in separate thread
    std::thread load_cp_thread (&simulation::run, sim);
    load_cp_thread.detach();
    */

    gui_stack->set_visible_child("page_progress");
}

/**
 * Invoked when starting a simulation. Method will start the progressbar animation thread and detach it.
 */
void Gui::page_3_handler() {
    page_3_label_time_remaining_dynamic->set_text("");
    simulation_fraction = 0;
    simulation_time_remaining = 0;
    worker_time_remaining = 0;
    worker_actual_fraction = 0;
    worker_target_fraction = 0;


    std::thread progress_loop_thread(&Gui::page_3_animation_worker, this);
    progress_loop_thread.detach();
}

/**
 * Called when the user chooses to cancel the simulation
 */
void Gui::on_page_3_button_cancel_clicked() {
    //scen.stop()
    on_return_to_main_menu();
}

/**
 * Called when starting the simulation in a detached thread.
 * Method is used to animate the progressbar.
 */
void Gui::page_3_animation_worker() {
    double progress_gap = 0.0;
    double progress_step = 0.0;
    int frames_passed = 0;
    int minutes = 0;
    int seconds = 0;
    bool finish = false;

    //page_3_progress_bar_progress->set_fraction(0);
    //while(Gtk::Main::events_pending()) Gtk::Main::iteration(false);


    // start animation loop
    // animation will attempt to reach updated progress within 1 second at 60fps
    while (true) {
        {
            std::lock_guard<std::mutex> lock(progress_bat_mtx);
            worker_target_fraction = simulation_fraction;
            worker_time_remaining = simulation_time_remaining;
        }

        // sleep 1 frame
        usleep(16666);

        // perform computation and animation
        if (worker_target_fraction == 1 && worker_time_remaining == -1 && worker_actual_fraction >= 1) {
            // simulation is complete, progress bar is full
            // show minutes prediction
            worker_message = "00:00 Minute(s)";
            // show label done
            //page_3_label_done->set_visible(true);

            // send system notification
            std::string notification = "notify-send -t 5000000 'SWE' 'The computation has finished and is now ready for review'";
            std::system(notification.c_str());

            // reset progress and time_remaining
            worker_target_fraction = 0;
            worker_time_remaining = 0;
            // leave loop
            break;
        } else if (worker_target_fraction == 1 && worker_time_remaining == -1 && worker_actual_fraction < 1) {
            if (!finish) {
                // calculate fraction so that bar is full within 1 second by 60 fps
                progress_gap = 1. - worker_actual_fraction;
                progress_step = progress_gap / 60;

                finish = true;
            }

            // set new progress
            worker_actual_fraction += progress_step;

        } else if (worker_time_remaining == -2) {
            // simulation encountered an error, leave loop immediately
            break;
        } else {
            // update progress gap
            progress_gap = worker_target_fraction - worker_actual_fraction;
            progress_step = progress_gap / 60;

            // set new progress
            worker_actual_fraction += progress_step;

            if (frames_passed == 0) {
                // update time remaining message every second
                minutes = worker_time_remaining / 60;
                seconds = worker_time_remaining % 60;
                worker_message = ((minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes)) + ":" +
                        ((seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds)) + " Minute(s)";
            }

            frames_passed++;
            frames_passed %= 60;
        }

        // notify_progress_bar_update main thread
        notify_progress_bar_update();
    } // while

    // notify_progress_bar_update main thread
    notify_progress_bar_update();

    // keep displaying the progress page for 1.5 seconds
    usleep(1200000);

    // if no error occurred go to result page
    if (worker_time_remaining != -2) {
        // call result page
        notify_page_change_to_result();
    }
}

/**
 * Invoked after a signal from the dispatcher progressbar was omitted.
 * Method is executed in main thread and will collect updated values
 * for gui manipulation
 */
void Gui::update_widgets() {
    {
        std::lock_guard<std::mutex> lock(progress_bat_mtx);
        // updates the values for main thread
        main_thread_fraction = worker_actual_fraction;
        main_thread_message = worker_message;
    }

    // update gui
    page_3_progress_bar_progress->set_fraction(main_thread_fraction);
    while(Gtk::Main::events_pending()) Gtk::Main::iteration(false);
    page_3_label_time_remaining_dynamic->set_text(main_thread_message);
}

/**
 * Invoked after a signal from the dispatcher progresssbar was omitted.
 * This method is connected to this signal.
 */
void Gui::on_notification_from_animation_worker_thread() {
    update_widgets();
}

/**
 * Called by the animation worker thread. Invokes a signal
 */
void Gui::notify_progress_bar_update() {
    dispatcher_progressbar_update.emit();
}

/**
 * Invoked after a sigal from the dispatcher show result page was omitted.
 * This method is connected to this signal.
 */
void Gui::on_notification_to_change_to_result_page() {
    gui_stack->set_visible_child("page_result");
    delete scen;
    scen = nullptr;
}

/**
 * Called by the animation worker thread. Invokes a signal
 */
void Gui::notify_page_change_to_result() {
    dispatcher_show_result_page.emit();
}


/**
 * After the simulation is finished, the user is able to view their output file directly in its directory.
 * Invoked when button 'open file location' is clicked.
 */
void Gui::on_page_4_open_location_button_clicked() {
    std::string open_directory_command = "xdg-open " + directory;
    std::system(open_directory_command.c_str());
}

/**
 * After the simulation is finished the user is able to open paraview at the specified location
 * Invoked when button 'open paraview'
 */
void Gui::on_page_4_open_paraview_button_clicked() {
    std::string open_paraview_command = "paraview " + directory + "/" + filename + ".nc";

    if (fork() == 0) {
        std::system(open_paraview_command.c_str());
        exit(0);
    }
    /*try {
        std::system(open_paraview_command.c_str());
    } catch (...) {
        page_4_label_paraview_error->set_visible(true);
    }*/
}

/**
 * Called by swe backend. Invoked to receive information about simulation status
 * @param _progress Specifies how much progress already has been made
 * @param _time_remaining Parameter specifies how long the simulation will probably still take in seconds.
 */
void Gui::update_progress(float _progress, int _time_remaining) {

    {
        std::lock_guard<std::mutex> lock(progress_bat_mtx);
        this->simulation_time_remaining = _time_remaining;
        this->simulation_fraction = _progress;
    }


    if (simulation_fraction == 1 && simulation_time_remaining < 0) {
        // set end time
        time(&time_end);
        // calculate the diff time
        diff_seconds = difftime(time_end, time_start);
        // create string
        int minutes = diff_seconds / 60;
        int seconds = diff_seconds % 60;

        {
            std::lock_guard<std::mutex> lock(result_duration_mtx);

            // final string
            result_duration_string = ((minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes)) + ":" +
                                       ((seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds)) + " Minute(s)";
        }
        // omit signal to set the result duration label
        dispatcher_set_result_duration.emit();
    }
}

void Gui::on_notification_to_set_result_duration() {
    {
        std::lock_guard<std::mutex> lock(result_duration_mtx);
        page_4_label_time_spent_dynamic->set_text(result_duration_string);
    }
}



/**
 * Called by swe backend. Invoked to show the error page as soon as an error has occurred
 * @param error Contains the error message
 */
void Gui::show_error_page(std::string error) {
    // set time remaining to 0
    update_progress(1.0F, -2);
    page_5_label_error_message->set_text(error);
    gui_stack->set_visible_child("page_error");
}
