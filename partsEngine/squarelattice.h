#ifndef SQUARELATTICE_H
#define SQUARELATTICE_H

#include <string>
#include "PartArray.h"
#include "Part.h"
#include "savehelper.h"
#include "loadhelper.h"


class SquareLattice : public PartArray
{
public:
    SquareLattice();

    /**
     * @brief dropSquareLattice Метод генерации массива квадратной решетки
     * @param m Количество частиц по горизонтали
     * @param n Количество частиц по вертикали
     * @param l Расстояние между соседними решетками
     * @param len длина вектора намагниченности по умолчанию
     * @param example Шаблон добавляемой частицы. Будут перезаписываться координаты и направление магнитного момента (длина сохраняется)
     */
    void dropSquareLattice(int m, int n, double l=1, double len=1, Part * example=0);

    StateMachineFree groundState();
    StateMachineFree maximalState();

    virtual void load(std::string file);
    virtual void save(std::string file);

    Part* at(int i, int j);

    int m,n;

private:
    double l;
};

//SysLoader::reg(new SquareLattice());

#endif // SQUARELATTICE_H
