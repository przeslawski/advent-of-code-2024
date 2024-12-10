#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>

using loc_id = std::size_t;
using loc_list = std::vector<loc_id>;

constexpr auto input_filename = "input/day1.txt";

auto read_input() -> std::pair<loc_list, loc_list> {
  loc_id a, b;

  loc_list left, right{};

  std::ifstream ifs{input_filename, std::ios::in};

  while (ifs >> a >> b) {
    left.push_back(a);
    right.push_back(b);
  }

  return {std::move(left), std::move(right)};
}

auto main() -> int {

  fmt::print("Hello, Advent of Code 2024!\n");

  auto [left, right] = read_input();

  ranges::sort(left);
  ranges::sort(right);

  /// *** Part 1 ***
  /// calculate accumulated distance

  auto acc_distance = std::size_t{0};

  for (auto i = std::size_t{0}; i < left.size(); ++i) {
    acc_distance +=
        left[i] > right[i] ? left[i] - right[i] : right[i] - left[i];
  }

  /// *** Part 2 ***
  /// calculate similarity score

  using count_t = std::size_t;
  std::unordered_map<loc_id, count_t> occurances{};
  for (auto r : right) {
    occurances[r]++;
  }

  auto similarity_score = std::size_t{0};

  for (auto l : left) {
    similarity_score += l * occurances[l];
  }

  fmt::println("Accumulated distance: {}", acc_distance);
  fmt::println("Similarity score: {}", similarity_score);

  return 0;
}
