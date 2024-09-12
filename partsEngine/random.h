#ifndef RANDOM_H
#define RANDOM_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
namespace rnd {

extern int (*_rand)(); //функция - рэндом
extern void (*_srand)(unsigned int); //функция - cид
extern unsigned long long randmax;

/**
 * @brief next Cлучайное число в интервале от minValue (включительно) до maxValue (исключительно)
 * @param minValue Минимальное значение случайного числа.
 * @param maxValue Максимальное значение случайного числа.
 * @return озвращает случайное число. Например, если minValue=1, maxValue=4, то вернет одно из чисел 1,2,3
 */
int next(const int minValue,const int maxValue);

/**
 * @brief next Cлучайное число в интервале от 0 (включительно) до maxValue (исключительно)
 * @param maxValue Максимальное значение случайного числа.
 * @return Возвращает случайное число. Например, если maxValue=3, то вернет одно из чисел 0,1,2
 */
int next(const int maxValue);

int next();
unsigned long int max();
void srand(unsigned int seed);

double nextDouble();

}

#endif // RANDOM_H
