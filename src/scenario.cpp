#include "scenario.h"

#include <array>
#include <cmath>
#include <cstddef>
#include <netcdf>
#include <vector>

file_scenario file_scenario::create(const std::string &bathy_name, const std::string &displ_name) {
    // Open bathymetry file
    netCDF::NcFile file{bathy_name, netCDF::NcFile::read};

    // Get number of cells in each dimension
    const std::array<std::size_t, 2> num_b{file.getDim("x").getSize(), file.getDim("y").getSize()};

    // Get values
    std::vector<float> b(num_b[0] * num_b[1]);
    file.getVar("z").getVar(b.data());

    // Get cell sizes in each direction
    std::array<float, 4> tmp{};
    file.getVar("x").getVar({0}, {2}, tmp.data());
    file.getVar("y").getVar({0}, {2}, tmp.data() + 2);
    const std::array<float, 2> size_b{tmp[1] - tmp[0], tmp[3] - tmp[2]};

    // Get origin location
    const std::array<float, 2> orig_b{tmp[0] - .5F * size_b[0], tmp[2] - .5F * size_b[1]};

    // Open displacement file
    file.open(displ_name, netCDF::NcFile::read);

    // Get number of cells in each dimension
    const std::array<std::size_t, 2> num_d{file.getDim("x").getSize(), file.getDim("y").getSize()};

    // Get values
    std::vector<float> d(num_d[0] * num_d[1]);
    file.getVar("z").getVar(d.data());

    // Get cell sizes in each direction
    file.getVar("x").getVar({0}, {2}, tmp.data());
    file.getVar("y").getVar({0}, {2}, tmp.data() + 2);
    const std::array<float, 2> size_d{tmp[1] - tmp[0], tmp[3] - tmp[2]};

    // Get origin location
    const std::array<float, 2> orig_d{tmp[0] - .5F * size_b[0], tmp[2] - .5F * size_b[1]};

    // Create and return new scenario
    return file_scenario{num_b, b, size_b, orig_b, num_d, d, size_d, orig_d};
}

file_scenario::file_scenario(const std::array<std::size_t, 2> &num_b,
                             std::vector<float> &b,
                             const std::array<float, 2> &size_b,
                             const std::array<float, 2> &orig_b,
                             const std::array<std::size_t, 2> &num_d,
                             std::vector<float> &d,
                             const std::array<float, 2> &size_d,
                             const std::array<float, 2> &orig_d)
        : num_b{num_b},
          b{std::move(b)},
          size_b{size_b},
          orig_b{orig_b},
          num_d{num_d},
          d{std::move(d)},
          size_d{size_d},
          orig_d{orig_d} {
    if (orig_d[0] < orig_b[0] || orig_d[0] + size_d[0] * num_d[0] > orig_b[0] + size_b[0] * num_b[0] ||
        orig_d[1] < orig_b[1] || orig_d[1] + size_d[1] * num_d[1] > orig_b[1] + size_b[1] * num_b[1]) {
        throw std::runtime_error("Displacement outside of bathymetry!");
    }
}

std::size_t file_scenario::get_index(const float &x,
                                     const float &y,
                                     const std::size_t &num_x,
                                     const std::array<float, 2> &cell_size,
                                     const std::array<float, 2> &origin) {
    return static_cast<std::size_t>((y - origin[1]) / cell_size[1]) * num_x +
           static_cast<std::size_t>((x - origin[0]) / cell_size[0]);
}

std::array<float, 2> file_scenario::get_origin() const { return orig_b; }

std::array<float, 2> file_scenario::get_size() const { return {num_b[0] * size_b[0], num_b[1] * size_b[1]}; }

float file_scenario::get_bathymetry(const float &x, const float &y) const {
    if (x < orig_b[0] || x >= orig_b[0] + size_b[0] * num_b[0] || y < orig_b[1] ||
        y >= orig_b[1] + size_b[1] * num_b[1]) {
        throw std::logic_error("Index outside scenario bounds!");
    }
    float out{b.at(get_index(x, y, num_b[0], size_b, orig_b)) +
              ((x < orig_d[0] || x >= orig_d[0] + size_d[0] * num_d[0] || y < orig_d[1] ||
                y >= orig_d[1] + size_d[1] * num_d[1]) ? 0.F : d.at(get_index(x, y, num_d[0], size_d, orig_d)))};
    return out < 0 ? std::min<float>(out, -20.F) : std::max<float>(out, 20.F);
}

float file_scenario::get_height(const float &x, const float &y) const {
    if (x < orig_b[0] || x >= orig_b[0] + size_b[0] * num_b[0] || y < orig_b[1] ||
        y >= orig_b[1] + size_b[1] * num_b[1]) {
        throw std::logic_error("Index outside scenario bounds!");
    }
    return -std::min<float>(b.at(get_index(x, y, num_b[0], size_b, orig_b)), 0.F);
}

std::array<float, 2> artificial_tsunami_scenario::get_origin() const { return {-5000.F, -5000.F}; }

std::array<float, 2> artificial_tsunami_scenario::get_size() const { return {10000.F, 10000.F}; }

float artificial_tsunami_scenario::get_bathymetry(const float &x, const float &y) const {
    return -100.F + (x >= -500.F && x <= 500.F && y >= -500.F && y <= 500.F ? 5.F * std::sin(
            (x / 500.F + 1.F) * static_cast<float>(M_PI)) * (-(y * y / 250000.F) + 1.F) : 0.F);
}

float artificial_tsunami_scenario::get_height(const float &x, const float &y) const { return 100.F; }

std::array<float, 2> radial_dambreak_obstacle_scenario::get_origin() const { return {0.F, 0.F}; }

std::array<float, 2> radial_dambreak_obstacle_scenario::get_size() const { return {1000.F, 1000.F}; }

float radial_dambreak_obstacle_scenario::get_bathymetry(const float &x, const float &y) const {
    return 725.F <= x && x < 775.F && 475.F <= y && y < 525.F ? 20.F : -20.F;
}

float radial_dambreak_obstacle_scenario::get_height(const float &x, const float &y) const {
    if (get_bathymetry(x, y) >= 0.F) { return 0.F; }
    return (std::sqrt((x - 500.F) * (x - 500.F) + (y - 500.F) * (y - 500.F)) < 100.F) ? 40.F : 20.F;
}
