#include "util.h"

#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

#include "lazycsv.hpp"

GaussMatrix load_csv_to_matrix(const char *filename) {
    std::vector<std::vector<double>> values;

    lazycsv::parser parser{filename};

    for (const auto row : parser) {
        std::vector<double> current_row;

        for (const auto cell : row) {
            current_row.push_back(std::stod(std::string(cell.raw())));
        }

        if (!values.empty() && current_row.size() != values.front().size()) {
            throw std::runtime_error("CSV rows have different sizes");
        }

        values.push_back(current_row);
    }

    if (values.empty()) {
        throw std::runtime_error("CSV file is empty");
    }

    GaussMatrix matrix(values.size(), values.front().size());

    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        for (int j = 0; j < static_cast<int>(values[i].size()); ++j) {
            matrix(i, j) = values[i][j];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream &out, const GaussMatrix &matrix, int prec) {
    out << std::fixed << std::setprecision(prec);

    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            out << matrix(i, j);

            if (j + 1 < matrix.cols()) {
                out << ',';
            }
        }

        out << '\n';
    }
}
