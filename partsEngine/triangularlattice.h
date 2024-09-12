#ifndef triangularlattice_h
#define triangularlattice_h

#include <vector>
#include "PartArray.h"
#include "Vect.h"
#include "Part.h"

using namespace std;


class Triangularlattice : public PartArray
{
public:
    Triangularlattice();

    void dropLattice(double l=1, Vect size=Vect(1,1,0));
    void dropLattice2(double l=1, Vect size=Vect(1,1,0));
    void dropSymmetricLattice(double l=1, Vect size=Vect(1,0,0));

    forward_list<Part*> neightbour(unsigned id){return this->neighbours[id];}

protected:
    //virtual bool isNeighbours(const Part *_a, const Part *_b) const;
    Vect totalSize;

};

#endif // triangularlattice_h
