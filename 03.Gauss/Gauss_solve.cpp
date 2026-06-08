#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>

namespace {
constexpr double EPS = 1e-12;
}

GaussVector Gauss_solve(GaussMatrix &ab) {
    GaussMatrix matrix = ab;

    const int rows = static_cast<int>(matrix.rows());
    const int cols = static_cast<int>(matrix.cols());

    if (rows == 0 || cols != rows + 1) {
        throw std::invalid_argument("Expected augmented matrix with n rows and n + 1 columns");
    }

    for (int col = 0; col < rows; ++col) {
        int pivot = col;

        for (int row = col + 1; row < rows; ++row) {
            if (std::abs(matrix(row, col)) > std::abs(matrix(pivot, col))) {
                pivot = row;
            }
        }

        if (std::abs(matrix(pivot, col)) < EPS) {
            throw std::runtime_error("Matrix is singular or system has no unique solution");
        }

        if (pivot != col) {
            matrix.row(col).swap(matrix.row(pivot));
        }

        const double pivot_value = matrix(col, col);
        matrix.row(col) /= pivot_value;

        for (int row = 0; row < rows; ++row) {
            if (row == col) {
                continue;
            }

            const double factor = matrix(row, col);

            if (std::abs(factor) > EPS) {
                matrix.row(row) -= factor * matrix.row(col);
            }
        }
    }

    return matrix.col(rows);
}
