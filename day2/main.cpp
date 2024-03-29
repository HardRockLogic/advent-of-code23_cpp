#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>

void partOne() {
  std::ifstream file("input.txt");

  std::regex pattern(R"((\d+)\s*([a-zA-Z]+))");

  unsigned accumulator = 0;
  unsigned counter = 0;

  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {
      ++counter;
      bool adding_condition = true;

      auto colors_begin =
          std::sregex_iterator(line.begin(), line.end(), pattern);
      auto colors_end = std::sregex_iterator();

      for (auto iter = colors_begin; iter != colors_end; ++iter) {
        std::smatch match = *iter;
        unsigned short number_cubes = std::stoi(match[1].str());
        std::string color = match[2].str();

        if (color == "red" && number_cubes > 12) {
          adding_condition = false;
          break;
        } else if (color == "green" && number_cubes > 13) {
          adding_condition = false;
          break;
        } else if (color == "blue" && number_cubes > 14) {
          adding_condition = false;
          break;
        }
      }
      if (adding_condition)
        accumulator += counter;
    }
  } else {
    std::cerr << "error opening file\n";
  }

  std::cout << accumulator << "\n";
}

void partTwo() {
  std::ifstream file("input.txt");

  std::regex pattern(R"((\d+)\s*([a-zA-Z]+))");

  unsigned accumulator = 0;
  // unsigned counter = 0;

  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {
      // ++counter;
      std::unordered_map<std::string, unsigned> bag;
      // bool adding_condition = true;

      auto colors_begin =
          std::sregex_iterator(line.begin(), line.end(), pattern);
      auto colors_end = std::sregex_iterator();

      for (auto iter = colors_begin; iter != colors_end; ++iter) {
        std::smatch match = *iter;
        unsigned number_cubes = std::stoi(match[1].str());
        std::string color = match[2].str();

        auto map_entry = bag.find(color);
        if (map_entry == bag.end()) {
          bag[color] = number_cubes;
        } else {
          if (number_cubes > map_entry->second) {
            map_entry->second = number_cubes;
          }
        }
      }
      accumulator += std::accumulate(
          bag.begin(), bag.end(), 1u,
          [](unsigned acc, const std::pair<std::string, unsigned> &pair) {
            return acc * pair.second;
          });
    }
  } else {
    std::cerr << "error opening file\n";
  }

  std::cout << accumulator << "\n";
}

int main() {
  partOne();
  std::cout << "------part two-------\n";
  partTwo();
}
