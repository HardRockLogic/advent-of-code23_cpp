#include <algorithm>
#include <cmath>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

class ScratchCardP1 {
private:
  std::vector<unsigned> left_part_;
  std::vector<unsigned> right_part_;
  unsigned short count_state_ = 1;
  unsigned acc_ = 0;

public:
  void push_number(unsigned number) {
    if (count_state_ <= 10) {
      left_part_.push_back(number);
    } else {
      right_part_.push_back(number);
    }

    ++count_state_;
  }

  void check_numbers() {
    unsigned multiplier = 0;

    for (unsigned const your_number : left_part_) {
      auto result =
          std::find(right_part_.begin(), right_part_.end(), your_number);
      if (result != right_part_.end()) {
        ++multiplier;
      }
    }
    // evaluating nth term of the geometric progression with first term=1 and
    // commin Ratio=2
    if (multiplier) {
      unsigned tempo = std::pow(2, multiplier - 1);
      acc_ += tempo;
    }
    count_state_ = 1;
    left_part_.clear();
    right_part_.clear();
  }

  unsigned accumulated() const { return acc_; }

  void show_res_debug() {
    fmt::print("{} \n---------\n", left_part_);
    fmt::print("{} \n", right_part_);
  }
};

class ScratchCardP2 {
private:
  std::vector<unsigned> left_part_;
  std::vector<unsigned> right_part_;
  std::unordered_map<unsigned, unsigned> cards_data;
  unsigned card_id = 0;
  unsigned short count_state_ = 1;
  unsigned acc_ = 0;

  [[nodiscard]] std::vector<unsigned>
  produce_duplicats(std::vector<unsigned> input) {
    std::vector<unsigned> output;

    for (unsigned const card : input) {
      unsigned res = cards_data[card];

      for (unsigned i = card + 1; i <= card + res; ++i) {
        output.push_back(i);
      }
    }

    return output;
  }

public:
  void push_number(unsigned number) {
    if (count_state_ <= 10) {
      left_part_.push_back(number);
    } else {
      right_part_.push_back(number);
    }

    ++count_state_;
  }

  void check_numbers() {
    unsigned multiplier = 0;
    ++card_id;

    for (unsigned const your_number : left_part_) {
      auto result =
          std::find(right_part_.begin(), right_part_.end(), your_number);
      if (result != right_part_.end()) {
        ++multiplier;
      }
    }

    cards_data[card_id] = multiplier;

    count_state_ = 1;
    left_part_.clear();
    right_part_.clear();
  }

  void count_cards() {
    std::vector<unsigned> cards_iteration(cards_data.size());
    std::iota(cards_iteration.begin(), cards_iteration.end(), 1);

    while (cards_iteration.size()) {
      acc_ += cards_iteration.size();
      cards_iteration = this->produce_duplicats(cards_iteration);
    }
  }

  unsigned accumulated() const { return acc_; }

  void show_res_debug() {
    fmt::print("{} \n---------\n", left_part_);
    fmt::print("{} \n", right_part_);
  }
};

int main() {
  // std::string s = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53";
  std::ifstream file("input.txt");
  ScratchCardP2 card;

  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {

      auto const is_empty = [](auto x) {
        return (x.size() == 0 || x[0] == '|') ? false : true;
      };

      // clang-format off
      auto result = line 
        | std::views::split(' ') 
        | std::views::filter(is_empty) 
        | std::views::drop(2);
      // clang-format on

      for (auto const &token : result) {

        std::string tempo;
        if (token.size() == 2) {
          tempo.push_back(token[0]);
          tempo.push_back(token[1]);
        } else {
          tempo.push_back(token[0]);
        }

        card.push_number(std::stoul(tempo));
      }
      // card.show_res_debug();
      card.check_numbers();
    }

  } else {
    std::cerr << "error opening file\n";
  }

  // this method is only necessary for part 2
  card.count_cards();

  // part2 asnwer: 6874754
  fmt::print("Part #: {}\n", card.accumulated());
}
