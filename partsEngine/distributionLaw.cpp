/*
 * File:   distributionLaw.cpp
 * Author: Отморозок
 *
 * Created on 4 Декабрь 2012 г., 15:29
 */

#include "distributionLaw.h"

distributionLaw::distributionLaw() {

}

int* distributionLaw::calculate(int count = 100) {
    //@todo добавить проверку на наличие элементов в массиве и на то, чтобы элементов было больше чем count

    int* distribution = new int[count];
    //обнуляем значения
    for (int i = 0; i < count; i++) {
        distribution[i] = 0;
    }

    double max, min, step;
    max = this->_max();
    min = this->_min();
    step = (max - min) / (double) (count - 1);

    vector<double>::iterator iter = this->values.begin();
    while (iter != this->values.end()) {
        if (*iter == max) {//в бесконечность и далее! ))
            distribution[count - 2] += 1;
        } else {
            distribution[(int) floor((*iter - min) / step)] += 1; //проверяем остальные элементы из интервала
        }
        iter++;
    }
    return distribution;
}

double* distributionLaw::getLables(int count = 100) {
    double* lables = new double[count];
    double max, min, step;
    min = this->_min();
    max = this->_max();
    step = (max - min) / (double) (count - 1);

    for (int i = 0; i < count; i++) {
        lables[i] = min + step * (double) i;
    }
    return lables;
}

double distributionLaw::_min() {
    double min;
    vector<double>::iterator iter = this->values.begin();
    min = *iter;
    iter++;
    while (iter != this->values.end()) {
        if (*iter < min) min = *iter;
        iter++;
    }
    return min;
}

double distributionLaw::_max() {
    double max;
    vector<double>::iterator iter = this->values.begin();
    max = (*iter);
    iter++;
    while (iter != this->values.end()) {
        if (*iter > max) max = *iter;
        iter++;
    }
    return max;
}
