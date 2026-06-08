#include <gtest/gtest.h>

#include <random>
#include <stdexcept>

#include "Gauss_solve.h"

namespace
{

constexpr double EPS = 1e-9;

void expect_vector_near(const GaussVector &actual, const GaussVector &expected)
{
    ASSERT_EQ(actual.size(), expected.size());

    for (int i = 0; i < actual.size(); ++i)
    {
        EXPECT_NEAR(actual(i), expected(i), EPS);
    }
}

}

TEST(GaussSolve, SolvesTwoByTwoSystem)
{
    GaussMatrix ab(2, 3);

    ab(0, 0) = 2.0;
    ab(0, 1) = 1.0;
    ab(0, 2) = 5.0;
    ab(1, 0) = 1.0;
    ab(1, 1) = -1.0;
    ab(1, 2) = 1.0;

    GaussVector expected(2);
    expected(0) = 2.0;
    expected(1) = 1.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, SolvesThreeByThreeSystem)
{
    GaussMatrix ab(3, 4);

    ab(0, 0) = 2.0;
    ab(0, 1) = 1.0;
    ab(0, 2) = -1.0;
    ab(0, 3) = 8.0;
    ab(1, 0) = -3.0;
    ab(1, 1) = -1.0;
    ab(1, 2) = 2.0;
    ab(1, 3) = -11.0;
    ab(2, 0) = -2.0;
    ab(2, 1) = 1.0;
    ab(2, 2) = 2.0;
    ab(2, 3) = -3.0;

    GaussVector expected(3);
    expected(0) = 2.0;
    expected(1) = 3.0;
    expected(2) = -1.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, UsesPivotingWhenFirstPivotIsZero)
{
    GaussMatrix ab(2, 3);

    ab(0, 0) = 0.0;
    ab(0, 1) = 2.0;
    ab(0, 2) = 4.0;
    ab(1, 0) = 1.0;
    ab(1, 1) = 1.0;
    ab(1, 2) = 3.0;

    GaussVector expected(2);
    expected(0) = 1.0;
    expected(1) = 2.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, SolvesGeneratedReproducibleSystem)
{
    constexpr int size = 40;

    std::mt19937 generator(12345);
    std::uniform_real_distribution<double> distribution(-10.0, 10.0);

    GaussMatrix a(size, size);
    GaussVector expected(size);

    for (int row = 0; row < size; ++row)
    {
        expected(row) = distribution(generator);

        for (int col = 0; col < size; ++col)
        {
            a(row, col) = distribution(generator);
        }

        a(row, row) += 500.0;
    }

    GaussVector b = a * expected;
    GaussMatrix ab(size, size + 1);

    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            ab(row, col) = a(row, col);
        }

        ab(row, size) = b(row);
    }

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, ThrowsOnSingularMatrix)
{
    GaussMatrix ab(2, 3);

    ab(0, 0) = 1.0;
    ab(0, 1) = 2.0;
    ab(0, 2) = 3.0;
    ab(1, 0) = 2.0;
    ab(1, 1) = 4.0;
    ab(1, 2) = 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}

TEST(GaussSolve, ThrowsOnWrongAugmentedMatrixSize)
{
    GaussMatrix ab(2, 2);

    ab(0, 0) = 1.0;
    ab(0, 1) = 2.0;
    ab(1, 0) = 3.0;
    ab(1, 1) = 4.0;

    EXPECT_THROW(Gauss_solve(ab), std::invalid_argument);
}
