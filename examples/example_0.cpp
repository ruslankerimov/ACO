#include "../src/ACO.h"
#include <iostream>
#include <vector>

using namespace std;

double fitness(vector <double> cords)
{
    double x = cords[0];
    double y = cords[1];

    return y * y - (x - 5) * (x - 5);
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
    config.max_iterations = 500;

    ACO * algorithm = new ACO(config);
    vector <double> best = algorithm->solve();

    cout << "x = " << best[0] << endl << "y = " << best[1] << endl;

    return 0;
}
