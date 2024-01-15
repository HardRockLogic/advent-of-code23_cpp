#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class CalibrationValue {
private:
  bool first_encounter_ = false;
  unsigned first_value_;
  unsigned last_value_;
  unsigned accumulator_ = 0;

public:
  void update_state(unsigned value) noexcept {
    if (!first_encounter_) {
      first_value_ = value;
      last_value_ = value;
      first_encounter_ = true;

    } else {
      last_value_ = value;
    }
  }

  void merge() noexcept {
    accumulator_ += first_value_ * 10 + last_value_;
    first_encounter_ = false;
  }

  unsigned accumulated() const { return accumulator_; }
};

class CalibrationAccumulator {
private:
  unsigned accumulator_ = 0;

public:
  void merge(unsigned short first, unsigned short last) noexcept {
    unsigned tempo = first * 10 + last;
    accumulator_ += tempo;
  }

  unsigned accumulated() const { return accumulator_; }
};

void first_part() {
  std::ifstream file("input.txt");
  CalibrationValue code;

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      for (char ch : line) {
        if (ch >= '0' && ch <= '9') {
          code.update_state(static_cast<unsigned>(ch - '0'));
        }
      }
      code.merge();
    }
    file.close();

  } else {
    std::cerr << "unable to open file\n";
  }

  std::cout << code.accumulated() << "\n";
}

void second_part() {
  std::unordered_map<std::string, unsigned short> values{
      {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
      {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

  std::ifstream file("input.txt");
  CalibrationAccumulator code;

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::pair<size_t, unsigned short> lowestFirst{std::string::npos, 10};
      std::pair<size_t, unsigned short> highesLast{0, 10};

      for (auto const &[sample, value] : values) {
        size_t foundFirst = line.find(sample);
        if (foundFirst != std::string::npos && foundFirst < lowestFirst.first) {
          lowestFirst = {foundFirst, value};
        }

        size_t foundLast = line.rfind(sample);
        if (foundLast != std::string::npos && foundLast > highesLast.first) {
          highesLast = {foundLast, value};
        }
      }

      for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
          if (i < lowestFirst.first) {
            lowestFirst = {i, static_cast<unsigned short>(line[i] - '0')};
          }
          if (i >= highesLast.first) {
            highesLast = {i, static_cast<unsigned short>(line[i] - '0')};
          }
        }
      }
      code.merge(lowestFirst.second, highesLast.second);
    }
    file.close();
  } else {
    std::cerr << "unable to open\n";
  }

  std::cout << code.accumulated() << '\n';
}

int main() {
  first_part();

  std::cout << "-----second part------\n";

  second_part();
}
