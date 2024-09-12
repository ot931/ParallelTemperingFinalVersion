
#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iostream>

class config
{
public:
    //относительные параметры магнитной системы
    double
    partR, //радиус частицы по умолчанию в относительных единицах
    m, //длина вектора магнитного момента частицы по умлочанию, в относительных единицах
    vol, //объем частицы как сферы / площадь для окружности. Вычисляется автоматически.
    hc; //критическое поле перемагничивания,


    //меры перевода в абсолютные величины (на одну единицу относительной величины)
    double
    santiMeter, //Мера расстояния в метрах на одну относительную единицу,
    ergGauss, //мера измерения энергии в ЭРГах/Гаусс на одну относительную еэиницу
    ersted; //мера измерения напряженности магнитного поля (намагниченности) в эрстедах на одну относительную единицу



    //Depticated
    double
    I0s, //намагuниченность насыщения в гаусс,
    dstep, //параметр решетки в сантиметрах (множитель)
    kv; //перевод в сантиметры (для отображения)

    //параметры MPI
    int
    size, //количество потоков MPI
    rank; //номер текущего потока MPI

    int rand_max;


    //переключаем режимы симуляции
    void set2D();
    void set3D();
    unsigned short dimensions(); //возвращает размерность пространства

    //функции рэндома
    int rand(); //генерация рэндом-числа
    void srand(int); //установка начальной точки рэндома
    void randmode_file(const char* file,int srand = 0); //переключить режим ГПСЧ в считывание последовательности с файла
    void randmode_standart(int srand = 0); //переключить в СИ-шный режим рэндома


    //паттерн синглтон
    static config* Instance();


private:
    bool U2D; //Use2Directions, если true - то используется плоскость (2D), иначе пространство (3D)
    static config* _self;
    config();
    virtual ~config();

    void rand_file_open(const char* file);
    void rand_file_close();

    /*режим рэндома
    0 - стандартный
    1 - нативный
    */
    int _randmode;
    std::ifstream _randfile;
};

#endif //CONFIG_H
