#ifndef SQUARESPINICECELL_H
#define SQUARESPINICECELL_H

#include <vector>
#include <iostream>
#include "Part.h"

using namespace std;

class Part;

struct oneCell{ //ячейка нужна для определения конфигураций
    Vect
    top, bottom, left, right;
    unsigned short int type;
};

/**
 * @brief The SpinIceCell class Одна ячейка спинового льда
 */
class SquareSpinIceCell
{

public:
    SquareSpinIceCell();
    virtual ~SquareSpinIceCell();

    int type();

    Part
    *top,
    *bottom,
    *left,
    *right;
    Vect pos;
    int column,row;

private:
    int types[16][4][4];
};

#endif // SQUARESPINICECELL_H
