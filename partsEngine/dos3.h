#ifndef DOS3_H
#define DOS3_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>

#include "dos2.h"

using namespace std;

template <typename T>
class Dos3 : public Dos2<T>
{
public:
    Dos3(double min, double max, unsigned interval)
        { this->data.resize(interval);   }

    virtual void save(string file);
    virtual void load(string file){(void)file;};
    string toString(){return "";};

};

#endif // DOS3_H
