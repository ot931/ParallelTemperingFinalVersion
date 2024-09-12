#ifndef CAGOMELATTICE_H
#define CAGOMELATTICE_H

#include <vector>
#include "PartArray.h"
#include "Vect.h"
#include "Part.h"

using namespace std;


class Cagomelattice : public PartArray
{
public:
    Cagomelattice();

    void dropLattice(double l=1, Vect size=Vect(1,1,0));
    void dropLattice2(double l=1, Vect size=Vect(1,1,0));
    void dropSymmetricLattice(double l=1, Vect size=Vect(1,0,0));

    forward_list<Part*> neightbour(unsigned id){return this->neighbours[id];}

protected:
    //virtual bool isNeighbours(const Part *_a, const Part *_b) const;
    Vect totalSize;

};

#endif // CAGOMELATTICE_H
