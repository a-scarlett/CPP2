#ifndef matrix_hpp
#define matrix_hpp

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace matrix_interpreter {

class matrix_exception : public std::exception {
protected:
    std::string
        // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
        m_message;  // NOLINT(misc-non-private-member-variables-in-classes)

public:
    explicit matrix_exception(std::string &&message)
        : m_message(std::move(message)) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return m_message.c_str();
    }
};

class bounds_exception : public matrix_exception {
public:
    explicit bounds_exception()
        : matrix_exception("Requested element is out of bounds") {
    }
};

class matrix_dimension_exception : public matrix_exception {
public:
    explicit matrix_dimension_exception(std::string &&left, std::string &&right)
        : matrix_exception(
              "Dimension mismatch: lhs=" + left + ", rhs=" + right
          ) {
    }
};

class unknown_command_exception : public matrix_exception {
public:
    explicit unknown_command_exception(std::string &cmd)
        : matrix_exception("Unknown command: '" + cmd + "'") {
    }
};

class command_exception : public matrix_exception {
public:
    explicit command_exception() : matrix_exception("Invalid command format") {
    }
};

class file_acess_exception : public matrix_exception {
public:
    explicit file_acess_exception(const std::string &filename)
        : matrix_exception("Unable to open file '" + filename + "'") {
    }
};

class reg_exception : public matrix_exception {
public:
    explicit reg_exception(const std::string &reg)
        : matrix_exception("'" + reg + "' is not a register") {
    }
};

class file_exception : public matrix_exception {
public:
    explicit file_exception() : matrix_exception("Invalid file format") {
    }
};

class matrix {
private:
    int m_rows;
    int m_cols;
    std::vector<std::vector<int>> m_data;

public:
    void validate_index(int r, int c) const;

    matrix() : m_rows(0), m_cols(0) {
    }

    matrix(int r, int c);

    void print() const noexcept;

    [[nodiscard]] int get_element(int r, int c) const;

    void set_element(int r, int c, int value);

    void add_matrices(const matrix &B);

    void multiply_matrices(const matrix &B);
};
}  // namespace matrix_interpreter
#endif  // matrix_hpp
