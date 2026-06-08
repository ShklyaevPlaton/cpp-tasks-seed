#include <exception>
#include <iostream>

#include "Gauss_solve.h"
#include "util.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " AB.csv\n";
        return 1;
    }

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        GaussVector solution = Gauss_solve(ab);

        for (int i = 0; i < solution.size(); ++i)
        {
            std::cout << solution(i);

            if (i + 1 < solution.size())
            {
                std::cout << ',';
            }
        }

        std::cout << '\n';
    }
    catch (const std::exception &error)
    {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
