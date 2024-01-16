#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Coord {
  unsigned x;
  unsigned y;

  bool operator==(const Coord &other) const {
    return (x == other.x) && (y == other.y);
  }
};

struct CoordHash {
  size_t operator()(const Coord &s) const {
    return std::hash<unsigned>()(s.x) ^ std::hash<unsigned>()(s.y);
  }
};

class EngineSchematicGridP1 {
private:
  std::vector<Coord> symbols_;
  std::vector<Coord> current_number_coords_;
  std::string current_number_;
  unsigned long acc_;

public:
  EngineSchematicGridP1(std::ifstream &file)
      : symbols_{}, current_number_coords_{}, current_number_{}, acc_{0} {
    if (file.is_open()) {
      std::string line;
      unsigned raw = 1;

      while (std::getline(file, line)) {
        unsigned column = 1;

        for (char ch : line) {
          if (ch != '.' && !(ch >= '0' && ch <= '9')) {
            symbols_.emplace_back(column, raw);
          }
          ++column;
        }
        ++raw;
      }

      file.clear();
      file.seekg(0, std::ios::beg);

    } else {
      std::cerr << "error reading file\n";
    }
  }

  void check_for_adjecency() {
    std::unordered_set<Coord, CoordHash> adjecent_coords;
    for (size_t i = 0; i < current_number_coords_.size(); ++i) {
      // upper
      adjecent_coords.emplace(current_number_coords_[i].x,
                              current_number_coords_[i].y - 1);
      // bottom
      adjecent_coords.emplace(current_number_coords_[i].x,
                              current_number_coords_[i].y + 1);
      // left
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y);
      // right
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y);
      // upper left diagonal
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y - 1);
      // bottom left diagonal
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y + 1);
      // upper right diagonal
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y - 1);
      // bottom right diagonal
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y + 1);
    }
    for (Coord const &coord : adjecent_coords) {
      auto result = std::find(symbols_.begin(), symbols_.end(), coord);
      if (result != symbols_.end()) {
        acc_ += std::stoul(current_number_);
        break;
      }
    }
    current_number_.clear();
    current_number_coords_.clear();
    current_number_coords_.resize(0);
  };

  void build_number(Coord coord, char chr) {
    if (chr >= '0' && chr <= '9') {
      current_number_.push_back(chr);
      current_number_coords_.push_back(coord);

    } else if (!(chr >= '0' && chr <= '9') || current_number_.size() == 3) {
      check_for_adjecency();
    }
  }

  unsigned accumulated() const { return acc_; }
};

class EngineSchematicGridP2 {
private:
  std::vector<Coord> stars;
  std::unordered_map<Coord, std::pair<unsigned, unsigned>, CoordHash> gears;
  std::vector<Coord> current_number_coords_;
  std::string current_number_;
  unsigned long acc_;

public:
  EngineSchematicGridP2(std::ifstream &file)
      : stars{}, gears{}, current_number_coords_{}, current_number_{}, acc_{0} {
    if (file.is_open()) {
      std::string line;
      unsigned raw = 1;

      while (std::getline(file, line)) {
        unsigned column = 1;

        for (char ch : line) {
          if (ch == '*') {
            stars.emplace_back(column, raw);
          }
          ++column;
        }
        ++raw;
      }

      file.clear();
      file.seekg(0, std::ios::beg);

    } else {
      std::cerr << "error reading file\n";
    }
  }

  void check_for_adjecency() {
    std::unordered_set<Coord, CoordHash> adjecent_coords;
    for (size_t i = 0; i < current_number_coords_.size(); ++i) {
      // upper
      adjecent_coords.emplace(current_number_coords_[i].x,
                              current_number_coords_[i].y - 1);
      // bottom
      adjecent_coords.emplace(current_number_coords_[i].x,
                              current_number_coords_[i].y + 1);
      // left
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y);
      // right
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y);
      // upper left diagonal
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y - 1);
      // bottom left diagonal
      adjecent_coords.emplace(current_number_coords_[i].x - 1,
                              current_number_coords_[i].y + 1);
      // upper right diagonal
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y - 1);
      // bottom right diagonal
      adjecent_coords.emplace(current_number_coords_[i].x + 1,
                              current_number_coords_[i].y + 1);
    }
    for (Coord const &coord : adjecent_coords) {
      auto result = std::find(stars.begin(), stars.end(), coord);

      if (result != stars.end()) {

        if (gears.find(*result) == gears.end()) {
          gears[*result] = std::make_pair(std::stoul(current_number_), 0);
        } else {
          gears[*result].second = std::stoul(current_number_);
        }
        break;
      }
    }
    current_number_.clear();
    current_number_coords_.clear();
    current_number_coords_.resize(0);
  }

  void build_number(Coord coord, char chr) {
    if (chr >= '0' && chr <= '9') {
      current_number_.push_back(chr);
      current_number_coords_.push_back(coord);

    } else if (!(chr >= '0' && chr <= '9') || current_number_.size() == 3) {
      check_for_adjecency();
    }
  }

  unsigned accumulate() {
    for (auto const &[_, gear] : gears) {
      if (gear.second != 0) {
        acc_ += gear.first * gear.second;
      }
    }
    return acc_;
  }
};

// the function is the same for both parts, the only diff is
// EngineSchematicGrid class name (...P1, ...P2)
// and final result method is accumulate not accumulated
void firts_part() {
  std::ifstream file("input.txt");

  EngineSchematicGridP2 solution{file};

  if (file.is_open()) {
    std::string line;
    unsigned raw = 1;

    while (std::getline(file, line)) {
      unsigned column = 1;

      for (char ch : line) {
        solution.build_number(Coord{column, raw}, ch);
        ++column;
      }
      ++raw;
      solution.check_for_adjecency();
    }
  } else {
    std::cerr << "error opening file \n";
  }

  std::cout << solution.accumulate() << '\n';
}

int main() {
  firts_part();

  std::cout << "-----second part------\n";
}
