#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ranges = std::ranges;

class Hand {
private:
  std::unordered_map<char, unsigned> cards_values_{
      {'2', 1}, {'3', 2}, {'4', 3},  {'5', 4},  {'6', 5},  {'7', 6}, {'8', 7},
      {'9', 8}, {'T', 9}, {'J', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};
  std::string cards_;
  unsigned bid_;
  unsigned base_value_;

  unsigned calcBaseValue() const {
    std::unordered_map<char, unsigned> occurences;
    for (char const &chr : cards_) {
      occurences[chr] += 1;
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 5; })) {
      return 7;
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 4; })) {
      return 6;
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 3; }) &&
        ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 2; })) {
      return 5;
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 3; })) {
      return 4;
    }
    if (ranges::count_if(occurences, [](const auto &pair) {
          return pair.second == 2;
        }) == 2) {
      return 3;
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 2; })) {
      return 2;
    }

    return 1;
  }

public:
  Hand(std::string cards, unsigned bid) : cards_(cards), bid_(bid) {
    base_value_ = calcBaseValue();
  }

  bool operator<(Hand const &other) const {
    if (base_value_ < other.base_value_) {
      return true;
    } else if (base_value_ > other.base_value_) {
      return false;
    } else {
      for (size_t i = 0; i < 5; ++i) {
        unsigned lhs = cards_values_.at(cards_[i]);
        unsigned rhs = cards_values_.at(other.cards_[i]);
        if (lhs < rhs) {
          return true;
        } else if (lhs > rhs) {
          return false;
        }
      }
      // Impossible variant if all hands are different form each other
      return false;
    }
  }
  unsigned bid() const { return bid_; }
};

int main() {
  std::ifstream file("input.txt");
  if (!file.is_open()) {
    std::cerr << "Error opening file\n";
  }

  std::vector<Hand> hands;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string cards, bid;
    iss >> cards >> bid;
    hands.push_back(Hand(cards, std::stoul(bid)));
  }

  std::sort(hands.begin(), hands.end());

  unsigned long long result = 0;

  for (size_t i = 0; i < hands.size(); ++i) {
    result += (i + 1) * hands[i].bid();
  }

  std::cout << result << '\n';
}
