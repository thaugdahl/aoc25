#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>

std::vector<std::string> read_lines(const std::filesystem::path &path) {
  std::vector<std::string> result{};

  std::ifstream fs{path};

  std::string current_line{};
  while (std::getline(fs, current_line)) {
    result.emplace_back(std::move(current_line));
  }

  return result;
}

std::vector<int> separateDigits(std::string line)
{
  std::vector<int> result{};

  for ( char c : line ) {
    int integerValue = static_cast<int>(c - '0');

    result.emplace_back(integerValue);
  }

  return result;
}

size_t find_max_joltage(std::vector<int> batteries) {
  size_t result = 0;

  for ( size_t i = 0; i < batteries.size() - 1; i++ ) {
    for ( size_t j = i+1; j < batteries.size(); j++ ) {
      size_t joltage = batteries[i] * 10 + batteries[j];

      if ( joltage > result ) result = joltage;
    }
  }

  return result;
}


size_t find_overridden_joltage(std::vector<int> batteries, size_t count) {
  size_t result = 0;

  std::vector<std::vector<int64_t>> dp(count+1, std::vector<int64_t>(batteries.size() +1,0));

  for ( size_t digits = 1; digits <= count; digits++ ) {
    for ( int64_t battery = batteries.size() - digits; battery >= 0; battery-- ) {
      assert(battery >= 0);
      dp[digits][battery] = std::max(dp[digits][battery+1],
        (static_cast<int64_t>(std::pow(10, digits-1)) * batteries[battery] + dp[digits-1][battery+1]));
    }
  }

  return dp[count][0];

}


void tests() {
  // separateDigits
  auto x = separateDigits("1234");
  assert(x[0] == 1);
  assert(x[1] == 2);
  assert(x[2] == 3);
  assert(x[3] == 4);


  assert(find_max_joltage(separateDigits("987654321111111")) == 98);
  assert(find_max_joltage(separateDigits("811111111111119")) == 89);
  assert(find_max_joltage(separateDigits("234234234234278")) == 78);
  assert(find_max_joltage(separateDigits("818181911112111")) == 92);

  assert(find_overridden_joltage(separateDigits("987654321111111"), 12) == 987654321111);
  assert(find_overridden_joltage(separateDigits("811111111111119"), 12) == 811111111119);
  assert(find_overridden_joltage(separateDigits("234234234234278"), 12) == 434234234278);
  assert(find_overridden_joltage(separateDigits("818181911112111"), 12) == 888911112111);

}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Input please\n";
    abort();
  }

  tests();

  auto lines = read_lines(argv[1]);

  size_t result1 = 0;
  size_t result2 = 0;
  for ( auto &l : lines ) {
    result1 += find_max_joltage(separateDigits(l));
    result2 += find_overridden_joltage(separateDigits(l), 12);;
  }

  std::cout << "Part 1: " << result1 << "\n";
  std::cout << "Part 2: " << result2 << "\n";

  return 0;
}

// vim: set sw=2:
