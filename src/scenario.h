#ifndef SCENARIO_H
#define SCENARIO_H

#include <array>
#include <cstddef>
#include <netcdf>
#include <vector>

/** Provides initial bathymetry and water height data for simulations. This is an abstract class. It only declares the functions a scenario class must have. */
class scenario {
public:
  virtual ~scenario() = default;

  /**
   * Getter for scenario origin
   * @return Origin (i.e. bottom left) position of scenario
   */
  [[nodiscard]] virtual std::array<float, 2> get_origin() const = 0;

  /**
   * Getter for scenrio domain size
   * @return Size of the scenario domain in x and y direction
   */
  [[nodiscard]] virtual std::array<float, 2> get_size() const = 0;

  /**
   * Getter for scenario bathymetry
   * @param x Position in x direction
   * @param y Position in y direction
   * @return Bathymetry at position (x, y)
   */
  [[nodiscard]] virtual float get_bathymetry(const float& x, const float& y) const = 0;

  /**
   * Getter for scenario water height
   * @param x Position in x direction
   * @param y Position in y direction
   * @return Initial water height at position (x, y)
   */
  [[nodiscard]] virtual float get_height(const float& x, const float& y) const = 0;
};

/** Loads scenario data from a bathymetry and a displacement file. */
class file_scenario final : public scenario {
private:
  /** Number of bathymetry cells in x and y direction */
  const std::array<std::size_t, 2> num_b;

  /** Bathymetry values */
  const std::vector<float> b;

  /** Size of bathymetry cells in x and y direction */
  const std::array<float, 2> size_b;

  /** Bathymetry origin */
  const std::array<float, 2> orig_b;

  /** Number of displacement cells in x and y direction */
  const std::array<std::size_t, 2> num_d;

  /** Displacement values */
  const std::vector<float> d;

  /** Size of displacement cells in x and y direction */
  const std::array<float, 2> size_d;

  /** Displacement origin */
  const std::array<float, 2> orig_d;

  /**
   * Creates scenario from extracted file data.
   * @param num_b Number of bathymetry cells in x and y direction
   * @param b Bathymetry values in scenario
   * @param size_b Size of bathymetry cells in x and y direction
   * @param orig_b Origin (bottom left corner) position of bathymetry in scenario
   * @param num_d Number of displacement cells in x and y direction
   * @param d Displacement values in scenario
   * @param size_d Size of displacement cells in x and y direction
   * @param orig_d Origin (bottom left corner) position of displacement in scenario
   */
  file_scenario(const std::array<std::size_t, 2>& num_b,
                std::vector<float>& b,
                const std::array<float, 2>& size_b,
                const std::array<float, 2>& orig_b,
                const std::array<std::size_t, 2>& num_d,
                std::vector<float>& d,
                const std::array<float, 2>& size_d,
                const std::array<float, 2>& orig_d);

  /**
   * Convert x and y coordinates to closest corresponding array index
   * @param x Position in x direction
   * @param y Position in y direction
   * @param num_x Number of cells in x direction
   * @param cell_size Cell size in x and y direction
   * @param origin Origin position
   * @return Index closest to position (x, y)
   */
  static std::size_t get_index(const float& x,
                               const float& y,
                               const std::size_t& num_x,
                               const std::array<float, 2>& cell_size,
                               const std::array<float, 2>& origin);

public:
  /**
   * Extracts data from bathymetry and displacement files to create a scenario
   * @param bathy_name Name of bathymetry file
   * @param displ_name Name of displacement file
   * @return Scenario
   */
  [[nodiscard]] static file_scenario create(const std::string& bathy_name, const std::string& displ_name);

  /** See scenario::get_origin. */
  [[nodiscard]] std::array<float, 2> get_origin() const final;

  /** See scenario::get_size. */
  [[nodiscard]] std::array<float, 2> get_size() const final;

  /** See scenario::get_bathymetry. */
  [[nodiscard]] float get_bathymetry(const float& x, const float& y) const final;

  /** See scenario::get_height. */
  [[nodiscard]] float get_height(const float& x, const float& y) const final;
};

/** Artificial scenario using positive and negative displacements */
class artificial_tsunami_scenario final : public scenario {
public:
  /** See scenario::get_origin. */
  [[nodiscard]] std::array<float, 2> get_origin() const final;

  /** See scenario::get_size. */
  [[nodiscard]] std::array<float, 2> get_size() const final;

  /** See scenario::get_bathymetry. */
  [[nodiscard]] float get_bathymetry(const float& x, const float& y) const final;

  /** See scenario::get_height. */
  [[nodiscard]] float get_height(const float& x, const float& y) const final;
};

/** Radial dambreak scenario with an obstacle */
class radial_dambreak_obstacle_scenario final : public scenario {
public:
  /** See scenario::get_origin. */
  [[nodiscard]] std::array<float, 2> get_origin() const final;

  /** See scenario::get_size. */
  [[nodiscard]] std::array<float, 2> get_size() const final;

  /** See scenario::get_bathymetry. */
  [[nodiscard]] float get_bathymetry(const float& x, const float& y) const final;

  /** See scenario::get_height. */
  [[nodiscard]] float get_height(const float& x, const float& y) const final;
};

/**
 * Checks a bathymetry or displacement file for basic correctness.
 * @param filename Name of file to be checked
 * @return true for correct files, false otherwise
 */
inline bool verify_input(const std::string& filename) {
  // Check that file exists and is accessible
  netCDF::NcFile file{};
  try {
    file.open(filename, netCDF::NcFile::read);
  } catch (...) {
    return false;
  }
  if (file.isNull()) {
    return false;
  }

  // Check that dimensions exist
  netCDF::NcDim dim_x{file.getDim("x")};
  netCDF::NcDim dim_y{file.getDim("y")};
  if (dim_x.isNull() || dim_y.isNull()) {
    return false;
  }

  // Check that dimensions aren't empty, need at least 2 cells in each
  // direction to be able to calculate cell size
  std::size_t size_dim_x{dim_x.getSize()};
  std::size_t size_dim_y{dim_y.getSize()};
  if (size_dim_x < 2 || size_dim_y < 2) {
    return false;
  }

  // Check that variables exist
  netCDF::NcVar var_x{file.getVar("x")};
  netCDF::NcVar var_y{file.getVar("y")};
  netCDF::NcVar var_z{file.getVar("z")};
  if (var_x.isNull() || var_y.isNull() || var_z.isNull()) {
    return false;
  }

  // Check that coordinate variables are strictly monotonous and increasing
  std::vector<float> x_values(size_dim_x);
  var_x.getVar(x_values.data());
  for (std::size_t i{1}; i < size_dim_x; ++i) {
    if (x_values[i - 1] >= x_values[i]) {
      return false;
    }
  }

  std::vector<float> y_values(size_dim_y);
  var_y.getVar(y_values.data());
  for (std::size_t i{1}; i < size_dim_y; ++i) {
    if (y_values[i - 1] >= y_values[i]) {
      return false;
    }
  }

  // All checks passed
  return true;
}

#endif  // SCENARIO_H
