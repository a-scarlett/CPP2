#include "matrix.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace matrix_interpreter {

matrix::matrix(int r, int c) : m_rows(r), m_cols(c) {
    if (r > 0 && c > 0) {
        m_data.resize(r, std::vector<int>(c, 0));
    } else {
        m_rows = r;
        m_cols = c;
    }
    // try catch for std::bad_alloc ?
}

void matrix::validate_index(int r, int c) const {
    if (r < 0 || r >= m_rows || c < 0 || c >= m_cols) {
        throw bounds_exception();
    }
}

void matrix::print() const noexcept {
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < (m_cols - 1); j++) {
            std::cout << m_data[i][j] << " ";
        }
        std::cout << m_data[i][(m_cols - 1)] << std::endl;
    }
}

[[nodiscard]] int matrix::get_element(int r, int c) const {
    validate_index(r, c);
    return m_data[r][c];
}

void matrix::set_element(int r, int c, int value) {
    validate_index(r, c);
    m_data[r][c] = value;
}

void matrix::add_matrices(const matrix &B) {
    if (this->m_rows != B.m_rows) {
        throw matrix_dimension_exception(
            std::to_string(this->m_rows), std::to_string(B.m_rows)
        );
    }
    if (this->m_cols != B.m_cols) {
        throw matrix_dimension_exception(
            std::to_string(this->m_cols), std::to_string(B.m_cols)
        );
    }
    for (int i = 0; i < this->m_rows; i++) {
        for (int j = 0; j < this->m_cols; j++) {
            this->m_data[i][j] += B.m_data[i][j];
        }
    }
}

void matrix::multiply_matrices(const matrix &B) {
    if (this->m_cols != B.m_rows) {
        throw matrix_dimension_exception(
            std::to_string(this->m_cols), std::to_string(B.m_rows)
        );
    }

    matrix C(this->m_rows, B.m_cols);
    for (int i = 0; i < this->m_rows; i++) {
        for (int j = 0; j < B.m_cols; j++) {
            for (int p = 0; p < this->m_cols; p++) {
                C.m_data[i][j] += (this->m_data[i][p] * B.m_data[p][j]);
            }
        }
    }
    this->m_cols = B.m_cols;
    this->m_data.resize(this->m_rows, std::vector<int>(this->m_cols, 0));
    for (int i = 0; i < this->m_rows; i++) {
        for (int j = 0; j < this->m_cols; j++) {
            this->m_data[i][j] = C.m_data[i][j];
        }
    }
}
}  // namespace matrix_interpreter
