/*
 * File:   Vect.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:55
 */

#include "config.h"
#include "Vect.h"
#include "Part.h"
#include <locale.h>

using namespace std;

Vect::Vect() {
	this->x = 0;
	this->y = 0;
    this->z = 0;
}

Vect::Vect(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vect::setUnitary(){
	double length = this->length();
	this->x /= length;
	this->y /= length;
    if (config::Instance()->dimensions()==3)
        this->z /= length;
}

Vect Vect::normalize()
{
    double length = this->length();
    return Vect(this->x / length, this->y / length, this->z / length);
}

void Vect::normalizeItself()
{
    double length = this->length();
    this->x /= length; this->y /= length; this->z /= length;
}


double Vect::scalar(Vect b) {
	return (this->x * b.x) + (this->y * b.y) + (this->z * b.z);
}

/**
 * Находит расстояние между двумя векторами
 * @param a
 * @param b
 * @return
 */
double Vect::space(const Vect &b) const {
    if (config::Instance()->dimensions()==2)
		return sqrt(
					(this->x - b.x)*(this->x - b.x) +
					(this->y - b.y)*(this->y - b.y)
					);
	else
		return sqrt(
					(this->x - b.x)*(this->x - b.x) +
					(this->y - b.y)*(this->y - b.y) +
					(this->z - b.z)*(this->z - b.z)
                    );
}

double Vect::space_2(const Vect &b) const
{
    if (config::Instance()->dimensions()==2)
        return
                    (this->x - b.x)*(this->x - b.x) +
                    (this->y - b.y)*(this->y - b.y)
                    ;
    else
        return
                    (this->x - b.x)*(this->x - b.x) +
                    (this->y - b.y)*(this->y - b.y) +
                    (this->z - b.z)*(this->z - b.z)
                    ;
}

Vect Vect::radius(const Vect &b) const {
	return Vect(b.x - this->x, b.y - this->y, b.z - this->z);
}

double Vect::length() const {
    if (config::Instance()->dimensions()==2)
		return sqrt(
					this->x * this->x +
					this->y * this->y
					);
	else
		return sqrt(
					this->x * this->x +
					this->y * this->y +
					this->z * this->z
					);
}

void Vect::setXYZ(double x, double y, double z = 0) {
	this->x = x;
	this->y = y;
	this->z = z;
	//this->_length = NULL;
}

void Vect::rotate() {
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
}

Vect Vect::operator=(const Vect& a) {
	this->x = a.x;
	this->y = a.y;
	this->z = a.z;
    return Vect(this->x,this->y,this->z);
}

Vect Vect::operator+=(const Vect& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
    return *this;
}

Vect Vect::operator +(const Vect& a) const
{
	Vect res;
	res.x = this->x+a.x;
	res.y = this->y+a.y;
	res.z = this->z+a.z;

    return res;
}

Vect Vect::operator-(const Vect & a) const
{
    Vect res;
    res.x = this->x-a.x;
    res.y = this->y-a.y;
    res.z = this->z-a.z;

    return res;
}

Vect Vect::operator*(const double num) const
{
    return Vect(
                this->x*num,
                this->y*num,
                this->z*num
                );
}

Vect Vect::operator*=(const double num)
{
    this->x*=num;
    this->y*=num;
    this->z*=num;
    return *this;
}

Vect Vect::operator/(const double num) const
{
    return Vect(
                this->x/num,
                this->y/num,
                this->z/num
                );
}

Vect Vect::operator/=(const double num)
{
    this->x/=num;
    this->y/=num;
    this->z/=num;
    return *this;
}

bool Vect::operator==(const Vect & a) const
{
    if (config::Instance()->dimensions()==2){
        return (this->x==a.x && this->y==a.y);
    } else {
        return (this->x==a.x && this->y==a.y && this->z==a.z);
    }
}

Vect Vect::crossProduct(const Vect &vect1, const Vect &vect2)
{
    return Vect(
                vect1.y*vect2.z-vect1.z*vect2.y,
                vect1.z*vect2.x-vect1.x*vect2.z,
                vect1.x*vect2.y-vect1.y*vect2.x
                );
}

Vect Vect::normal(const Vect &vect1, const Vect &vect2)
{
    return Vect::crossProduct(vect1,vect2).normalize();
}

void Vect::toAbs(){
	this->x *= config::Instance()->vol;
	this->y *= config::Instance()->vol;
	this->z *= config::Instance()->vol;
}

void Vect::toRel(){
	this->x /= config::Instance()->vol;
	this->y /= config::Instance()->vol;
	this->z /= config::Instance()->vol;
}

void Vect::draw() const{
    cout<<"x="<<this->x<<"; y="<<this->y<<"; z="<<this->z<<"; l="<<this->length()<<endl;
}

double Vect::angle(){
    if(this->x>0&&this->y>=0)
        return atan(this->y/this->x);
    if (this->x>0&&this->y<0)
        return atan(this->y/this->x)+M_PI*2;
    if(this->x<0)
        return atan(this->y/this->x)+M_PI;
    if(this->x==0&&this->y>0)
        return M_PI_2;
    if (this->x==0&&this->y<0)
        return M_PI_2*3;
    if(this->x==0&&this->y==0)
        return 0.;
    return 0.;
}

double Vect::grade(){
    return this->angle()*180/M_PI;
}

ostream& operator<<(ostream & os, const Vect& p){
    os<<"("<<p.x<<";"<<p.y;
    if (config::Instance()->dimensions()==3){
        os<<";"<<p.z;
    }
    os<<")";
    return os;
}
