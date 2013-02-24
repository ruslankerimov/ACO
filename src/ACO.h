#ifndef ACO_H_INCLUDED
#define ACO_H_INCLUDED

#include <vector>
#include <algorithm>
#include "ACOconfig.h"

using namespace std;

class ACO
{
private:
    struct Ant
    {
        vector <double> cords;
        vector <double> tau; // @todo rename
        vector <double> pMin; // @todo rename
        vector <double> pMax; // @todo rename
        double value;
        static bool compare(Ant *, Ant *);
        Ant(vector <double>, double);
    };

    ACOconfig config;
    int dimension;
    vector <double> get_random_cords();

    static double random_double(double, double);
    static bool is_rand_inited;
public:
    ACO(ACOconfig);
    void solve();
};

#endif
