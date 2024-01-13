#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

int main() {
  std::ifstream file("input.txt");

  std::regex pattern(R"((\d+)\s*([a-zA-Z]+))");

  unsigned accumulator = 0;
  unsigned counter = 0;

  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {
      ++counter;
      // std::cout << counter << '\n';
      // std::vector<std::pair<unsigned short, std::string>> matches;
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
        // matches.emplace_back(number_cubes, color);
      }
      if (adding_condition)
        accumulator += counter;
      // for (auto const &[number, color] : matches) {
      //   std::cout << number << ": " << color << "\n";
      //   if (color == "red" && number > 12) {
      //     break;
      //   } else if (color == "green" && number > 13) {
      //     break;
      //   } else if (color == "blue" && number > 14) {
      //     break;
      //   }
      // }
    }
  } else {
    std::cerr << "error opening file\n";
  }

  std::cout << accumulator << "\n";
}
