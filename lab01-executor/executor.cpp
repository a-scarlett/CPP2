#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

const int dx[] = {0, 2, 0, -2};
const int dy[] = {2, 0, -2, 0};
const char arrow[] = {'^', '>', 'v', '<'};

using pair_t = std::pair<int, int>;

struct hash_name {
    size_t operator()(const pair_t &p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

using map_t = std::unordered_map<pair_t, char, hash_name>;

enum class command {
    quit = 0,
    sherlock = 101,
    cat = 100,
    invalid_command = 333,
    fwd = 2,
    bwd = -2,
    right = 1,
    left = -1,
    show_map = 42,
    start_macro = 50,
    end_macro = 55,
    run_macro = 16,
    put_wall = 20,
    remove_wall = 23
};

command parse_line(std::string &string) {
    std::stringstream str(string);
    std::string s;
    std::string check_garbage;
    str >> s;
    if (str >> check_garbage) {
        return command::invalid_command;
    }
    if (s == "quit" or s == "exit") {
        return command::quit;
    }
    if (s == "sherlock" or s == "sherlock-holmes") {
        return command::sherlock;
    }
    if (s == "cat" or s == "cat-jeoffry") {
        return command::cat;
    }
    if (s == "forward" or s == "fwd") {
        return command::fwd;
    }
    if (s == "backward" or s == "bwd") {
        return command::bwd;
    }
    if (s == "turn-right" or s == "right") {
        return command::right;
    }
    if (s == "turn-left" or s == "left") {
        return command::left;
    }
    if (s == "show-map") {
        return command::show_map;
    }
    if (s == "start-macro") {
        return command::start_macro;
    }
    if (s == "end-macro") {
        return command::end_macro;
    }
    if (s == "run-macro") {
        return command::run_macro;
    }
    if (s == "put-wall") {
        return command::put_wall;
    }
    if (s == "remove-wall") {
        return command::remove_wall;
    }
    return command::invalid_command;
}

void invalid_command(const std::string &s) {
    std::cout << "Invalid command: '" << s << "'" << std::endl;
}

void sherlock(const bool &record_macro, const std::string &s) {
    if (record_macro) {
        invalid_command(s);
        return;
    }
    std::cout
        << "I had had so many reasons to believe in my friend's subtle powers "
           "of\n"
           "reasoning and extraordinary energy in action that I felt that he "
           "must\n"
           "have some solid grounds for the assured and easy demeanour with "
           "which\n"
           "he treated the singular mystery which he had been called upon to\n"
           "fathom. Once only had I known him to fail, in the case of the King "
           "of\n"
           "Bohemia and of the Irene Adler photograph; but when I looked back "
           "to\n"
           "the weird business of the Sign of Four, and the extraordinary\n"
           "circumstances connected with the Study in Scarlet, I felt that it "
           "would\n"
           "be a strange tangle indeed which he could not unravel.\n";
}

void cat(const bool &record_macro, const std::string &s) {
    if (record_macro) {
        invalid_command(s);
        return;
    }
    std::cout << R"(For he is of the tribe of Tiger.
For the Cherub Cat is a term of the Angel Tiger.
For he has the subtlety and hissing of a serpent, which in goodness he suppresses.
For he will not do destruction, if he is well-fed, neither will he spit without provocation.
For he purrs in thankfulness, when God tells him he's a good Cat.
For he is an instrument for the children to learn benevolence upon.
For every house is incomplete without him and a blessing is lacking in the spirit.
For the Lord commanded Moses concerning the cats at the departure of the Children of Israel from Egypt.
For every family had one cat at least in the bag.
For the English Cats are the best in Europe.)"
              << std::endl;
}

void left_right(
    const command t,
    const bool &record_macro,
    std::vector<command> &macro,
    int &direction
) {
    if (record_macro) {
        macro.push_back(t);
        std::cout << "Recorded" << std::endl;
        return;
    }
    if (t == command::right) {
        direction++;
    }
    if (t == command::left) {
        direction--;
    }
    direction = (direction + 4) % 4;
}

bool check_adjoining_walls(
    const map_t &map,
    const pair_t &first,
    const pair_t &second,
    const pair_t &another,
    const char &ch_another,
    const char &ch
) {
    return (
        (map.find(first) == map.end() || map.at(first) != ch) &&
        (map.find(second) == map.end() || map.at(second) != ch) &&
        (map.find(another) == map.end() || map.at(another) != ch_another)
    );
}

bool check_wall(const int &i, const int &j, const map_t &map) {
    return (
        map.find({i, j}) == map.end() ||
        (map.at({i, j}) != '|' && map.at({i, j}) != '-')
    );
}

void fwd(
    map_t &map,
    pair_t &position,
    pair_t &width,
    pair_t &height,
    const int &direction,
    const bool &record_macro,
    std::vector<command> &macro
) {
    if (record_macro) {
        macro.push_back(command::fwd);
        std::cout << "Recorded" << std::endl;
        return;
    }
    if (check_wall(
            position.first + (dx[direction] / 2),
            position.second + (dy[direction] / 2), map
        )) {
        map[position] = 'o';
        position.first += dx[direction];
        position.second += dy[direction];
        width.second = std::max(width.second, position.first + 1);
        width.first = std::min(width.first, position.first - 1);
        height.first = std::min(height.first, position.second - 1);
        height.second = std::max(height.second, position.second + 1);
    } else {
        std::cout << "Bump!" << std::endl;
    }
}

void bwd(
    map_t &map,
    pair_t &position,
    pair_t &width,
    pair_t &height,
    const int &direction,
    const bool &record_macro,
    std::vector<command> &macro
) {
    if (record_macro) {
        macro.push_back(command::bwd);
        std::cout << "Recorded" << std::endl;
        return;
    }
    if (check_wall(
            position.first - (dx[direction] / 2),
            position.second - (dy[direction] / 2), map
        )) {
        map[position] = 'o';
        position.first -= dx[direction];
        position.second -= dy[direction];
        width.second = std::max(width.second, position.first + 1);
        width.first = std::min(width.first, position.first - 1);
        height.first = std::min(height.first, position.second - 1);
        height.second = std::max(height.second, position.second + 1);
    } else {
        std::cout << "Bump!" << std::endl;
    }
}

void show_map(
    map_t &map,
    const pair_t &position,
    const pair_t &width,
    const pair_t &height,
    const int &direction,
    const bool &record_macro,
    std::vector<command> &macro
) {
    if (record_macro) {
        macro.push_back(command::show_map);
        std::cout << "Recorded" << std::endl;
        return;
    }
    map[position] = arrow[direction];
    for (int j = height.second; j >= height.first; j--) {
        for (int i = width.first; i <= width.second; i++) {
            if (map.find({i, j}) == map.end()) {
                if (i % 2 == 0 && j % 2 == 0) {
                    map[{i, j}] = '.';
                    std::cout << map[{i, j}];
                } else {
                    map[{i, j}] = ' ';
                    std::cout << map[{i, j}];
                }
            } else {
                std::cout << map[{i, j}];
            }
        }
        std::cout << std::endl;
    }
}

void put_wall(
    map_t &map,
    const pair_t &position,
    const int &direction,
    const bool &record_macro,
    std::vector<command> &macro
) {
    if (record_macro) {
        macro.push_back(command::put_wall);
        std::cout << "Recorded" << std::endl;
        return;
    }
    if (direction % 2 == 0) {
        map[{position.first, position.second + (dy[direction] / 2)}] = '-';
        map[{position.first + 1, position.second + (dy[direction] / 2)}] = '+';
        map[{position.first - 1, position.second + (dy[direction] / 2)}] = '+';
    } else {
        map[{position.first + (dx[direction] / 2), position.second}] = '|';
        map[{position.first + (dx[direction] / 2), position.second + 1}] = '+';
        map[{position.first + (dx[direction] / 2), position.second - 1}] = '+';
    }
}

void remove_wall(
    map_t &map,
    const pair_t &position,
    const int &direction,
    const bool &record_macro,
    std::vector<command> &macro
) {
    if (record_macro) {
        macro.push_back(command::remove_wall);
        std::cout << "Recorded" << std::endl;
        return;
    }
    if (direction % 2 == 0) {
        map[{position.first, position.second + (dy[direction] / 2)}] = ' ';
        if (check_adjoining_walls(
                map,
                {position.first - 1, position.second + (dy[direction] / 2) - 1},
                {position.first - 1, position.second + (dy[direction] / 2) + 1},
                {position.first - 2, position.second + (dy[direction] / 2)},
                '-', '|'
            )) {
            map[{position.first - 1, position.second + (dy[direction] / 2)}] =
                ' ';
        }
        if (check_adjoining_walls(
                map,
                {position.first + 1, position.second + (dy[direction] / 2) - 1},
                {position.first + 1, position.second + (dy[direction] / 2) + 1},
                {position.first + 2, position.second + (dy[direction] / 2)},
                '-', '|'
            )) {
            map[{position.first + 1, position.second + (dy[direction] / 2)}] =
                ' ';
        }
    } else {
        map[{position.first + (dx[direction] / 2), position.second}] = ' ';
        if (check_adjoining_walls(
                map,
                {position.first + (dx[direction] / 2) + 1, position.second + 1},
                {position.first + (dx[direction] / 2) - 1, position.second + 1},
                {position.first + (dx[direction] / 2), position.second + 2},
                '|', '-'
            )) {
            map[{position.first + (dx[direction] / 2), position.second + 1}] =
                ' ';
        }
        if (check_adjoining_walls(
                map,
                {position.first + (dx[direction] / 2) + 1, position.second - 1},
                {position.first + (dx[direction] / 2) - 1, position.second - 1},
                {position.first + (dx[direction] / 2), position.second - 2},
                '|', '-'
            )) {
            map[{position.first + (dx[direction] / 2), position.second - 1}] =
                ' ';
        }
    }
}

void end_macro(
    bool &record_macro,
    const std::string &s,
    const std::vector<command> &macro
) {
    if (!record_macro) {
        invalid_command(s);
        return;
    }
    record_macro = false;
    std::cout << "New macro of " << static_cast<int>(macro.size())
              << " command(s)" << std::endl;
};

int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    std::string test1 = "    fwd      ";
    std::string test2 = "   sherlock- holmes";
    std::string test3 = "cad    ";
    assert(parse_line(test1) == command::fwd);
    assert(parse_line(test2) == command::invalid_command);
    assert(parse_line(test3) == command::invalid_command);

