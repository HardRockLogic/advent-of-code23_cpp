#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#define LOG_PAIR(x) std::cout << x.first << ", " << x.second << ";\n"

using pair = std::pair<unsigned long long, unsigned long long>;
using ull = unsigned long long;

void fillInput(pair &time_dist, std::ifstream &file) {
  std::string line;
  std::getline(file, line);
  std::istringstream iss(line);
  std::string token;
  iss >> token;
  std::string holder;
  while (iss >> token) {
    std::for_each(token.begin(), token.end(),
                  [&](auto c) { holder.push_back(c); });
  }
  time_dist.first = std::stoull(holder);
  holder.clear();
  std::getline(file, line);
  iss = std::istringstream(line);
  iss >> token;
  while (iss >> token) {
    std::for_each(token.begin(), token.end(),
                  [&](auto c) { holder.push_back(c); });
  }
  time_dist.second = std::stoull(holder);
}

ull countBests(pair const &time_dist) {
  ull counter = 0;
  ull ms = time_dist.first;
  ull mm = time_dist.second;
  for (ull j = 0; j <= ms; ++j) {
    ull res = j * (ms - j);
    if (res > mm) {
      ++counter;
    }
  }
  return counter;
}

int main() {
  std::ifstream file("input.txt");
  if (!file.is_open()) {
    std::cerr << "Error reading file." << std::endl;
    return 1;
  }
  pair time_dist;
  fillInput(time_dist, file);
  LOG_PAIR(time_dist);
  std::cout << countBests(time_dist) << '\n';
}
