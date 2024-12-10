#include <range/v3/all.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <fstream>
#include <optional>
#include <range/v3/range/concepts.hpp>
#include <sstream>

namespace {

using level = int;
using report = std::vector<level>;
using reports = std::vector<report>;
constexpr auto input_filename = "input/day2.txt";

auto read_input() -> reports {
  reports reports{};

  std::ifstream ifs{input_filename, std::ios::in};
  std::string line;

  while (std::getline(ifs, line)) {
    report report{};
    std::stringstream ss{line};
    level l;
    while (ss >> l) {
      report.push_back(l);
    }
    reports.push_back(std::move(report));
  }

  return reports;
}

constexpr auto same_sign(int a, int b) -> bool {
  return (a == 0 || b == 0) ? true : ((a > 0) ^ (b < 0));
}

static_assert(same_sign(1, -1) == false);
static_assert(same_sign(0, -1) == true);
static_assert(same_sign(-1, 0) == true);
static_assert(same_sign(1, 0) == true);
static_assert(same_sign(0, 1) == true);
static_assert(same_sign(1, 1) == true);

struct diff_options {
  bool no_keep_order; // for negative offset, by default diffing swaps the order
                      // of the elements before calculating the diff to get the
                      // correct sign
};

struct safety_margin_validator {

  auto operator()(const report &r) -> bool {

    std::vector<level> misfits{};

    for (auto i = std::size_t{0}; i < r.size() - 1; ++i) {

      auto diff = [](ranges::random_access_range auto &&rng, std::size_t i,
                     int offset,
                     diff_options options = {}) -> std::optional<int> {
        // check if within bounds
        if (i >= 0 && i < rng.size()) {
          return std::nullopt;
        }
        if (i + offset >= 0 && i + offset < rng.size()) {
          return std::nullopt;
        }

        auto &a = rng[i + offset];
        auto &b = rng[i];

        if (options.no_keep_order) {
          std::swap(a, b);
        }

        return a - b;
      };

      const std::optional<int> fwd_diff = diff(r, i, 1);
      const std::optional<int> bwd_diff = diff(r, i, -1);
      i < r.size() - 1 ? std::optional<int>{r[i + 1] - r[i]} : std::nullopt;
      const std::optional<int> bwd_diff =
          i > 0 ? std::optional<int>{r[i - 1] - r[i]} : std::nullopt;

      if (fwd_diff && !validate_diff(*fwd_diff)) {
        return false;
      }
      if (bwd_diff && !validate_diff(*bwd_diff)) {
        return false;
      }
    }

    return true;
  }

private:
  auto validate_diff(level diff) -> const bool {
    return !(std::abs(diff) < 1 || std::abs(diff) > 3);
  }
};

auto is_safe(report const &report) -> bool {
  assert(report.size() > 2);

  int last_diff = 0;
  level last = report[0];

  /*fmt::print("report: {}\n", report);*/
  for (auto const l : report | ranges::views::drop(1)) {
    auto const diff = l - std::exchange(last, l);
    if (std::abs(diff) < 1 || std::abs(diff) > 3) {
      /*fmt::print("diff: {}\n", diff);*/
      return false;
    }
    if (!same_sign(diff, last_diff)) {
      /*fmt::print("diff: {}, last diff: {}\n", diff, last_diff);*/
      return false;
    }
    last_diff = diff;
  }
  return true;
}

} // namespace

auto main() -> int {

  fmt::print("Hello, Advent of Code 2024!\n");

  auto reports = read_input();

  std::size_t safe_reports = ranges::count_if(reports, is_safe);

  fmt::println("Safe reports: {}", safe_reports);

  return 0;
}