    std::string s;
    int direction = 0;
    int count_macro = 0;
    command now;  // NOLINT(cppcoreguidelines-init-variables)
    bool end = false;
    bool record_macro = false;
    bool run_macro = false;
    std::vector<command> macro;
    map_t map;
    pair_t position = {0, 0};
    pair_t width = {-1, 1};
    pair_t height = {-1, 1};

    while (true) {
        if (run_macro) {
            count_macro++;
            if (count_macro < static_cast<int>(macro.size())) {
                now = macro[count_macro];
            } else {
                run_macro = false;
                continue;
            }
        } else {
            if (end or !std::getline(std::cin, s)) {
                break;
            } else {
                now = parse_line(s);
            }
        }
        switch (now) {
            case command::sherlock: {
                sherlock(record_macro, s);
                break;
            }
            case command::cat: {
                cat(record_macro, s);
                break;
            }
            case command::quit: {
                if (record_macro) {
                    invalid_command(s);
                    break;
                }
                end = true;
                break;
            }
            case command::invalid_command: {
                invalid_command(s);
                break;
            }
            case command::fwd: {
                fwd(map, position, width, height, direction, record_macro,
                    macro);
                break;
            }
            case command::bwd: {
                bwd(map, position, width, height, direction, record_macro,
                    macro);
                break;
            }
            case command::right: {
                left_right(command::right, record_macro, macro, direction);
                break;
            }
            case command::left: {
                left_right(command::left, record_macro, macro, direction);
                break;
            }
            case command::show_map: {
                show_map(
                    map, position, width, height, direction, record_macro, macro
                );
                break;
            }
            case command::start_macro: {
                if (record_macro) {
                    invalid_command(s);
                    break;
                }
                macro.clear();
                count_macro = 0;
                record_macro = true;
                break;
            }
            case command::end_macro: {
                end_macro(record_macro, s, macro);
                break;
            }
            case command::run_macro: {
                run_macro = true;
                count_macro = -1;
                break;
            }
            case command::put_wall: {
                put_wall(map, position, direction, record_macro, macro);
                break;
            }
            case command::remove_wall: {
                remove_wall(map, position, direction, record_macro, macro);
                break;
            }
            default: {
                std::cout << "Command in processed" << std::endl;
                break;
            }
        }
    }
}