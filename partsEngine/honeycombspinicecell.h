#ifndef HONEYCOMBSPINICECELL_H
#define HONEYCOMBSPINICECELL_H

#include "Part.h"
#include "Vect.h"
#include <vector>

class HoneycombSpinIceArray;

using namespace std;
class HoneycombSpinIceCell
{
    friend class HoneycombSpinIceArray; //сделать недоступным класс из других кусков кода
private:
    HoneycombSpinIceCell();
    ~HoneycombSpinIceCell();


    void rotateClockWise();
    void rotateAntiClockWise();
    void rotateChaotic();

    vector<Part*> parts;
    Vect pos;
};

#endif // HONEYCOMBSPINICECELL_H
