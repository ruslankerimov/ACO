#include "ACO.h"

using namespace std;

ACO::ACO(ACOconfig config)
{
    ACO::config = config;
    dimension = config.limits.size();
}

vector <double> ACO::solve()
{
    vector <Ant *> ants;

    for (int i = 0; i < config.count_of_ants; ++i)
    {
        vector <double> cords = get_random_cords();
        ants.push_back(
            new Ant(cords, vector <double> (dimension, config.tau_0), config.fitness(cords)));
    }

    sort(ants.begin(), ants.end(), compare);

    for (int i = 0; i < config.max_iterations; ++i)
    {
        for (int j = 0; j < config.count_of_good_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                (ants[j]->tau)[k] += config.alpha * config.tau_0 * (config.count_of_good_ants - j - 1); // @todo
            }
        }

        vector <double> total_tau (dimension, 0);
        for (int j = 0; j < config.count_of_ants; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                total_tau[k] += (ants[j]->tau)[k];
            }
        }

        vector < vector < pair <double, double> > > p_ants;
        for (int j = 0; j < config.count_of_ants; ++j)
        {
            vector < pair <double, double> > p_ant;
            for (int k = 0; k < dimension; ++k)
            {
                double p_min = j == 0 ? 0 : p_ants[j - 1][k].second;
                double p_max = p_min + (ants[j]->tau)[k] / total_tau[k];
                p_ant.push_back(make_pair(p_min, p_max));
            }
            p_ants.push_back(p_ant);
        }

        for (int j = 1; j < config.count_of_ants; ++j)
        {
            if (get_random_double(0, 1) < config.propability_selecting)
            {
                for (int k = 0; k < dimension; ++k)
                {
                    double p = get_random_double(0, 1);

                    for (int m = 0; m < config.count_of_ants; ++m)
                    {
                        if (p >= p_ants[m][k].first && p < p_ants[m][k].second)
                        {
                            (ants[j]->cords)[k] = (ants[m]->cords)[k];
                            break;
                        }
                    }
                }

                ants[j]->value = config.fitness(ants[j]->cords);
            }
        }

        sort(ants.begin(), ants.end(), compare);

        for (int j = config.count_of_ants - 1; j > config.count_of_ants - config.count_of_bed_ants - 1; --j)
        {
            ants[j]->cords = get_random_cords();
            ants[j]->value = config.fitness(ants[j]->cords);
        }

        sort(ants.begin(), ants.end(), compare);

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

double ACO::get_random_double(double min, double max)
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

        new_cords.push_back(get_random_double(min, max));
    }

    return new_cords;
}

vector <double> ACO::get_random_neighbor_cords(vector <double> cords)
{
    vector <double> new_cords;

    for (int i = 0; i < dimension; ++i)
    {
        double min = cords[i] - config.delta_r;
        double max = cords[i] + config.delta_r;

        if (min < config.limits[i].first)
        {
            min = config.limits[i].first;
        }

        if (max > config.limits[i].second)
        {
            max = config.limits[i].second;
        }

        new_cords.push_back(get_random_double(min, max));
    }

    return new_cords;
}

ACO::Ant::Ant(vector <double> cords, vector <double> tau, double value)
{
    Ant::cords = cords;
    Ant::value = value;
    Ant::tau = tau;
}

bool ACO::compare(Ant * a, Ant * b)
{
    return a->value > b->value;
}
