#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define LOG_PAIR(x) std::cout << x.first << ", " << x.second << ";\n"

namespace ranges = std::ranges;

struct PairHash {
  template <typename T1, typename T2>
  std::size_t operator()(std::pair<T1, T2> const &p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

class Hand {
private:
  std::unordered_map<char, unsigned> cards_values_{
      {'J', 1},  {'2', 2},  {'3', 3},  {'4', 4}, {'5', 5},
      {'6', 6},  {'7', 7},  {'8', 8},  {'9', 9}, {'T', 10},
      {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};
  std::string cards_;
  unsigned bid_;
  unsigned base_value_;

  // Base hand type firts, N of jokers second
  std::pair<unsigned, unsigned> calcBaseValue() const {
    std::unordered_map<char, unsigned> occurences;
    for (char const &chr : cards_) {
      occurences[chr] += 1;
    }
    unsigned jokers = occurences['J'];

    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 5; })) {
      return {7, jokers};
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 4; })) {
      return {6, jokers};
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 3; }) &&
        ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 2; })) {
      return {5, jokers};
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 3; })) {
      return {4, jokers};
    }
    if (ranges::count_if(occurences, [](const auto &pair) {
          return pair.second == 2;
        }) == 2) {
      return {3, jokers};
    }
    if (ranges::any_of(occurences,
                       [](const auto &pair) { return pair.second == 2; })) {
      return {2, jokers};
    }

    return {1, jokers};
  }

  unsigned embedJokers() const {
    std::unordered_map<std::pair<unsigned, unsigned>, unsigned, PairHash>
        varians_of_embedding = {
            {{5, 3}, 7}, {{4, 3}, 6}, {{5, 2}, 7}, {{3, 2}, 6}, {{2, 2}, 4},
            {{6, 1}, 7}, {{4, 1}, 6}, {{2, 1}, 4}, {{3, 1}, 5}, {{1, 1}, 2}};
    std::pair<unsigned, unsigned> before_embedding = calcBaseValue();
    if (before_embedding.second == 0) {
      return before_embedding.first;
    } else if (before_embedding.second == 4 || before_embedding.first == 7) {
      return 7;
    }

    LOG_PAIR(before_embedding);
    return varians_of_embedding.at(before_embedding);
  }

public:
  Hand(std::string cards, unsigned bid) : cards_(cards), bid_(bid) {
    base_value_ = embedJokers();
    // std::cout << base_value_ << '\n';
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
