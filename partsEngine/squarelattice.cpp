#include "squarelattice.h"

SquareLattice::SquareLattice(): PartArray(),
    m(0),
    n(0),
    l(1)
{
    _type = "squarelattice";
}

void SquareLattice::dropSquareLattice(int m, int n, double l, double len, Part *example)
{
    this->clear(); //очищаем систему
    this->reserveParts(unsigned(m*n));

    //сохраняем параметры решетки
    this->m = m;
    this->n = n;
    this->l = l;

    if (example==0){ //если образец не задан, делаем его пустым по умолчанию
        example = new Part();
    }

    Part* temp; //временная частица
    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++){
            temp = new Part(*example); //создаем копию объекта
            temp->m = Vect(0,len,0);
            temp->pos = Vect(i*l, j*l, 0);
            this->insert(temp);
        }
    }
}

StateMachineFree SquareLattice::groundState()
{
    StateMachineFree oldState = state, newState; //сохраняем текущее состояние

    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    int i=0, j=0;
    while (iter != parts.end()){ //обходим все частицы
        temp = *iter;
        if ((temp->m.scalar(Vect(0,1,0))>0) ^ (i%2==1))
            temp->rotate();

        if ((++j) == n){
            j=0; i++;
        }
        iter++;
    }

    newState = state; //сохраняем состояние минимума
    state = oldState; //возвращаем начальное состояние

    return newState;
}

StateMachineFree SquareLattice::maximalState()
{
    StateMachineFree oldState = state, newState;
    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    int i=0, j=0;
    while (iter != parts.end()){
        temp = *iter;
        if ((temp->m.scalar(Vect(0,1,0))>0) ^ (j%2==1))
            temp->rotate();

        if ((++j) == n){
            j=0; i++;
        }
        iter++;
    }

    newState = state;
    state = oldState;

    return newState;
}

void SquareLattice::load(std::string file)
{
    //грузим основной набор частиц
    PartArray::load(file);

    LoadHelper helper(file);

    //пропускаем до своей секции
    helper.go("square");

    //читаем три своих поля
    helper>>this->m;
    helper>>this->n;
    helper>>this->l;

    //закрываем файл
    helper.close();
}

void SquareLattice::save(std::string file)
{
    //Сохраняем основную часть файла
    PartArray::save(file);
    SaveHelper helper(file);

    //пишем секцию
    helper.go("square");

    //пишем свои данные
    helper<<this->m;
    helper<<this->n;
    helper<<this->l;

    //закрываем файл
    helper.close();
}

Part *SquareLattice::at(int i, int j)
{
    return this->operator [](i*n+j);
}

