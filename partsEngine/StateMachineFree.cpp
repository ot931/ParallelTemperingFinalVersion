#include "StateMachineFree.h"
#include "StateMachine.h"

StateMachineFree::StateMachineFree()
{
    this->_state.clear();
}

StateMachineFree::StateMachineFree(const unsigned long size)
{
    this->_state.resize(size,false);
}

StateMachineFree::StateMachineFree(const StateMachineBase &state)
{
    this->_construct(&state);
}

void StateMachineFree::reset(){
    vector<char>::iterator iter = this->_state.begin();
    while(iter!=this->_state.end()){
        *iter = false;
        iter++;
    }
}

void StateMachineFree::clear()
{
    this->_state.clear();
}

void StateMachineFree::setLast(){
    vector<char>::iterator iter = this->_state.begin();
    while(iter!=this->_state.end()){
        *iter = true;
        iter++;
    }
}

unsigned StateMachineFree::randomize(unsigned count){
    int randnum=0, parts = this->size();
    vector<char>::iterator iter;
    for (unsigned i=0;i<count;i++){
        randnum = rnd::next(parts);
        iter = this->_state.begin()+randnum;
        *iter = !*iter;
    }
    if (count==1)
        return randnum;
    else
        return count;
}

bool StateMachineFree::isFirst(){
    vector<char>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==true)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::isLast(){
    vector<char>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==false)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::isHalfLast(){
    vector<char>::iterator iter = this->_state.begin();
    while (iter!=this->_state.end()-1){
        if (*iter==false)
            return false;
        iter++;
    }
    return true;
}

bool StateMachineFree::next(){
    std::vector<char>::iterator iter;
    iter = this->_state.begin();

    while (iter != this->_state.end()){
        if (*iter==0){
            *iter=1;
            return true;
            break;
        } else {
            *iter=0;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::halfNext(){
    std::vector<char>::iterator iter;
    iter = this->_state.begin();

    while (iter != (this->_state.end()-1)){
        if (*iter==0){
            *iter=1;
            return true;
            break;
        } else {
            *iter=0;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::prev(){
    std::vector<char>::iterator iter;
    iter = this->_state.begin();

    while (iter != this->_state.end()){
        if (*iter==1){
            *iter=0;
            return true;
            break;
        } else {
            *iter=1;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::halfPrev(){
    std::vector<char>::iterator iter;
    iter = this->_state.begin();

    while (iter != (this->_state.end()-1)){
        if (*iter==1){
            *iter=0;
            return true;
            break;
        } else {
            *iter=1;
        }
        iter++;
    }

    return false;
}

bool StateMachineFree::operator++()
{
    return this->next();
}

bool StateMachineFree::operator--()
{
    return this->prev();
}

bool StateMachineFree::operator++(int)
{
    return this->next();
}

bool StateMachineFree::operator--(int)
{
    return this->prev();
}

std::string StateMachineFree::toString() const{
    std::vector<char>::const_iterator iter;
    std::string s="";
    iter = this->_state.begin();
    while (iter!=this->_state.end()){
        if (*iter==0){
            s+="0";
        } else {
            s+="1";
        }
        iter++;
    }
    return s;
}

bool StateMachineFree::fromString(const std::string &s)
{
    this->_state.clear();
    for (unsigned int i=0;i<s.size();i++){
        this->_state.push_back(s[i]!='0');
    }
    return true;
}

StateMachineFree & StateMachineFree::operator= (const StateMachineFree & one){
    this->_state.clear();

    for (unsigned int i=0;i<one.size();i++)
        this->_state.push_back(one[i]);

    return *this;
}

StateMachineFree &StateMachineFree::operator=(const StateMachineBase &one)
{
    unsigned os;
    if (this->_state.size()!=(os=one.size()))
        this->_state.resize(os);

    for (unsigned i=0;i<os;i++)
        this->_state.at(i)=one[i];

    return *this;
}

StateMachineFree &StateMachineFree::operator+=(int val)
{
    for (int i=0;i<val;i++){
        this->next();
    }
    return *this;
}

StateMachineFree StateMachineFree::operator &(const StateMachineBase &one) const
{
        if (one.size()!=this->size())
            throw "operands must be the same size";
        StateMachineFree ret = StateMachineFree(this->size());
        for (unsigned i=0;i<size();i++){
            ret.set(i, one[i] & this->operator[](i));
        }
        return ret;
}

StateMachineFree StateMachineFree::operator ^(const StateMachineBase &one) const
{
    unsigned ts=size();
    if (one.size()!=ts)
        throw "operands must be the same size";
    StateMachineFree ret = StateMachineFree(ts);
    for (unsigned i=0;i<ts;i++){
        ret.set(i, one[i] ^ _state[i]);
    }
    return ret;
}

StateMachineFree & StateMachineFree::operator ^=(const StateMachineBase &one)
{
    unsigned ts=size();
    if (one.size()!=ts)
        throw "operands must be the same size";
    for (unsigned i=0;i<ts;i++){
        _state[i] = _state[i]^one[i];
    }
    return *this;
}

void StateMachineFree::resize(const unsigned long size)
{
    this->_state.resize(size);
}

void StateMachineFree::_construct(const StateMachineBase *state)
{
    this->_state.resize(state->size());
    for (unsigned int i=0;i<state->size();i++){
        this->_state[i]=state->operator[](i);
    }
}


