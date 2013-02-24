#ifndef ACO_H_INCLUDED
#define ACO_H_INCLUDED

#include <vector>
#include <algorithm>
#include <time.h>
#include "ACOconfig.h"

using namespace std;

class ACO
{
private:
    struct Ant
    {
        vector <double> cords;
        vector <double> tau;
        double value;
        Ant(vector <double>, vector <double>, double);
    };

    ACOconfig config;
    int dimension;
    vector <double> get_random_cords();
    vector <double> get_random_neighbor_cords(vector <double>);

    static double get_random_double(double, double);
    static bool is_rand_inited;

    static bool compare(Ant *, Ant *);
public:
    ACO(ACOconfig);
    vector <double> solve();
};

#endif
