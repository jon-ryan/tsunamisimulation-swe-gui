#ifndef SIMULATION_H
#define SIMULATION_H

/*#include "gui.h"
#include "scenario.h"

#include <array>
#include <chrono>

#include <stdexcept>
#include <thread>
#include <vector>*/
#include "gui.h"
#include <string>
#include <cstddef>
#include "solver.h"
#include "writer.h"
#include <omp.h>

/** Options regarding the creation of output */
struct output_options {
  /** Whether output should be created */
  const bool create_output;

  /** Name of the output file */
  const std::string output_name;

  /** Name of the checkpoint file */
  const std::string checkpoint_name;

  /** Maximum amount of timesteps in output file (0 = infinite) */
  const std::size_t max_num_timesteps;

  /** Factor by which to reduce the output grid size. Must be >= 1. */
  const float coarse_factor;

  /** GUI reference for pushing updates */
  Gui& gui;
};

/** Simulation parameters */
struct sim_options {
  /** Number of cells in x and y direction */
  const std::array<std::size_t, 2> num_cells;

  /** Whether boundaries should act like walls */
  const bool reflective_bounds;

  /** Amount of time to be simulated in seconds */
  const float duration;

  /** Maximum amount of threads to be used by OpenMP */
  const int num_threads;
};

/** Simulates a scenario using dimensional splitting and a f-wave solver. */
class simulation {
  const std::array<std::size_t, 2> num_cells;
  const std::array<float, 2> cell_size;
  const std::array<float, 2> origin;
  const bool reflective_bounds;
  const std::vector<float> b;
  std::vector<float> h;
  std::vector<float> hu;
  std::vector<float> hv;
  float time;
  const float duration;
  bool stop{false};

  simulation(const std::array<std::size_t, 2>& num_cells,
             const std::array<float, 2>& cell_size,
             const std::array<float, 2>& origin,
             const bool& reflective_bounds,
             const std::vector<float>& b,
             const std::vector<float>& h,
             const std::vector<float>& hu,
             const std::vector<float>& hv,
             const float& time,
             const float& duration,
             const int& num_threads);

  /** Compute current time step */
  void computeTimestep(bool& error_happened);

public:
  static simulation create(const scenario& scen, const sim_options& sim_opt);

  /** Starts the simulation */
  void run(output_options out_opt);

  /** Stops the simulation */
  void abort();
};

#endif // SIMULATION_H
