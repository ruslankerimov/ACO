#include "src/ACO.h"
#include <iostream>
#include <vector>

using namespace std;

double fitness(vector <double> cords)
{
    return cords[1] * cords[1] - (cords[0] - 5) * (cords[0] - 5);
}

int main(int argc, char** argv)
{
    vector < pair <double, double> > limits;
    limits.push_back(make_pair(-10, 10));
    limits.push_back(make_pair(-10, 10));

    ACOconfig config;
    config.limits = limits;
    config.fitness = fitness;
    config.count_of_ants = 100;
    config.max_iterations = 50;

	ACO * aco = new ACO(config);

	aco->solve();

	return 0;
}
