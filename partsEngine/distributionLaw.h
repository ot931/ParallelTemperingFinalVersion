#ifndef DISTRIBUTIONLAW_H
#define DISTRIBUTIONLAW_H

#include <vector>
#include <cmath>

using namespace std;
//template <typename arrType> class distributionLaw {
class distributionLaw {
public:
    distributionLaw();
    //distributionLaw(int count, arrType array);
    int* calculate(int count);
    double* getLables(int count);
    vector<double> values; //значения, для которых рассчитан закон распределения
    int* counts; //массив количества попаданий значения в интервал
    double* probabilities;

private:
    double _max();
    double _min();
};

#endif //DISTRIBUTIONLAW_H
