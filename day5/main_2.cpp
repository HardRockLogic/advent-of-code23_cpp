#include <array>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef DEBUG
#define LOG_VEC(x)                                                             \
  for (auto const &elem : x) {                                                 \
    std::cout << elem << '\n';                                                 \
  }
#else
#define LOG_VEC(x)
#endif

using u64_t = unsigned long long;

class Range {
private:
  u64_t start_;
  u64_t edge_;
  bool mapped_ = false;

public:
  Range() = default;
  Range(u64_t start, u64_t edge, bool mapped)
      : start_(start), edge_(edge), mapped_(mapped){};

  void reset() { mapped_ = false; }
  u64_t start() const { return start_; }
  u64_t edge() const { return edge_; }
  bool mapped() const { return mapped_; }

  // debug
  friend std::ostream &operator<<(std::ostream &os, const Range &range) {
    os << "start: " << range.start_ << ", edge: " << range.edge_;
    return os;
  }
};

class DestSrcRange {
private:
  long long int diff_;

  // Destination -> Source
  Range dest_;
  Range src_;

public:
  DestSrcRange(u64_t src, u64_t dest, u64_t length) {
    dest_ = Range(dest, dest + length - 1, false);
    src_ = Range(src, src + length - 1, false);
    diff_ = dest - src;
  }

  bool in_range(Range range) const {
    return (range.start() >= src_.start() && range.edge() <= src_.edge())
               ? true
               : false;
  }

  std::vector<Range> get_mapped_value(Range range) const {
    std::vector<Range> out;

    // fully fits the range
    if (in_range(range)) {
      out.emplace_back(range.start() + diff_, range.edge() + diff_, true);
      return out;

      // chopped left (intersects on right)
    } else if (range.start() < src_.start() && range.edge() <= src_.edge() &&
               !(range.edge() < src_.start())) {
      out.emplace_back(range.start(), src_.start() - 1, false);
      out.emplace_back(dest_.start(), range.edge() + diff_, true);
      return out;

      // chopped right (intersects on left)
    } else if (range.start() >= src_.start() && range.edge() > src_.edge() &&
               !(range.start() > src_.edge())) {
      out.emplace_back(range.start() + diff_, dest_.edge(), true);
      out.emplace_back(src_.edge() + 1, range.edge(), false);
      return out;

      // source range fully fits input range
    } else if (range.start() < src_.start() && range.edge() > src_.edge()) {
      out.emplace_back(dest_.start(), dest_.edge(), true);
      out.emplace_back(range.start(), src_.start() - 1, false);
      out.emplace_back(src_.edge() + 1, range.edge(), false);
      return out;

      // input range doesnt belong to source range
    } else {
      out.push_back(range);
      return out;
    }
  }
};

// clang-format off
void drainToFrom(std::vector<Range> &to, std::vector<Range> &from) {
  to.insert(to.end(), std::make_move_iterator(from.begin()),
                      std::make_move_iterator(from.end()));
}
// clang-format on

class GenericMap {
private:
  std::vector<DestSrcRange> list_of_ranges_;

public:
  void push_range(DestSrcRange range) { list_of_ranges_.push_back(range); }

  bool is_valid() const { return list_of_ranges_.empty() ? false : true; }

  std::vector<Range> search_in_ranges(std::vector<Range> outer_ranges) const {

    std::vector<Range> unmapped;
    drainToFrom(unmapped, outer_ranges);
    std::vector<Range> output;
    std::vector<Range> mapped;

    for (DestSrcRange const &local_range : list_of_ranges_) {
      for (auto const &input : unmapped) {
        auto tempo = local_range.get_mapped_value(input);
        // LOG_VEC(tempo)
        drainToFrom(output, tempo);
      }
      unmapped.clear();

      for (auto &recieved : output) {
        if (recieved.mapped()) {
          recieved.reset();
          mapped.push_back(recieved);

        } else {
          unmapped.push_back(recieved);
        }
      }
      output.clear();
    }

    drainToFrom(output, unmapped);
    drainToFrom(output, mapped);

    return output;
  }
};

class MapAggregator {
public:
  std::vector<Range> seeds_;

private:
  std::array<GenericMap, 7> maps_;
  unsigned short map_id_ = 0;

  // hidning implementation details
  std::vector<Range> seed_to_location(Range seeds) {
    std::vector<Range> active_value = {seeds};
    for (GenericMap const &map : maps_) {
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
    std::vector<Range> collection;

    for (Range const &seed : seeds_) {
      // std::vector<Range> tempo = seed_to_location(seed);
      for (auto const &recieved : seed_to_location(seed)) {
        collection.push_back(recieved);
      }
    }
    for (auto const &finals : collection) {
      // std::cout << finals << ", ";
      if (finals.start() < lowest) {
        lowest = finals.start();
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

  while (true) {
    std::string len;
    try {
      iss >> token >> len;
      aggregator.seeds_.emplace_back(
          std::stoull(token), std::stoull(token) + std::stoull(len) - 1, false);
    } catch (const std::invalid_argument &) {
      // std::cout << "invalid arg met\n";
      break;
    }
  }

  LOG_VEC(aggregator.seeds_)

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
    }
  }

  // part1: 240320250; part2: 28580589;
  fmt::print("{}\n", aggregator.lowest_location());
}
