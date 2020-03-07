#ifndef WRITER_H
#define WRITER_H

#include <array>
#include <cstddef>
#include <netcdf>
#include <string>
#include <vector>

#include "scenario.h"

/**
 * Class for writing simulation output to a file
 */
class writer {
  netCDF::NcFile file;
  netCDF::NcDim time_dim;
  netCDF::NcDim y_dim;
  netCDF::NcDim x_dim;
  netCDF::NcVar time_var;
  netCDF::NcVar y_var;
  netCDF::NcVar x_var;
  netCDF::NcVar b_var;
  netCDF::NcVar h_var;
  netCDF::NcVar hu_var;
  netCDF::NcVar hv_var;
  const float* time;
  const float* h;
  const float* hu;
  const float* hv;
  const std::size_t& timesteps_written;

public:
  /**
   * Create a new output file
   * @param filename Name of file
   * @param num_cells Number of cells in x and y direction
   * @param origin Coordinates of bottom left corner
   * @param cell_size Size of cells in x and y direction
   * @param time Reference to current simulation time
   * @param b Reference to simulation bathymetry
   * @param h Reference to
   * @param hu
   * @param hv
   */
  writer(const std::string& filename,
         std::array<std::size_t, 2> num_cells,
         std::array<float, 2> origin, std::array<float, 2> cell_size,
         const float& time, const std::vector<float>& b,
         const std::vector<float>& h, const std::vector<float>& hu,
         const std::vector<float>& hv,
         const std::size_t& timesteps_written)
          : file{filename, netCDF::NcFile::replace, netCDF::NcFile::nc4},
            time_dim{file.addDim("time")},
            y_dim{file.addDim("y", num_cells.at(1))},
            x_dim{file.addDim("x", num_cells.at(0))},
            time_var{file.addVar("time", netCDF::ncFloat, time_dim)},
            y_var{file.addVar("y", netCDF::ncFloat, y_dim)},
            x_var{file.addVar("x", netCDF::ncFloat, x_dim)},
            b_var{file.addVar("b", netCDF::ncFloat, {y_dim, x_dim})},
            h_var{file.addVar("h", netCDF::ncFloat, {time_dim, y_dim, x_dim})},
            hu_var{file.addVar("hu", netCDF::ncFloat, {time_dim, y_dim, x_dim})},
            hv_var{file.addVar("hv", netCDF::ncFloat, {time_dim, y_dim, x_dim})},
            time{&time},
            h{&h.at(num_cells.at(0))},
            hu{&hu.at(num_cells.at(0))},
            hv{&hv.at(num_cells.at(0))},
            timesteps_written{timesteps_written} {
      file.putAtt("Conventions", "CF-1.7");
      time_var.putAtt("units", "seconds since begin of simulation");

      std::vector<float> y(num_cells.at(1), 0.F);
      for (std::size_t i{0}; i < y.size(); ++i) {
          y.at(i) = origin.at(1) + (i + .5F) * cell_size.at(1);
      }
      y_var.putVar(y.data());

      std::vector<float> x(num_cells.at(0), 0.F);
      for (std::size_t i{0}; i < x.size(); ++i) {
          x.at(i) = origin.at(0) + (i + .5F) * cell_size.at(0);
      }
      x_var.putVar(x.data());

      b_var.putVar(&b.at(num_cells.at(0)));

      time_var.putVar({0}, this->time);

      std::vector<size_t> start{0, 0, 0};
      std::vector<size_t> count{1, y_dim.getSize(), x_dim.getSize()};
      h_var.putVar(start, count, this->h);
      hu_var.putVar(start, count, this->hu);
      hv_var.putVar(start, count, this->hv);
  }

  inline void write() {
      time_var.putVar({timesteps_written}, time);
      h_var.putVar({timesteps_written, 0, 0},
                   {1, y_dim.getSize(), x_dim.getSize()}, h);
      hu_var.putVar({timesteps_written, 0, 0},
                    {1, y_dim.getSize(), x_dim.getSize()}, hu);
      hv_var.putVar({timesteps_written, 0, 0},
                    {1, y_dim.getSize(), x_dim.getSize()}, hv);
  }

