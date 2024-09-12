#ifndef SQUARESPINICEARRAY_H
#define SQUARESPINICEARRAY_H

#include <string>
#include "PartArray.h"
#include "squarespinicecell.h"
#include "Vect.h"
#include "savehelper.h"
#include "loadhelper.h"

using namespace std;

class SquareSpinIceArray : public PartArray
{

public:
    SquareSpinIceArray();
    SquareSpinIceArray(const SquareSpinIceArray &sys);
    virtual ~SquareSpinIceArray();

    SquareSpinIceArray &operator = (const SquareSpinIceArray &sys);

    void dropSpinIce(int hCells, int vCells, double l=1);

    StateMachineFree groundState();
    StateMachineFree maximalState();

    vector<SquareSpinIceCell*> cells;

    virtual void clear();

    virtual void load(std::string file);
    virtual void save(std::string file);

private:
    void clearCells(); //чистим информацию о ячейках
};

#endif // SQUARESPINICEARRAY_H
