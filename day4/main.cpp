#include <cstdlib>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ios>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

class ScratchCard {
private:
  std::vector<unsigned> left_part_;
  std::vector<unsigned> right_part_;
  unsigned short count_state_ = 1;

public:
  void push_number(unsigned number) {
    if (count_state_ <= 5) {
      left_part_.push_back(number);
    } else {
      right_part_.push_back(number);
    }

    ++count_state_;
  }

  void show_res_debug() {
    fmt::print("{} \n---------\n", left_part_);
    fmt::print("{} \n", right_part_);
  }
};

int main() {
  std::string s = "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53";

  auto result = s | std::views::split(' ') | std::views::drop(2);
  fmt::print("{} \n", fmt::join(result, " "));

  ScratchCard card;
  for (auto const &token : result) {
    if (token.size() == 0 || token[0] == '|')
      continue;
    std::string tempo;
    if (token.size() == 2) {
      tempo.push_back(token[0]);
      tempo.push_back(token[1]);
    } else {
      tempo.push_back(token[0]);
    }
    card.push_number(std::stoul(tempo));
  }

  card.show_res_debug();
}
