#ifndef HC_FLOWER_H
#define HC_FLOWER_H

#include <vector>
#include "PartArray.h"
#include "Vect.h"
#include "Part.h"

using namespace std;


class HC_Flower : public PartArray
{
public:
    HC_Flower();

    void dropSymmetricLattice(double l=1, Vect size=Vect(1,1,0));

    forward_list<Part*> neightbour(unsigned id){return this->neighbours[id];}

protected:
    //virtual bool isNeighbours(const Part *_a, const Part *_b) const;
    Vect totalSize;

};

#endif // HC_FLOWER_H
