#include <array>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#define LOG_VEC(x)                                                             \
  for (auto const &element : x) {                                              \
    std::cout << element << "; ";                                              \
  }                                                                            \
  std::cout << '\n'

using vec_u32 = std::vector<unsigned>;

void fillInput(vec_u32 &mills, vec_u32 &milimiters, std::ifstream &file) {
  std::string line;
  std::getline(file, line);
  std::istringstream iss(line);
  std::string token;
  iss >> token;
  while (iss >> token) {
    mills.push_back(std::stoul(token));
  }
  std::getline(file, line);
  iss = std::istringstream(line);
  iss >> token;
  while (iss >> token) {
    milimiters.push_back(std::stoul(token));
  }
}

unsigned countBests(vec_u32 const &mills, vec_u32 const &milimiters) {
  if (mills.size() > 4 || milimiters.size() > 4) {
    std::cerr << "more elements than expected\n";
    return 0;
  }
  std::array<unsigned, 4> results;
  for (size_t i = 0; i < mills.size(); ++i) {
    unsigned counter = 0;
    unsigned ms = mills[i];
    unsigned mm = milimiters[i];
    for (unsigned j = 0; j <= ms; ++j) {
      auto res = j * (ms - j);
      if (res > mm) {
        ++counter;
      }
    }
    results[i] = counter;
  }
  return std::reduce(results.begin(), results.end(), 1, std::multiplies<>{});
}

int main() {
  std::ifstream file("input.txt");
  if (!file.is_open()) {
    std::cerr << "Error reading file." << std::endl;
    return 1;
  }
  vec_u32 mills;
  vec_u32 milimiters;
  fillInput(mills, milimiters, file);
  LOG_VEC(mills);
  LOG_VEC(milimiters);
  std::cout << countBests(mills, milimiters) << '\n';
}
