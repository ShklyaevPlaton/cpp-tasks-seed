#include <gtest/gtest.h>

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

    ab << 2.0, 1.0, 5.0,
        1.0, -1.0, 1.0;

    GaussVector expected(2);
    expected << 2.0, 1.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, SolvesThreeByThreeSystem)
{
    GaussMatrix ab(3, 4);

    ab << 2.0, 1.0, -1.0, 8.0,
        -3.0, -1.0, 2.0, -11.0,
        -2.0, 1.0, 2.0, -3.0;

    GaussVector expected(3);
    expected << 2.0, 3.0, -1.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, UsesPivotingWhenFirstPivotIsZero)
{
    GaussMatrix ab(2, 3);

    ab << 0.0, 2.0, 4.0,
        1.0, 1.0, 3.0;

    GaussVector expected(2);
    expected << 1.0, 2.0;

    expect_vector_near(Gauss_solve(ab), expected);
}

TEST(GaussSolve, ThrowsOnSingularMatrix)
{
    GaussMatrix ab(2, 3);

    ab << 1.0, 2.0, 3.0,
        2.0, 4.0, 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}

TEST(GaussSolve, ThrowsOnWrongAugmentedMatrixSize)
{
    GaussMatrix ab(2, 2);

    ab << 1.0, 2.0,
        3.0, 4.0;

    EXPECT_THROW(Gauss_solve(ab), std::invalid_argument);
}
