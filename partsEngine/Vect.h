#ifndef VECT_H
#define VECT_H

#include <iostream>

using namespace std;

class Vect {
public:
    double x, y, z; //основные параметры векторов

    Vect(); //конструктор нулевого вектора
    Vect(double x, double y, double z); //конструктор направленного вектора

    double scalar(Vect); //Скалярное произведение двух векторов

    double space(const Vect &b) const; //расстояние между векторами
    double space_2(const Vect &b) const; //расстояние между векторами в квадрате
    Vect radius(const Vect &b) const; //возвращает радиус-вектор
    double length() const; //длина вектора
    void setXYZ(double x, double y, double z); //задать вектор
    void rotate(); //вращать вектор на 180 градусов

    void toAbs(); //переводит вектор в абсолютные величины из относительных

    void toRel(); //переводит вектор в относительные величины из абсолютных (не точно, что работает)
    void draw() const; //выводит вектор на экран

    void setUnitary(); //делает вектор едииничной длины
    Vect normalize(); //возвращает нормализованный вектор
    void normalizeItself(); //нормирует себя на единицу

    double angle(); //возвращает угол наклона полярной системы координат в радианах, работает только в 2D
    double grade(); //возвращает угол наклона полярной системы координат в градусвх

    Vect operator=(const Vect&); //присваивание вектора
    Vect operator+=(const Vect&); //оператор сложения
    Vect operator+(const Vect&) const;
    Vect operator-(const Vect&) const;
    Vect operator*(const double num) const;
    Vect operator*=(const double num);
    Vect operator/(const double num) const;
    Vect operator/=(const double num);
    bool operator==(const Vect&) const;
    inline bool operator!=(const Vect& a) const {return !this->operator ==(a);}
    static Vect crossProduct(const Vect& vect1, const Vect& vect2);  //векторное произведение двух векторов
    static Vect normal(const Vect& vect1, const Vect& vect2); //нормаль к плоскости векторов

    friend ostream &operator<<(ostream & os, const Vect& p);
};

#endif // VECT_H
