#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>

enum class Direction { LEFT, RIGHT };

std::vector<std::string> read_lines(const std::filesystem::path &path) {
  std::vector<std::string> result{};

  std::ifstream fs{path};

  std::string current_line{};
  while (std::getline(fs, current_line)) {
    result.emplace_back(std::move(current_line));
  }

  return result;
}

std::pair<size_t, size_t> extract_range(std::string_view sv) {
    const auto dash_pos = sv.find('-');
    std::string_view first_sv = sv.substr(0, dash_pos);
    std::string_view second_sv = sv.substr(dash_pos+1);

    size_t first = std::stoull(std::string{first_sv});
    size_t second = std::stoull(std::string{second_sv});

    return std::make_pair(first, second);
}

std::vector<std::pair<size_t, size_t>> extract_ranges(std::string_view sv) {

  std::vector<std::pair<size_t, size_t>> result{};
  std::string_view rest{sv};

  while ( !rest.empty() ) {
    auto pos = rest.find(',');
    auto candidate = rest.substr(0, pos);
    rest = rest.substr(pos+1);
    result.emplace_back(extract_range(candidate));

    if ( pos == rest.npos ) {
      rest = {};
    }
  }

  return result;

}

constexpr bool is_valid_p1(const std::string_view &repr) {
  const auto len = repr.length();
  if ( len % 2 == 1 ) return true;

  if ( repr.substr(0,len/2) == repr.substr(len/2) ) {
    return false;
  }

  return true;
}

constexpr bool is_valid_p2(const std::string_view &repr) {
  const auto len = repr.length();
  size_t max_block_size = len / 2;
  size_t num_blocks = len / max_block_size;

  for ( size_t block_size = max_block_size; block_size >= 1; block_size-- ) {
    if ( len % block_size != 0 ) continue;

    std::string_view base_block = repr.substr(0, block_size);

    bool repeats = true;
    for ( int block_index = 1; block_index < len/block_size; block_index++ ) {
      std::string_view compare_block = repr.substr(block_index*block_size,block_size);
      if ( compare_block != base_block ) { repeats = false; break; }
    }

    if ( repeats ) {
      return false;
    }

  }


  return true;
}

static_assert(is_valid_p1("11") == false);
static_assert(is_valid_p1("12") == true);


static_assert(is_valid_p1("22") == false);
static_assert(is_valid_p1("223") == true);
static_assert(is_valid_p1("223223") == false);

static_assert(is_valid_p2("824824824") == false);


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Input please\n";
    abort();
  }

  auto lines = read_lines(argv[1]);

  assert(lines.size() == 1 && "Expected single-line input");

  std::vector<std::pair<size_t, size_t>> ranges = extract_ranges(lines[0]);

  size_t invalid_sum = 0;
  size_t invalid_sum_p2 = 0;
  for ( auto &[first, second] : ranges ) {
    for ( size_t i = first; i <= second; i++ ) {
      if ( ! is_valid_p1(std::to_string(i)) ) {
        invalid_sum += i;
      }

      if ( ! is_valid_p2(std::to_string(i)) ) {
        invalid_sum_p2 += i;
      }
    }
  }

  std::cout << "Sum (Part 1): " << invalid_sum << "\n";
  std::cout << "Sum (Part 2): " << invalid_sum_p2 << "\n";


  return 0;
}
