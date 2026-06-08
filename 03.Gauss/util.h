#pragma once
#include <iosfwd>
#include <Eigen/Dense>

using GaussVector = Eigen::VectorXd;
using GaussMatrix = Eigen::MatrixXd;

GaussMatrix load_csv_to_matrix(const char *filename);

void print_matrix_as_csv(std::ostream &out, const GaussMatrix &matrix, int prec = 6);
