#include <iostream>
#include "ACO.h"

using namespace std;

ACO::ACO(ACOconfig config)
{
    ACO::config = config;
    dimension = config.limits.size(); // @todo
}

vector <double> ACO::solve()
{
    vector <Ant *> ants;

    for (int i = 0; i < config.count_of_ants; ++i)
    {
        vector <double> cords = get_random_cords();
        ants.push_back(new Ant(cords, config.fitness(cords)));
    }

    sort(ants.begin(), ants.end(), Ant::compare);

    for (int i = 0; i < config.max_iterations; ++i)
    {
        for (int j = 0; j < config.count_of_good_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                (ants[j]->tau)[k] += config.alpha * config.tau_0 * (config.count_of_good_ants - j - 1); // @todo
            }
        }

        vector <double> total_tau;
        for (int j = 0; j < dimension; ++j)
        {
            total_tau.push_back(0);
        }
        for (int j = 0; j < config.count_of_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                total_tau[k] += (ants[j]->tau)[k];
            }
        }

        for (int j = 0; j < config.count_of_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                (ants[j]->pMin)[k] = j == 0 ? 0 : (ants[j - 1]->pMax)[k];
                (ants[j]->pMax)[k] = (ants[j]->pMin)[k] + (ants[j]->tau)[k] / total_tau[k];
            }
        }

        for (int j = 1; j < config.count_of_ants; ++j)
        {
            if (random_double(0, 1) < config.propability_selecting)
            {
                for (int k = 0; k < dimension; ++k)
                {
                    double p = random_double(0, 1);

                    for (int m = 0; m < config.count_of_ants; ++m)
                    {
                        if (p >= (ants[m]->pMin)[k] && p < (ants[m]->pMax)[k])
                        {
                            (ants[j]->cords)[k] = (ants[m]->cords)[k];
                            break;
                        }
                    }
                }

                ants[j]->value = config.fitness(ants[j]->cords);
            }
        }

        sort(ants.begin(), ants.end(), Ant::compare);

        for (int j = config.count_of_ants - 1; j > config.count_of_ants - config.count_of_bed_ants - 1; --j)
        {
            ants[j]->cords = get_random_cords();
            ants[j]->value = config.fitness(ants[j]->cords);
        }

        sort(ants.begin(), ants.end(), Ant::compare);

        for (int j = 0; j < config.count_of_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                (ants[j]->tau)[k] *= config.ro;

                if (j < config.count_of_good_ants - 1)
                {
                    (ants[j]->tau)[k] += config.alpha * config.tau_0 * (config.count_of_good_ants - j - 1);
                }
            }
        }

        double old_value = ants[0]->value;
        vector <double> old_cords = ants[0]->cords;

        ants[0]->cords = get_random_neighbor_cords(ants[0]->cords);
        ants[0]->value = config.fitness(ants[0]->cords);

        if (old_value > ants[0]->value)
        {
            ants[0]->cords = old_cords;
            ants[0]->value = old_value;
        }
    }

    return ants[0]->cords;
}

bool ACO::is_rand_inited = false;

double ACO::random_double(double min, double max)
{
    if ( ! is_rand_inited)
    {
        srand(time(NULL));
        is_rand_inited = true;
    }

    return (rand() / ((double) RAND_MAX + 1.0)) * (max - min) + min;
}

vector <double> ACO::get_random_cords()
{
    vector <double> new_cords;

    for (int i = 0; i < dimension; ++i)
    {
        double min = config.limits[i].first;
        double max = config.limits[i].second;

        new_cords.push_back(random_double(min, max));
    }

    return new_cords;
}

vector <double> ACO::get_random_neighbor_cords(vector <double> cords)
{
    vector <double> new_cords;

    for (int i = 0; i < dimension; ++i)
    {
        double min = cords[i] - config.delta_r; // config.limits[i].first;
        double max = cords[i] + config.delta_r; // config.limits[i].second;

        if (min < config.limits[i].first)
        {
            min = config.limits[i].first;
        }

        if (max > config.limits[i].second)
        {
            max = config.limits[i].second;
        }

        new_cords.push_back(random_double(min, max));
    }

    return new_cords;
}

ACO::Ant::Ant(vector <double> cords, double value)
{
    vector <double> new_tau;
    vector <double> new_pMax;
    vector <double> new_pMin;
    for (int i = 0, size = cords.size(); i < size; ++i)
    {
        new_tau.push_back(1);
        new_pMax.push_back(0);
        new_pMin.push_back(0);
    }

    Ant::cords = cords;
    Ant::value = value;
    Ant::tau = new_tau; // @todo optimize
    Ant::pMax = new_pMax;
    Ant::pMin = new_pMin;
}

bool ACO::Ant::compare(Ant * a, Ant * b)
{
    return a->value > b->value;
}