  /**
   * FUNCTION CRASHES
   *
   * Makes output coarse by putting the coarse grid on top of the simulation grid like a mask. A simulation cell
   * contributes to a coarse cell depending on how much of it is covered by the coarse cell. Example: A coarse cell
   * covers cell A completely, one third of cell B, and no other cells. Then the value of the coarse cell will be
   * approx. (1 * A + 0.33 * B) / 1.33. This algorithm provides the closest resemblance to the original grid and avoids
   * stretching at the top and right borders.
   * @param in Simulation grid variable
   * @return Coarse grid variable
   */
  // [[nodiscard]] inline std::vector<float> apply_coarse(const std::vector<float>& in) const {
  //   // Storage for output data
  //   std::vector<float> out(out_num_cells[0] * out_num_cells[1]);
  //
  //   // Iterate over output cells
  //   for (std::size_t y{0}; y < out_num_cells[1]; ++y) {
  //     for (std::size_t x{0}; x < out_num_cells[0]; ++y) {
  //       // Absolute y position on simulation grid
  //       auto sim_y_pos{1.F + static_cast<float>(y) * coarse_mod[1]};
  //       // Remaining y distance in current cell on simulation grid
  //       auto rem_sim_y{std::floor(sim_y_pos + 1.F) - sim_y_pos};
  //       // Remaining y distance in current cell on output grid
  //       auto rem_out_y{coarse_mod[1]};
  //
  //       // Fill output cell vertically
  //       while (rem_out_y > 0.F) {
  //         // Progress in y direction
  //         auto y_step{std::min<float>(rem_sim_y, rem_out_y)};
  //
  //         // Absolute x position on simulation grid
  //         auto sim_x_pos{static_cast<float>(x) * coarse_mod[0]};
  //         // Remaining x distance in current cell on simulation grid
  //         auto rem_sim_x{std::floor(sim_x_pos + 1.F) - sim_x_pos};
  //         // Remaining x distance in current cell on output grid
  //         auto rem_out_x{coarse_mod[0]};
  //
  //         // Fill output cell horizontally
  //         while (rem_out_x > 0.F) {
  //           // Progress in x direction
  //           auto x_step{std::min<float>(rem_sim_x, rem_out_x)};
  //
  //           // Add this slice to output
  //           out[y * out_num_cells[0] + x] += y_step * x_step *
  //                                            in[static_cast<std::size_t>(sim_y_pos) * sim_num_cells[0] +
  //                                               static_cast<std::size_t>(sim_x_pos)];
  //
  //           // Update x progress
  //           rem_sim_x -= x_step;
  //           sim_x_pos += x_step;
  //
  //           // Advance to next simulation cell in x direction, if current one is done
  //           if (rem_sim_x <= 0.F) {
  //             sim_x_pos = std::floor(sim_x_pos + .5F);
  //             rem_sim_x = 1.F;
  //           }
  //           rem_out_x -= x_step;
  //         }
  //         // Update y progress
  //         rem_sim_y -= y_step;
  //         sim_y_pos += y_step;
  //         // Advance to next simulation cell in y direction, if current one is done
  //         if (rem_sim_y <= 0.F) {
  //           sim_y_pos = std::floor(sim_y_pos + .5F);
  //           rem_sim_y = 1.F;
  //         }
  //         rem_out_y -= y_step;
  //       }
  //     }
  //   }
  //   // Divide each cell by combined coarseness factor
  //   #pragma omp parallel for schedule(auto) default(none) shared(out)
  //   for (std::size_t i = 0; i < out.size(); ++i) {
  //     out[i] /= coarse_mod[0] * coarse_mod[1];
  //   }
  //   return out;
  // }


