#ifndef SOLVER_H
#define SOLVER_H

#include <array>
#include <cmath>
#include <stdexcept>

/**
 * Calculates updates and maximum absolute wave speed.
 * @param in Array [b_l, h_l, hu_l, b_r, h_r, hu_r] b must be negative, h must be positive
 * @return Array [delta_h_left, delta_hu_left, delta_h_right, delta_hu_right, max absolute wave speed]
 */
static inline std::array<float, 5> solve(const std::array<float, 6>& in) {
  // We do not support dry cells in the solver. These must be handled by the caller beforehand.
  if (!(in[0] < 0 && in[3] < 0 && in[1] > 0 && in[4] > 0)) {
    throw std::runtime_error("Positive bathymetry and/or negative water height in solver!");
  }

  // Velocity u
  const auto u_l{in[2] / in[1]};
  const auto u_r{in[5] / in[4]};

  // Square roots of the heights, store to minimize calculations
  const auto sqrt_h_l{std::sqrt(in[1])};
  const auto sqrt_h_r{std::sqrt(in[4])};

  // height h^Roe and particle velocity u^Roe
  const auto h_roe{.5F * (in[1] + in[4])};
  const auto u_roe{(u_l * sqrt_h_l + u_r * sqrt_h_r) / (sqrt_h_l + sqrt_h_r)};

  // Gravity of Earth
  static constexpr auto g{9.80665F};

  // Square root of gravity * h^Roe
  const auto sqrt_g_h_roe{std::sqrt(g * h_roe)};

  // Wave speeds, aka Roe eigenvalues
  const auto lambda_1{u_roe - sqrt_g_h_roe};
  const auto lambda_2{u_roe + sqrt_g_h_roe};

  // Early return, if both cells have the same values in them.
  if (in[0] == in[3] && in[1] == in[4] && in[2] == in[5]) {
    return {0.F, 0.F, 0.F, 0.F, in[2] < 0 ? -lambda_1 : lambda_2};
  }

  // Difference between wave speeds
  const auto delta_lambda{lambda_2 - lambda_1};

  // Inverse of the matrix of eigenvectors  |   1        1    |
  //                                        |lambda_1 lambda_2|
  const std::array<float, 4> inverse{lambda_2 / delta_lambda,
                                     -1.F / delta_lambda,
                                     -lambda_1 / delta_lambda,
                                     1.F / delta_lambda};

  // Flux function difference, including effects of bathymetry
  const std::array<float, 2> delta_flux_bathy{in[5] - in[2],
                                              in[5] * u_r - in[2] * u_l +
                                              g * (.5F * (in[4] * in[4] - in[1] * in[1]) + (in[3] - in[0]) * h_roe)};

  // Eigencoefficients
  const std::array<float, 2> alpha{inverse[0] * delta_flux_bathy[0] + inverse[1] * delta_flux_bathy[1],
                                   inverse[2] * delta_flux_bathy[0] + inverse[3] * delta_flux_bathy[1]};

  // Waves
  const std::array<float, 4> z{alpha[0], alpha[0] * lambda_1, alpha[1], alpha[1] * lambda_2};

  // Return net updates and wave speed
  if (lambda_1 > 0.F) {
    return {0.F, 0.F, z[0] + z[2], z[1] + z[3], lambda_2};
  } else if (lambda_2 < 0.F) {
    return {z[0] + z[2], z[1] + z[3], 0.F, 0.F, -lambda_1};
  } else {
    return {z[0], z[1], z[2], z[3], std::max<float>(-lambda_1, lambda_2)};
  }
}

#endif  // SOLVER_H
