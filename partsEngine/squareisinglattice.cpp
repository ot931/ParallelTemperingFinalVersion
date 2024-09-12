#include "squareisinglattice.h"

SquareIsingLattice::SquareIsingLattice():
    SquareLattice()
{
    this->SquareLattice::setInteractionRange(1.1);
    this->setHamiltonian(hamiltonianIsing);
}

void SquareIsingLattice::dropSquareLattice(int m, int n)
{
    this->SquareLattice::dropSquareLattice(m,n);
//замыкаем соседей
    Part* temp1, *temp2;
    for (int i=0; i<m; i++){
        temp1 = this->at(i,0);
        temp2 = this->at(i,n-1);
        neighbours[temp1->Id()].push_front(temp2);
        neighbours[temp2->Id()].push_front(temp1);
    }
    for (int i=0; i<n; i++){
        temp1 = this->at(0,i);
        temp2 = this->at(m-1,i);
        neighbours[temp1->Id()].push_front(temp2);
        neighbours[temp2->Id()].push_front(temp1);
    }
}

StateMachineFree SquareIsingLattice::groundState()
{
    StateMachineFree newState(this->size()); //сохраняем текущее состояние

    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    while (iter != parts.end()){ //обходим все частицы
        temp = *iter;
        if (temp->m.scalar(Vect(0,1,0))<0) //все верх
            newState.set(temp->Id(), !temp->state);
        else
            newState.set(temp->Id(), temp->state);
        iter++;
    }

    return newState;
}

StateMachineFree SquareIsingLattice::maximalState()
{
    StateMachineFree newState(this->size());
    Part* temp; //временная частица

    vector<Part*>::iterator iter = parts.begin();
    int i=0, j=0;
    while (iter != parts.end()){
        temp = *iter;
        if ((temp->m.scalar(Vect(0,1,0))>0) ^ ((j+i)%2==1))
            newState.set(temp->Id(), !temp->state);
        else
            newState.set(temp->Id(), temp->state);

        if ((++j) == n){
            j=0; i++;
        }
        iter++;
    }

    return newState;
}
