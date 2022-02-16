#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <random>
#include "genetic_smooth_curve_fitting.h"

int main()
{	
	int sets;
	int points;
	int degree;
	float x_coord;
	float y_coord;

	// Chosen parameters
	// Increase POPULATION_SIZE and GENERATIONS for better accuracy but slower running time
	const unsigned short POPULATION_SIZE = 1000;
	const unsigned short GENERATIONS = 10;
	const unsigned short TOURNAMENT_SIZE = 16;
	const unsigned short prCROSSOVER = 70;
	const unsigned short prMUTATION = 1;
	const float non_uniformity = 1.5;		// "b" in value-of-mutation equation
	const float LB = -10.0;
	const float UB = 10.0;

	long int seed = (unsigned)(time(0));
	srand(seed);

	std::ifstream input("input-2.txt");
	std::ofstream output("output-2.txt");
	input >> sets;
	for(int n = 0; n < sets; n++)
	{
		input >> points;
		input >> degree;
		gscf::data_points = points;
		gscf::polydegree = degree;
		for(int i = 0; i < points; i++)
		{
			input >> x_coord;
			input >> y_coord;
			gscf::points.push_back({x_coord,y_coord});
		}
		
		// Initializing population
		for(int i = 0; i < POPULATION_SIZE; i++)
		{
			gscf::chromosome cr;
			for(int j = 0; j <= degree; j++)
			{
				cr.genes.push_back(LB + (float)(rand()) / ((float)(RAND_MAX / (UB-LB))));
			}
			cr.evaluate_fitness();
			gscf::population.push_back(cr);
		}

		gscf::chromosome all_time_best = *std::max_element(gscf::population.begin(), gscf::population.end(), gscf::compareByFitness);

		for(int i = 0; i < GENERATIONS; i++)
		{
			gscf::chromosome generational_best = *std::max_element(gscf::population.begin(), gscf::population.end(), gscf::compareByFitness);
			
			if(gscf::compareByFitness(all_time_best, generational_best))
			{
				all_time_best = generational_best;
			}
			
			for(int j = 0; j < POPULATION_SIZE / 2; j++)
			{
				// Performing "Tournament" selection
				gscf::chromosome parent1;
				gscf::chromosome parent2;

				std::shuffle(gscf::population.begin(), gscf::population.end(), std::default_random_engine(seed));
				parent1.genes = 
						(*std::max_element(gscf::population.begin(), gscf::population.begin()+TOURNAMENT_SIZE, gscf::compareByFitness)).genes;

				std::shuffle(gscf::population.begin(), gscf::population.end(), std::default_random_engine(seed));
				parent2.genes = 
						(*std::max_element(gscf::population.begin(), gscf::population.begin()+TOURNAMENT_SIZE, gscf::compareByFitness)).genes;

				gscf::chromosome child1;
				gscf::chromosome child2;

				// Performing 2-point crossover 
				if((rand() % 100) <= prCROSSOVER)
				{
					int point1 = (rand() % degree) + 1;
					int point2 = (rand() % degree) + 1;
					
					while(point1 == point2)
					{
						point1 = (rand() % degree) + 1;
						point2 = (rand() % degree) + 1;
					}

					if(point1 > point2)
					{
						int temp = point1;
						point1 = point2;
						point2 = temp;
					}
					
					for(int cr = 0; cr < point1; cr++)
					{
						child1.genes.push_back(parent1.genes[cr]);
						child2.genes.push_back(parent2.genes[cr]);
					}
					
					for(int cr = point1; cr < point2; cr++)
					{
						child1.genes.push_back(parent2.genes[cr]);
						child2.genes.push_back(parent1.genes[cr]);
					}
					
					for(int cr = point2; cr <= degree; cr++)
					{
						child1.genes.push_back(parent1.genes[cr]);
						child2.genes.push_back(parent2.genes[cr]);
					}
				}
				else
				{
					child1.genes = parent1.genes;
					child2.genes = parent2.genes;
				}
				
				// Performing non-uniform mutation
				for(int cr = 0; cr <= degree; cr++)
				{
					if((rand() % 100) <= prMUTATION)
					{
						if(rand() % 2)
						{
							float delta_lower = child1.genes[cr] - LB;
							float r = (float)(rand()) / (float)(RAND_MAX);
							float r_power = pow((1 - ((i+1) / GENERATIONS)), non_uniformity);
							float value_of_mutation = delta_lower * (1 - pow(r, r_power));
							child1.genes[cr] -= value_of_mutation;
						}

						else
						{
							float delta_upper = UB - child1.genes[cr];
							float r = (float)(rand()) / (float)(RAND_MAX);
							float r_power = pow((1 - ((i+1) / GENERATIONS)), non_uniformity);
							float value_of_mutation = delta_upper * (1 - pow(r, r_power));
							child1.genes[cr] += value_of_mutation;
						}
					}
				}

				for(int cr = 0; cr <= degree; cr++)
				{
					if((rand() % 100) <= prMUTATION)
					{
						if(rand() % 2)
						{
							float delta_lower = child2.genes[cr] - LB;
							float r = (float)(rand()) / (float)(RAND_MAX);
							float r_power = pow((1 - ((i+1) / GENERATIONS)), non_uniformity);
							float value_of_mutation = delta_lower * (1 - pow(r, r_power));
							child2.genes[cr] -= value_of_mutation;
						}

						else
						{
							float delta_upper = UB - child2.genes[cr];
							float r = (float)(rand()) / (float)(RAND_MAX);
							float r_power = pow((1 - ((i+1) / GENERATIONS)), non_uniformity);
							float value_of_mutation = delta_upper * (1 - pow(r, r_power));
							child2.genes[cr] += value_of_mutation;
						}
					}
				}
				
				child1.evaluate_fitness();
				child2.evaluate_fitness();

				gscf::child_population.push_back(child1);
				gscf::child_population.push_back(child2);

			}
			
			// Performing generational replacement
			gscf::population = gscf::child_population;
			gscf::child_population.clear();
		}

		double mse = 1.f / all_time_best.fitness;
		std::cout << "CASE " << n + 1 << ":\n\n" << "Total Error (MSE):\n" << mse << "\n\n";
		std::cout << "Coefficients:\n";
		for (auto coeff: all_time_best.genes)
		{
        	std::cout << coeff << '\n';
			output << coeff << "  ";
    	}
		output << "Error: " << mse;
		output << "\n";
		std::cout << "\n\n";
		gscf::points.clear();
		gscf::population.clear();
	}

	return 0;
}