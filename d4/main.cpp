#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> read_lines_str(const std::string &str) {
  std::vector<std::string> result{};

  std::string_view sv{str};

  while (true) {
    auto newlinePos = sv.find('\n');

    if (newlinePos == sv.npos)
      break;

    auto to_insert = sv.substr(0, newlinePos);
    sv = sv.substr(newlinePos + 1);

    result.emplace_back(std::string{to_insert});
  }

  return result;
}

std::vector<std::string> read_lines(const std::filesystem::path &path) {
  std::vector<std::string> result{};

  std::ifstream fs{path};

  std::string current_line{};
  while (std::getline(fs, current_line)) {
    result.emplace_back(std::move(current_line));
  }

  return result;
}

size_t part1(std::vector<std::string> &map) {
  size_t result = 0;
  const auto width = map[0].size();
  const auto height = map.size();

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      // Check neighbors
      if (map[y][x] != '@')
        continue;

      size_t neighbor_count = 0;

      for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
          const int64_t oy = y + dy;
          const int64_t ox = x + dx;

          if (oy < 0 || ox < 0 || (oy == y && ox == x) || ox >= width ||
              oy >= height)
            continue;
          if (map[oy][ox] == '@') {
            neighbor_count++;
          }
        }
      }

      if (neighbor_count < 4) {
        std::cout << x << ", " << y << map[y][x] << "\n";
        result++;
      }
    }
  }

  return result;
}

std::pair<size_t, std::vector<std::string>>
part2(std::vector<std::string> &map) {
  size_t result = 0;
  const auto width = map[0].size();
  const auto height = map.size();

  std::vector<std::string> after{};

  after.resize(height);
  for (auto &row : after) {
    row.resize(width, '.');
  }

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      // Check neighbors
      if (map[y][x] != '@')
        continue;

      size_t neighbor_count = 0;

      for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
          const int64_t oy = y + dy;
          const int64_t ox = x + dx;

          if (oy < 0 || ox < 0 || (oy == y && ox == x) || ox >= width ||
              oy >= height)
            continue;
          if (map[oy][ox] == '@') {
            neighbor_count++;
          }
        }
      }

      if (neighbor_count < 4) {
        after[y][x] = '.';
        result++;
      } else {
        after[y][x] = '@';
      }
    }
  }

  return std::make_pair(result, std::move(after));
}

void tests() {}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Input please\n";
    abort();
  }

  tests();

  auto lines = read_lines(argv[1]);

  size_t result1 = part1(lines);
  size_t result2 = 0;

  std::vector<std::string> map = std::move(lines);
  while (true) {
    auto [count, after] = part2(map);

    if (count > 0) {
      result2 += count;
      std::swap(map, after);
    } else {
      break;
    }
  }

  std::cout << "Part 1: " << result1 << "\n";
  std::cout << "Part 2: " << result2 << "\n";

  return 0;
}

// vim: set sw=2:
