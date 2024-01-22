#include <array>
#include <cstddef>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using u64_t = unsigned long long;

class DestSrcRange {
private:
  u64_t src_;
  u64_t dest_;
  u64_t length_;
  long long int diff_;

public:
  DestSrcRange(u64_t src, u64_t dest, u64_t length)
      : src_(src), dest_(dest), length_(length) {
    diff_ = dest_ - src_;
  }

  bool in_range(u64_t value) const {
    // fmt::print("val:{}, srcStart:{}, stcEnd:{}\n", value, src_, src_ +
    // length_);
    return (value >= src_ && value <= src_ + length_ - 1) ? true : false;
  }

  u64_t get_mapped_value(u64_t value) const {
    // just returning value + diff is sufficient as checking against range
    // handled in GenericMap
    if (in_range(value)) {
      // std::cout << "happenst check\n";
      // return value + diff_;
      return dest_ + (value - src_);
    } else {
      return value;
    }
  }
};

class GenericMap {
private:
  std::vector<DestSrcRange> list_of_ranges_;

public:
  void push_range(DestSrcRange range) { list_of_ranges_.push_back(range); }

  bool is_valid() const { return list_of_ranges_.empty() ? false : true; }

  // debug method
  size_t get_size() const { return list_of_ranges_.size(); }

  u64_t search_in_ranges(u64_t value) const {
    for (DestSrcRange const &range : list_of_ranges_) {

      if (range.in_range(value)) {
        return range.get_mapped_value(value);
      }
    }
    // if value doesnt match any range, returning value itself
    return value;
  }
};

class MapAggregator {
public:
  std::vector<u64_t> seeds_;

private:
  std::array<GenericMap, 7> maps_;
  unsigned short map_id_ = 0;

  // hidning implementation details
  u64_t seed_to_location(u64_t seed) {
    u64_t active_value = seed;
    for (GenericMap const &map : maps_) {
      // fmt::print("#{}, ", map.get_size());
      active_value = map.search_in_ranges(active_value);
    }
    return active_value;
  }

public:
  void push_map(GenericMap map) {
    if (map_id_ > 6) {
      std::cerr << "only 7 maps allowed\n";
    } else if (map.is_valid()) {
      maps_[map_id_] = map;
      ++map_id_;
    }
  }

  u64_t lowest_location() {
    u64_t lowest = std::numeric_limits<u64_t>::max();

    for (u64_t const &seed : seeds_) {
      u64_t tempo = seed_to_location(seed);
      if (tempo < lowest) {
        lowest = tempo;
      }
    }
    return lowest;
  }
};

int main() {
  std::ifstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "error reading file";
    return 1;
  }

  std::ostringstream file_content;
  file_content << file.rdbuf();
  std::istringstream iss(file_content.str());

  MapAggregator aggregator;

  std::string token;
  iss >> token;
  // fmt::print("{}\n", token);

  while (true) {
    try {
      iss >> token;
      aggregator.seeds_.push_back(std::stoll(token));
    } catch (const std::invalid_argument &) {
      // std::cout << "invalid arg met\n";
      break;
    }
  }
  fmt::print("{}\n", aggregator.seeds_);

  GenericMap map;
  while (true) {
    if (iss.eof()) {
      aggregator.push_map(map);
      break;
    }

    iss >> token;
    try {
      u64_t dest = std::stoull(token);
      iss >> token;
      u64_t src = std::stoull(token);
      iss >> token;
      u64_t length = std::stoull(token);
      DestSrcRange range(src, dest, length);
      map.push_range(range);
    } catch (const std::invalid_argument &) {
      aggregator.push_map(map);
      map = GenericMap();
      // fmt::print("catch: {}", token);
    }
  }

  fmt::print("{}\n", aggregator.lowest_location());
}
