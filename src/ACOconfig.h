#ifndef ACOCONFIG_H_INCLUDED

#include <vector>

using namespace std;

struct ACOconfig
{
    int count_of_ants;
    int count_of_good_ants;
    int count_of_bed_ants;
    int max_iterations;
    vector < pair <double, double> > limits;
    double ( * fitness)(vector <double>);
    float alpha;
    float ro;
    float delta_r;
    float propability_selecting;
    float tau_0;
    ACOconfig();
};

#endif
