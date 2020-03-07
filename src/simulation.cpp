#include "simulation.h"

simulation::simulation(const std::array<std::size_t, 2> &num_cells,
                       const std::array<float, 2> &cell_size,
                       const std::array<float, 2> &origin,
                       const bool &reflective_bounds,
                       const std::vector<float> &b,
                       const std::vector<float> &h,
                       const std::vector<float> &hu,
                       const std::vector<float> &hv,
                       const float &time,
                       const float &duration,
                       const int &num_threads)
        : num_cells{num_cells},
          cell_size{cell_size},
          origin{origin},
          reflective_bounds{reflective_bounds},
          b{b},
          h{h},
          hu{hu},
          hv{hv},
          time{time},
          duration{duration} { omp_set_num_threads(num_threads); }

simulation simulation::create(const scenario &scen, const sim_options &sim_opt) {
    // Number of cells
    auto num_cells{sim_opt.num_cells};

    // Cell size
    auto scen_size{scen.get_size()};
    std::array<float, 2> cell_size{scen_size[0] / num_cells[0], scen_size[1] / num_cells[1]};

    // Bathymetry and water height
    auto origin{scen.get_origin()};
    std::vector<float> b(num_cells[0] * (num_cells[1] + 2));
    std::vector<float> h(num_cells[0] * (num_cells[1] + 2));
    // Initialize inner cells
    for (std::size_t y{1}; y <= num_cells[1]; ++y) {
        for (std::size_t x{0}; x < num_cells[0]; ++x) {
            b[y * num_cells[0] + x] = scen.get_bathymetry(origin[0] + (static_cast<float>(x) + .5F) * cell_size[0],
                                                          origin[1] + (static_cast<float>(y) - .5F) * cell_size[1]);
            h[y * num_cells[0] + x] = b[y * num_cells[0] + x] >= 0.F ? 0.F : scen.get_height(
                    origin[0] + (static_cast<float>(x) + .5F) * cell_size[0],
                    origin[1] + (static_cast<float>(y) - .5F) * cell_size[1]);
        }
    }

    // Initialize boundary bathymetry at bottom and top
    for (std::size_t x{0}; x < num_cells[0]; ++x) {
        b[x] = b[num_cells[0] + x];
        b[(num_cells[1] + 1) * num_cells[0] + x] = b[num_cells[1] * num_cells[0] + x];
    }

    return simulation{num_cells,
                      cell_size,
                      origin,
                      sim_opt.reflective_bounds,
                      b,
                      h,
                      std::vector<float>(num_cells[0] * (num_cells[1] + 2)),
                      std::vector<float>(num_cells[0] * (num_cells[1] + 2)),
                      0.F,
                      sim_opt.duration,
                      sim_opt.num_threads};
}

void simulation::run(output_options out_opt) {
    bool error_happened{false};
    //omp_set_num_threads(num_threads == 0 ? (std::max<int>(std::thread::hardware_concurrency(), 1)) : num_threads);
    // Time at which simulation started
    const auto start_time{std::chrono::high_resolution_clock::now()};

    // With or without output
    if (out_opt.create_output) {
        // Amount of timesteps currently in output file
        std::size_t timesteps_written{1};

        // Initialize writer
        writer out_writer{out_opt.output_name,
                //out_opt.checkpoint_name,
                          num_cells, origin, cell_size,
                //out_opt.coarse_factor,
                //duration,
                //use_walls,
                //out_opt.num_timesteps,
                          time, b, h, hu, hv, timesteps_written};

        // Loop until end of simulation
        while (time < duration) {
            computeTimestep(error_happened);
            if (stop) { return; }
            if (error_happened) {
                out_opt.gui.show_error_page("Negative water height encountered during calculations!");
                return;
            }
            // Write current data to output
            if (out_opt.max_num_timesteps == 0 || time >= duration / out_opt.max_num_timesteps * timesteps_written) {
                out_writer.write();
                ++timesteps_written;
            }
            if (time <= 0.F) { throw std::runtime_error{"No time has passed during timestep!"}; }
            // Tell GUI, how much time is remaining
            out_opt.gui.update_progress(std::min<float>(time / duration, .99F), std::max(static_cast<int>(
                                                                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                                                                         std::chrono::high_resolution_clock::now() -
                                                                                                         start_time).count() *
                                                                                                 (duration - time) /
                                                                                                 time + .5F),
                                                                                         1));
        }
    } else {
        // Loop until end of simulation
        while (time < duration) {
            computeTimestep(error_happened);
            if (stop) { return; }
            if (error_happened) {
                out_opt.gui.show_error_page("Negative water height encountered during calculations!");
                return;
            }
            //Tell GUI, how much time is remaining
            out_opt.gui.update_progress(std::min<float>(time / duration, .99F), std::max(static_cast<int>(
                                                                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                                                                         std::chrono::high_resolution_clock::now() -
                                                                                                         start_time).count() *
                                                                                                 (duration - time) /
                                                                                                 time + .5F),
                                                                                         1));
        }
    }
    // Tell GUI, that we're done
    out_opt.gui.update_progress(1.F, -1.F);
}

