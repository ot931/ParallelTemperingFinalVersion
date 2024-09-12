#include "random.h"

int (*rnd::_rand)() = std::rand; //функция - рэндом
void (*rnd::_srand)(unsigned int) = std::srand; //функция - cид
unsigned long long rnd::randmax = RAND_MAX;

int rnd::next(const int minValue, const int maxValue){return rnd::_rand()%(maxValue-minValue)+minValue;}

int rnd::next(const int maxValue){ return next(0,maxValue); }

int rnd::next() {return rnd::_rand();}

unsigned long rnd::max() {return rnd::randmax; }

void rnd::srand(unsigned int seed) { rnd::_srand(seed); }

double rnd::nextDouble(){return (double)(rnd::_rand()) / (double)(rnd::randmax);}
