#ifndef PYROCHLORELATTICE_H
#define PYROCHLORELATTICE_H

#include <vector>
#include "PartArray.h"
#include "Vect.h"
#include "Part.h"

using namespace std;


class PyrochloreLattice : public PartArray
{
public:
    PyrochloreLattice();

    void dropLattice(double l=1, Vect size=Vect(1,1,1));

    forward_list<Part*> neightbour(unsigned id){return this->neighbours[id];}

protected:
    //virtual bool isNeighbours(const Part *_a, const Part *_b) const;
    Vect totalSize;

};

#endif // PYROCHLORELATTICE_H
