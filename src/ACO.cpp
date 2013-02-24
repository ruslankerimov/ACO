#include <iostream>
#include "ACO.h"

using namespace std;

ACO::ACO(ACOconfig config)
{
    ACO::config = config;
    dimension = config.limits.size(); // @todo
}


/*

var options = {
    ant_count : 20,
    good_ants : 4,
    alpha : 0.9,
    tau0 : 1,
    propability_selecting : 0.9,
    bad_ants : 3,
    ro : 0.7,
    delta_r : 0.1
};

function Ant(point) {
    this._point = point || this._getRandomPoint();
    this._tauX = options.tau0;
    this._tauY = options.tau0;
    this._value = this._getValue();
}

Ant.prototype._getValue = function() {
    var point = this._point;

    return point.x * point.x + point.y * point.y;
};

Ant.prototype._getRandomPoint = function() {
    return {
        x : this._getRandom(-1000, 1000),
        y : this._getRandom(-1000, 1000)
    };
};

Ant.prototype._getRandom = function(a, b) {
    return Math.random() * (b - a) + a;
};

Ant.prototype.setTau = function() {

};

Ant.prototype.valueOf = function() {
    return this._value;
};



function main() {
    var i, size, j, k,
        ants = [],
        p,
        totalTau;

    for (i = 0, size = options.ant_count; i < size; ++i) {
        ants.push(new Ant());
    }

    ants.sort(function(a, b) {
        return b - a;
    });

    for (i = 0, size = options.good_ants - 1; i < size; ++i) {
        ants[i]._tauX += options.alpha * options.tau0 * (options.good_ants - i - 1);
        ants[i]._tauY += options.alpha * options.tau0 * (options.good_ants - i - 1);
    }

    for (k = 0; k < 500; ++k) {
        totalTau = { x : 0, y : 0 };
        for (i = 0, size = options.ant_count; i < size; ++i) {
            totalTau.x += ants[i]._tauX;
            totalTau.y += ants[i]._tauY;
        }

        for (i = 0, size = options.ant_count; i < size; ++i) {
            ants[i]._pXmin = i === 0 ? 0 : ants[i - 1]._pXmax;
            ants[i]._pXmax = ants[i]._pXmin + ants[i]._tauX / totalTau.x;

            ants[i]._pYmin = i === 0 ? 0 : ants[i - 1]._pYmax;
            ants[i]._pYmax = ants[i]._pYmin + ants[i]._tauY / totalTau.y;
        }

        for (i = 1, size = options.ant_count; i < size; ++i) {
            p = Math.random();
            if (p < options.propability_selecting) {
                p = Math.random();
                for (j = 0; j < options.ant_count; ++j) {
                    if (p >= ants[j]._pXmin && p < ants[j]._pXmax) {
                        ants[i]._point.x = ants[j]._point.x;
                    }
                }

                p = Math.random();
                for (j = 0; j < options.ant_count; ++j) {
                    if (p >= ants[j]._pYmin && p < ants[j]._pYmax) {
                        ants[i]._point.y = ants[j]._point.y;
                    }
                }

                ants[i]._value = ants[i]._getValue();
            }
        }

        ants.sort(function(a, b) {
            return -b + a;
        });

        for (i = options.ant_count - 1; i > options.ant_count - options.bad_ants - 1; --i) {
            ants[i]._point = ants[i]._getRandomPoint();
            ants[i]._value = ants[i]._getValue();
        }

        ants.sort(function(a, b) {
            return a - b;
        });

        for (i = 0; i < options.ant_count; ++i) {
            ants[i]._tauX *= options.ro;
            ants[i]._tauY *= options.ro;

            if (i < options.good_ants - 1) {
                ants[i]._tauX += options.alpha * options.tau0 * (options.good_ants - i - 1);
                ants[i]._tauY += options.alpha * options.tau0 * (options.good_ants - i - 1);
            }


        }

        var old_value = ants[0]._value,
            old_x = ants[0]._point.x,
            old_y = ants[0]._point.y;

        ants[0]._point.x = ants[0]._getRandom(old_x - options.delta_r, old_x + options.delta_r);
        ants[0]._point.y = ants[0]._getRandom(old_y - options.delta_r, old_y + options.delta_r);
        ants[0]._value = ants[0]._getValue();
        if (ants[0]._value > old_value) {
            ants[0]._point.x = old_x;
            ants[0]._point.y = old_y;
            ants[0]._value = ants[0]._getValue();
        }
    }

    console.log(JSON.stringify(ants[0], null, 4));
}


*/

void ACO::solve()
{
    vector <Ant *> ants;

    for (int i = 0; i < config.count_of_ants; ++i)
    {
        vector <double> cords = get_random_cords();
        ants.push_back(new Ant(cords, config.fitness(cords)));
    }

    sort(ants.begin(), ants.end(), Ant::compare);

    for (int j = 0; j < config.count_of_good_ants; ++j)
    {
        for (int k = 0; k < dimension; ++k)
        {
            (ants[j]->tau)[k] += config.alpha * config.tau_0 * (config.count_of_good_ants - j - 1); // @todo
        }
    }

    for (int i = 0; i < config.max_iterations; ++i)
    {
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

//        var old_value = ants[0]._value,
//            old_x = ants[0]._point.x,
//            old_y = ants[0]._point.y;
//
//        ants[0]._point.x = ants[0]._getRandom(old_x - options.delta_r, old_x + options.delta_r);
//        ants[0]._point.y = ants[0]._getRandom(old_y - options.delta_r, old_y + options.delta_r);
//        ants[0]._value = ants[0]._getValue();
//        if (ants[0]._value > old_value) {
//            ants[0]._point.x = old_x;
//            ants[0]._point.y = old_y;
//            ants[0]._value = ants[0]._getValue();
//        }


    }

    cout << "x = " << ants[0]->cords[0] << endl << "y = " << ants[0]->cords[1] << endl;
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
