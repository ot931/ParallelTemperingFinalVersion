#ifndef SQUAREISINGLATTICE_H
#define SQUAREISINGLATTICE_H

#include "squarelattice.h"

class SquareIsingLattice : public SquareLattice
{
public:
    SquareIsingLattice();
    void dropSquareLattice(int m, int n);

    StateMachineFree groundState();
    StateMachineFree maximalState();

private:
    //прячем наследованные функции от изменения
    void hamiltonianDipolar();
    void setInteractionRange(const double range);
};

#endif // SQUAREISINGLATTICE_H
