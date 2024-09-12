#ifndef AMORPHOUSARRAY_H
#define AMORPHOUSARRAY_H

#include "PartArray.h"
#include "savehelper.h"

class AmorphousArray : public PartArray
{
public:
    AmorphousArray();
    /*
    void dropRandom(double dencity);
    void dropRandom(unsigned count);*/



};
    /*рассчитывает плотность массива
    если simple=true, то считать только исходя из количества частиц и объема одной частицы, иначе
    рассчитывать плтность суммируя объемы всех частиц
    double destiny(bool simple=true);
    */
#endif // AMORPHOUSARRAY_H
