#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>


enum class Direction {
    LEFT,
    RIGHT
};

std::vector<std::string> read_lines(const std::filesystem::path &path)
{
    std::vector<std::string> result{};

    std::ifstream fs{path};

    std::string current_line{};
    while ( std::getline(fs, current_line) ) {
        result.emplace_back(std::move(current_line));
    }

    return result;
}


std::pair<Direction, int> parse_line(const std::string &line) {
    std::string_view sv{line};
    sv = sv.substr(1);

    Direction res_direction = Direction::LEFT;
    if ( line[0] == 'R' ) res_direction = Direction::RIGHT;

    int count = std::stoi(std::string{sv});

    return std::make_pair(res_direction, count);
}

int clicks_on_wrap(int current, int count, Direction dir) {
    int delta = count % 100;
    int full_wraps = count / 100;

    if ( dir == Direction::RIGHT ) {
        return (current + delta > 100) ? 1 + full_wraps : full_wraps;
    } else {
        if ( current == 0 ) return full_wraps;
        int underflow = (current - (delta)) < 0 ? 1 : 0;
        return underflow + full_wraps;
    }
}

int update_position(int current, int count, Direction dir) {
    int delta = count % 100;
    if ( dir == Direction::LEFT ) {
        delta = -delta;
    }

    int new_pos = (current + delta) % 100;
    if ( new_pos < 0) new_pos += 100;
    return new_pos;
}

int main(int argc, char *argv[])
{

    if ( argc < 2 ) {
        std::cerr << "Input please\n";
        abort();
    }

    auto lines = read_lines(argv[1]);


    int zero_count = 0;
    int current = 50;

    for ( auto &l : lines ) {
        auto [dir, count] = parse_line(l);

        int partial_clicks = clicks_on_wrap(current, count, dir);
        int new_pos = update_position(current, count, dir);
        if ( new_pos == 0 ) ++partial_clicks;
        zero_count += partial_clicks;
        current = new_pos;
    }

    std::cout << "Password: " << zero_count << "\n";


    return 0;
}
