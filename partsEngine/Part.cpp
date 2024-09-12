/*
 * File:   Part.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#include <vector>
#include "Part.h"
#include "config.h"

Part::Part() :
    pos(),
    m(),
    state(false),
    parent(0),
    id(-1) //значит что уникальный ИД не задан
{
    //по умолчанию на частицу ничего не действует
    this->h.x = this->h.y = this->h.z = 0;
    sector = 0; //по умолчанию, все частицы из сектора 0
    this->r = config::Instance()->partR; //по умолчанию радиус задается в конфигах
    this->e = 0;
    this->w1 = this->h1 = 0;
}

Part::Part(const Part &obj):
    pos(obj.pos),
    m(obj.m),
    h(obj.h),
    e(obj.e),
    parent(0),
    id(-1)
{
    this->_vol = obj._vol;
    this->sector = obj.sector;
    this->state = obj.state;
}

bool Part::operator==(const Part &one) const
{
    if (this->pos != one.pos)
        return false;

    if (this->state == one.state){
        return this->m == one.m;
    } else {
        return this->m == (one.m * (-1.));
    }
}

Part::~Part(){
}

Vect Part::interact(const Vect &p) const{
    Vect h;
    double r, r2, r5, part, dx, dy, dz;

    r = p.radius(this->pos).length();
    r2 = r * r; //радиус в квадрате
    r5 = r2 * r2 * r; //радиус в пятой
    dx = this->pos.x - p.x;
    dy = this->pos.y - p.y;
    dz = this->pos.z - p.z;
    part = this->m.x * dx + this->m.y * dy + this->m.z * dz;
    //степени отличаются от формулы потому что обе дроби внесены под общий знаменатель
    h.x = ((3 * part * dx) - this->m.x * r2) / r5;
    h.y = ((3 * part * dy) - this->m.y * r2) / r5;
    h.z = ((3 * part * dz) - this->m.z * r2) / r5;

    //h.absK = config::Instance()->ergGauss/pow(config::Instance()->santiMeter,3.);

    return h;
}

void Part::rotate(bool updateEnergy){
    this->m.rotate();
    this->state = !this->state;
    if (updateEnergy)
        parent->EFastUpdate(this);
    else
        parent->changeState();
}

double Part::volume()
{
    if (config::Instance()->dimensions()==2){
        return M_PI * this->r * this->r;
    } else {
        return (4.0/3.0) * M_PI * this->r * this->r * this->r;
    }
}
