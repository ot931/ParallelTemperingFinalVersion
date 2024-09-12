#include "statemachinebase.h"

StateMachineBase::StateMachineBase()
{

}

StateMachineBase::~StateMachineBase()
{

}

bool StateMachineBase::at(const unsigned long num) const
{
    return this->operator [](num);
}

bool StateMachineBase::operator==(const StateMachineBase &one) const
{
    if (one.size()!=this->size()) return false;
    if (one.size()==0 || one.size()==1) return true;

    bool compEquals = (one[0]==(*this)[0]);
    for (unsigned long i=0; i<this->size(); i++){
        if ( (one[i]==(*this)[i]) != compEquals)
            return false;
    }

    return true;
}

bool StateMachineBase::operator!=(const StateMachineBase &one) const
{
    return !((*this)==one);
}

unsigned long StateMachineBase::rotatedCount()
{
    unsigned long rot=0;
    for (unsigned i=0; i<size(); i++){
        if (operator [](i))
            rot++;
    }
    if (rot>size()/2)
        rot = size()-rot;
    return rot;
}