void simulation::computeTimestep(bool &error_happened) {
    // Set up boundaries according to uses_walls
#pragma omp parallel for schedule(static) default(none)
    for (std::size_t x = 0; x < num_cells[0]; ++x) {
        // Height
        h[x] = h[num_cells[0] + x];
        h[(num_cells[1] + 1) * num_cells[0] + x] = h[num_cells[1] * num_cells[0] + x];
        // Momentum in x direction
        hu[x] = hu[num_cells[0] + x];
        hu[(num_cells[1] + 1) * num_cells[0] + x] = hu[num_cells[1] * num_cells[0] + x];
        // Momentum in y direction
        hv[x] = reflective_bounds ? -hv[num_cells[0] + x] : hv[num_cells[0] + x];
        hv[(num_cells[1] + 1) * num_cells[0] + x] = reflective_bounds ? -hv[num_cells[1] * num_cells[0] + x] : hv[
                num_cells[1] * num_cells[0] + x];
    }

    // Pull as much storage as possible out of the loops
    std::vector<float> h_updates_neg((num_cells[0] + 1) * (num_cells[1] + 2));
    std::vector<float> h_updates_pos((num_cells[0] + 1) * (num_cells[1] + 2));
    std::vector<float> hu_updates_neg((num_cells[0] + 1) * (num_cells[1] + 2));
    std::vector<float> hu_updates_pos((num_cells[0] + 1) * (num_cells[1] + 2));
    float max_wave_speed{0.F};
    // X Sweep
#pragma omp parallel for collapse(2) schedule(static) default(none) shared(h_updates_neg, hu_updates_neg, h_updates_pos, hu_updates_pos) reduction(max \
                                                                                                                                                 : max_wave_speed)
    for (std::size_t y = 0; y < num_cells[1] + 2; ++y) {
        for (std::size_t x = 0; x <= num_cells[0]; ++x) {
            // Cell indices for this iteration
            const std::size_t index_r{y * num_cells[0] + x};
            const std::size_t index_l{index_r - 1};
            // Set up input for solver
            std::array<float, 6> in{};
            if (x == 0) {              // Left border
                if (b[index_r] >= 0.F) { // Cell dry
                    continue;
                } else { // Cell wet
                    in[3] = b[index_r];
                    in[4] = h[index_r];
                    in[5] = hu[index_r];
                    in[0] = in[3];
                    in[1] = in[4];
                    if (reflective_bounds) { // Wall boundaries
                        in[2] = -in[5];
                    } else { // Outflow boundaries
                        in[2] = in[5];
                    }
                }
            } else if (x == num_cells[0]) { // Right border
                if (b[index_l] >= 0.F) {      // Cell dry
                    continue;
                } else { // Cell wet
                    in[0] = b[index_l];
                    in[1] = h[index_l];
                    in[2] = hu[index_l];
                    in[3] = in[0];
                    in[4] = in[1];
                    if (reflective_bounds) { // Wall boundaries
                        in[5] = -in[2];
                    } else { // Outflow boundaries
                        in[5] = in[2];
                    }
                }
            } else {                                        // Inner edge
                if (b[index_l] >= 0.F && b[index_r] >= 0.F) { // Both cells dry
                    continue;
                } else if (b[index_l] >= 0.F) { // Left cell dry
                    in[3] = b[index_r];
                    in[4] = h[index_r];
                    in[5] = hu[index_r];
                    in[0] = in[3];
                    in[1] = in[4];
                    in[2] = -in[5];
                } else if (b[index_r] >= 0.F) { // Right cell dry
                    in[0] = b[index_l];
                    in[1] = h[index_l];
                    in[2] = hu[index_l];
                    in[3] = in[0];
                    in[4] = in[1];
                    in[5] = -in[2];
                } else { // Both cells wet
                    in[0] = b[index_l];
                    in[1] = h[index_l];
                    in[2] = hu[index_l];
                    in[3] = b[index_r];
                    in[4] = h[index_r];
                    in[5] = hu[index_r];
                }
            }

            // Run solver
            const std::array<float, 5> result{solve(in)};

            // Store results
            h_updates_neg[index_r + y] = result[0];
            hu_updates_neg[index_r + y] = result[1];
            h_updates_pos[index_r + y] = result[2];
            hu_updates_pos[index_r + y] = result[3];

            // Update maximum wave speed
            max_wave_speed = std::max<float>(max_wave_speed, result[4]);
        }
    }

    // Calculate timestep size
    const float timestep{.4F * cell_size[0] / max_wave_speed};

    // Apply updates
