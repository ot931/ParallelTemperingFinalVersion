#include "dos3.h"


template <typename T>
void Dos3<T>::save(string file)
{
    ofstream f(file);
    for (unsigned i=0; i<this->data.size()-1; i++){
        for (auto d : this->data.at(i)){
            f<< this->val(i)<<"\t";
            f<< this->val(i+1)<<"\t";
            f<< d.first <<endl;
        }
    }

    //f << val(data.size()-1)<<"\t";
    //f << max <<"\t";
    //f << data[data.size()-1] <<endl;

    f.close();
}

