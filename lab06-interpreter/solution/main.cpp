#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "matrix.hpp"
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

namespace matrix_interpreter {

enum class command {
    load = 0,
    print = 5,
    elem = 3,
    exit = -1,
    add = 10,
    mul = 15,
    invalid = 1,
    unknown = 2
};

command parse_line(const std::string &cmd) {
    if (cmd == "load") {
        return command::load;
    }
    if (cmd == "print") {
        return command::print;
    }
    if (cmd == "elem") {
        return command::elem;
    }
    if (cmd == "exit") {
        return command::exit;
    }
    if (cmd == "add") {
        return command::add;
    }
    if (cmd == "mul") {
        return command::mul;
    }
    if (cmd.empty()) {
        return command::invalid;
    }
    return command::unknown;
}

class interpreter {
private:
    std::vector<matrix> m_registers;

    bool static validate_register(const std::string &reg) {
        if (reg.length() != 2 || reg[0] != '$' || reg[1] - '0' < 0 ||
            reg[1] - '0' > 9) {
            throw reg_exception(reg);
        }
        return true;
    }

    matrix &get_matrix(int reg) noexcept {
        return m_registers[reg];
    }

    void load_matrix(int reg, const std::string &filename) {
        std::filesystem::path file_name(filename);
        if (!std::filesystem::exists(file_name)) {
            throw file_acess_exception(filename);
        }
        std::ifstream file(filename);
        int rows = 0;
        int cols = 0;
        if (!(file >> rows >> cols)) {
            throw file_exception();
        }

        matrix matrix(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int element = 0;
                if (!(file >> element)) {
                    throw file_exception();
                }
                matrix.set_element(i, j, element);
            }
        }
        get_matrix(reg) = matrix;
    }

    void print_matrix(int reg) noexcept {
        get_matrix(reg).print();
    }

    void print_element(int reg, int row, int col) {
        const matrix &m = get_matrix(reg);
        int element = 0;
        element = m.get_element(row, col);
        std::cout << element << std::endl;
    }

public:
    interpreter() : m_registers(10) {
    }

    bool execute(const std::string &string) {
        std::stringstream str(string);
        std::string cmd;
        std::string garbage;
        str >> cmd;
        switch (parse_line(cmd)) {
            case command::load: {
                std::string reg;
                std::string filename;
                if (str >> reg >> filename && validate_register(reg) &&
                    !(str >> garbage)) {
                    load_matrix(reg[1] - '0', filename);
                } else {
                    throw command_exception();
                }
                break;
            }
            case command::print: {
                std::string reg;
                if (str >> reg && validate_register(reg) && !(str >> garbage)) {
                    print_matrix(reg[1] - '0');
                } else {
                    throw command_exception();
                }
                break;
            }
            case command::elem: {
                std::string reg;
                int row = 0;
                int col = 0;
                if (str >> reg >> row >> col && validate_register(reg) &&
                    !(str >> garbage)) {
                    print_element(reg[1] - '0', row, col);
                } else {
                    throw command_exception();
                }
                break;
            }
            case command::exit: {
                return true;
            }
            case command::add: {
                std::string A;
                std::string B;
                if (str >> A >> B && validate_register(A) &&
                    validate_register(B) && !(str >> garbage)) {
                    get_matrix(A[1] - '0').add_matrices(get_matrix(B[1] - '0'));
                } else {
                    throw command_exception();
                }
                break;
            }
            case command::mul: {
                std::string A;
                std::string B;
                if (str >> A >> B && validate_register(A) &&
                    validate_register(B) && !(str >> garbage)) {
                    get_matrix(A[1] - '0')
                        .multiply_matrices(get_matrix(B[1] - '0'));
                } else {
                    throw command_exception();
                }
                break;
            }
            case command::invalid: {
                throw command_exception();
            }
            default:
                throw unknown_command_exception(cmd);
        }
        return false;
    };
};
}  // namespace matrix_interpreter

int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
    std::string command;
    matrix_interpreter::interpreter interpr;
    bool end = false;
    while (std::getline(std::cin, command) && !end) {
        try {
            end = interpr.execute(command);
        } catch (const std::bad_alloc &) {
            std::cout << "Unable to allocate memory." << std::endl;
        } catch (const matrix_interpreter::matrix_exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