#pragma omp parallel for collapse(2) schedule(static) default(none) shared(h_updates_neg, hu_updates_neg, h_updates_pos, hu_updates_pos, timestep, error_happened)
    for (std::size_t y = 0; y < num_cells[1] + 2; ++y) {
        for (std::size_t x = 0; x < num_cells[0]; ++x) {
            const std::size_t index{y * num_cells[0] + x};
            if (b[index] < 0.F) {
                h[index] -= timestep / cell_size[0] * (h_updates_pos[index + y] + h_updates_neg[index + y + 1]);
                hu[index] -= timestep / cell_size[0] * (hu_updates_pos[index + y] + hu_updates_neg[index + y + 1]);
                if (h[index] <= 0.F) { error_happened = true; }
            }


        }
    }

    // Y Sweep
#pragma omp parallel for collapse(2) schedule(static) default(none) shared(h_updates_neg, hu_updates_neg, h_updates_pos, hu_updates_pos)
    for (std::size_t y = 0; y <= num_cells[1]; ++y) {
        for (std::size_t x = 0; x < num_cells[0]; ++x) {
            const std::size_t index_b{y * num_cells[0] + x};
            const std::size_t index_t{index_b + num_cells[0]};
            // Set up input for solver
            std::array<float, 6> in{};
            if (b[index_b] >= 0.F && b[index_t] >= 0.F) { // Both cells dry
                continue;
            } else if (b[index_b] >= 0.F) { // Bottom cell dry
                in[3] = b[index_t];
                in[4] = h[index_t];
                in[5] = hv[index_t];
                in[0] = in[3];
                in[1] = in[4];
                in[2] = -in[5];
            } else if (b[index_t] >= 0.F) { // Top cell dry
                in[0] = b[index_b];
                in[1] = h[index_b];
                in[2] = hv[index_b];
                in[3] = in[0];
                in[4] = in[1];
                in[5] = -in[2];
            } else { // Both cells wet
                in[0] = b[index_b];
                in[1] = h[index_b];
                in[2] = hv[index_b];
                in[3] = b[index_t];
                in[4] = h[index_t];
                in[5] = hv[index_t];
            }

            // Run solver
            std::array<float, 5> result{solve(in)};

            // Store results
            h_updates_neg[index_b] = result[0];
            hu_updates_neg[index_b] = result[1];
            h_updates_pos[index_b] = result[2];
            hu_updates_pos[index_b] = result[3];
        }
    }

    // Apply updates
#pragma omp parallel for collapse(2) schedule(static) default(none) shared(h_updates_neg, hu_updates_neg, h_updates_pos, hu_updates_pos, timestep, error_happened)
    for (std::size_t y = 1; y <= num_cells[1]; ++y) {
        for (std::size_t x = 0; x < num_cells[0]; ++x) {
            const std::size_t index{y * num_cells[0] + x};
            if (b[index] < 0.F) {
                h[index] -= timestep / cell_size[1] * (h_updates_pos[index - num_cells[0]] + h_updates_neg[index]);
                hv[index] -= timestep / cell_size[1] * (hu_updates_pos[index - num_cells[0]] + hu_updates_neg[index]);
                if (h[y * num_cells[0] + x] <= 0.F) { error_happened = true; }
            }

        }
    }

    // Update time
    time += timestep;
}

void simulation::abort() {
    stop = true;
}
