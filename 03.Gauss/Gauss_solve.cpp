#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

namespace {
constexpr double EPS = 1e-12;
}

GaussVector Gauss_solve(GaussMatrix ab) {
    const int rows = static_cast<int>(ab.rows());
    const int cols = static_cast<int>(ab.cols());

    if (rows == 0 || cols != rows + 1) {
        throw std::invalid_argument("Expected augmented matrix with n rows and n + 1 columns");
    }

    for (int col = 0; col < rows; ++col) {
        int pivot = col;

        for (int row = col + 1; row < rows; ++row) {
            if (std::abs(ab(row, col)) > std::abs(ab(pivot, col))) {
                pivot = row;
            }
        }

        if (std::abs(ab(pivot, col)) < EPS) {
            throw std::runtime_error("Matrix is singular or system has no unique solution");
        }

        if (pivot != col) {
            ab.row(col).swap(ab.row(pivot));
        }

        const double pivot_value = ab(col, col);
        ab.row(col) /= pivot_value;

        for (int row = 0; row < rows; ++row) {
            if (row == col) {
                continue;
            }

            const double factor = ab(row, col);

            if (std::abs(factor) > EPS) {
                ab.row(row) -= factor * ab.row(col);
            }
        }
    }

    return ab.col(rows);
}
