#include <fstream>
#include <iostream>
#include <string>

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

  unsigned const accumulated() { return accumulator_; }
};

int main() {
  std::ifstream file("input.txt");
  CalibrationValue code;

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      for (char ch : line) {
        if (ch >= '0' && ch <= '9') {
          code.update_state(static_cast<unsigned>(ch - '0'));
          // std::cout << ch << '\n';
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
