#include <vector>
#include <cmath>

namespace gscf
{
    int polydegree;
    int data_points;

    struct point
    {
        float x;
        float y;
    };

    std::vector<point> points;

    struct chromosome
    {
        std::vector<float> genes;
        double fitness = 0.0;
        inline void evaluate_fitness()
        {
            float result;
            for(const auto& p : points)
            {
                result = 0.0;
                for(int i = 0; i <= polydegree; i++)
                {
                    result += genes[i] * std::pow(p.x, i);
                }
                result -= p.y;
                result = std::pow(result, 2);
                fitness += result;
            }
            fitness /= data_points;
            fitness = 1.f/fitness;
        }
    };

    std::vector<chromosome> population;
    std::vector<chromosome> child_population;

    bool compareByFitness(const chromosome &a, const chromosome &b)
    {
        return a.fitness < b.fitness;
    }
}