  // CONSTRUCTOR BODY WITH CHECKPOINT CREATION, WORKED IN AN OLDER COMMIT
  //
  // // Attributes
  // out_file.putAtt("Conventions", "CF-1.7");
  // time_var_out.putAtt("units", "seconds since begin of simulation");
  //
  // // Checkpoint coordinate grid
  // std::vector<float> y_cp(num_cells[1]);
  // for (std::size_t i{0}; i < y_cp.size(); ++i) { y_cp[i] = origin[1] + (i + .5F) * cell_size[1]; }
  // y_var_cp.putVar(y_cp.data());
  //
  // std::vector<float> x_cp(num_cells[0]);
  // for (std::size_t i{0}; i < x_cp.size(); ++i) { x_cp[i] = origin[0] + (i + .5F) * cell_size[0]; }
  // x_var_cp.putVar(x_cp.data());
  //
  // // Write initial checkpoint
  // b_var_cp.putVar(&b[sim_num_cells[0]]);
  // time_var_cp.putVar(this->time);
  // h_var_cp.putVar(&h[sim_num_cells[0]]);
  // hu_var_cp.putVar(&hu[sim_num_cells[0]]);
  // hv_var_cp.putVar(&hv[sim_num_cells[0]]);
  //
  // // Additional data for cp file
  // const int use_walls_int{use_walls ? 1 : 0};
  // cp_file.addVar("use_walls", netCDF::ncInt).putVar(&use_walls_int);
  // cp_file.addVar("duration", netCDF::ncFloat).putVar(&duration);
  // cp_file.addVar("num_timesteps", netCDF::ncFloat).putVar(&num_timesteps);
  // cp_file.addVar("coarse_factor", netCDF::ncFloat).putVar(&coarse_factor);
  //
  // // Output grid
  // if (coarse_factor == 1.F) {
  //   // Reuse checkpoint coordinate grid
  //   y_dim_out = out_file.addDim("y", num_cells[1]);
  //   y_var_out = out_file.addVar("y", netCDF::ncFloat, y_dim_out);
  //   y_var_out.putVar(y_cp.data());
  //   x_dim_out = out_file.addDim("x", num_cells[0]);
  //   x_var_out = out_file.addVar("x", netCDF::ncFloat, x_dim_out);
  //   x_var_out.putVar(x_cp.data());
  // } else {
  //   // Create coarse coordinate grid
  //   y_dim_out = out_file.addDim("y", out_num_cells[1]);
  //   y_var_out = out_file.addVar("y", netCDF::ncFloat, y_dim_out);
  //   std::vector<float> y_out(out_num_cells[1]);
  //   for (std::size_t i{0}; i < y_out.size(); ++i) { y_out[i] = origin[1] + (i + .5F) * cell_size[1] * coarse_mod[1]; }
  //   y_var_out.putVar(y_out.data());
  //   x_dim_out = out_file.addDim("x", out_num_cells[0]);
  //   x_var_out = out_file.addVar("x", netCDF::ncFloat, x_dim_out);
  //   std::vector<float> x_out(out_num_cells[0]);
  //   for (std::size_t i{0}; i < x_out.size(); ++i) { x_out[i] = origin[0] + (i + .5F) * cell_size[0] * coarse_mod[0]; }
  //   x_var_out.putVar(x_out.data());
  // }
  //
  // // Output variables
  // b_var_out = out_file.addVar("b", netCDF::ncFloat, {y_dim_out, x_dim_out});
  // h_var_out = out_file.addVar("h", netCDF::ncFloat, {time_dim, y_dim_out, x_dim_out});
  // hu_var_out = out_file.addVar("hu", netCDF::ncFloat, {time_dim, y_dim_out, x_dim_out});
  // hv_var_out = out_file.addVar("hv", netCDF::ncFloat, {time_dim, y_dim_out, x_dim_out});
  //
  // // Write initial output
  // time_var_out.putVar({0}, this->time);
  // const std::vector<size_t> start{0, 0, 0};
  // const std::vector<size_t> count{1, y_dim_out.getSize(), x_dim_out.getSize()};
  // if (coarse_factor == 1.F) {
  //   b_var_out.putVar(&b[num_cells[0]]);
  //   h_var_out.putVar(start, count, &h[num_cells[0]]);
  //   hu_var_out.putVar(start, count, &hu[num_cells[0]]);
  //   hv_var_out.putVar(start, count, &hv[num_cells[0]]);
  // } else {
  //   const auto b_out = apply_coarse(b);
  //   const auto h_out = apply_coarse(h);
  //   const auto hu_out = apply_coarse(hu);
  //   const auto hv_out = apply_coarse(hv);
  //   b_var_out.putVar(&b_out);
  //   h_var_out.putVar(start, count, &h_out);
  //   hu_var_out.putVar(start, count, &hu_out);
  //   hv_var_out.putVar(start, count, &hv_out);
  // }


  // WRITE FUNCTION WITH CHECKPOINT, WORKED IN AN OLDER COMMIT
  //
  // /** Write current simulation data to output and checkpoint files */
  // inline void write() {
  //   // Write checkpoint data
  //   time_var_cp.putVar(time);
  //   h_var_cp.putVar(&h[sim_num_cells[0]]);
  //   hu_var_cp.putVar(&hu[sim_num_cells[0]]);
  //   hv_var_cp.putVar(&hv[sim_num_cells[0]]);
  //
  //   // Write output data
  //   time_var_out.putVar({timesteps_written}, time);
  //   const std::vector<std::size_t> start{timesteps_written, 0, 0};
  //   static const std::vector<std::size_t> count{1, y_dim_out.getSize(), x_dim_out.getSize()};
  //   if (coarse_factor == 1.F) {
  //     h_var_out.putVar(start, count, &h[sim_num_cells[0]]);
  //     hu_var_out.putVar(start, count, &hu[sim_num_cells[0]]);
  //     hv_var_out.putVar(start, count, &hv[sim_num_cells[0]]);
  //   } else {
  //     const auto h_out = apply_coarse(h);
  //     const auto hu_out = apply_coarse(hu);
  //     const auto hv_out = apply_coarse(hv);
  //     h_var_out.putVar(start, count, &h_out);
  //     hu_var_out.putVar(start, count, &hu_out);
  //     hv_var_out.putVar(start, count, &hv_out);
  //   }
  // }

};

#endif  // WRITER_H
