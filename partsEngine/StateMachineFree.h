#ifndef STATEMACHINEFREE_H
#define STATEMACHINEFREE_H

#include <vector>
#include <string>
#include "config.h"
#include "statemachinebase.h"

using namespace std;

class StateMachine;
/**
 * @brief The StateMachineFree class представляет собой класс, не привязанный к какой-либо системе.
 * Поддерживает стандартное представление StateMachine и умеет копировать из него состояния
 */
class StateMachineFree: public StateMachineBase
{
public:
    StateMachineFree();

    StateMachineFree(const unsigned long int size);

    StateMachineFree(const StateMachineBase &state);


    virtual void reset();
    virtual void clear();
    virtual void setLast();
    virtual unsigned randomize(unsigned count=1);
    virtual bool isFirst();
    virtual bool isLast();
    virtual bool isHalfLast();
    virtual bool next();
    virtual bool halfNext();
    virtual bool prev();
    virtual bool halfPrev();
    virtual bool operator++();
    virtual bool operator--();
    virtual bool operator++(int);
    virtual bool operator--(int);
    virtual std::string toString() const;
    virtual bool fromString(const std::string&);
    inline virtual bool operator [](const unsigned long int num) const { return (bool)this->_state[num]; }
    inline virtual bool set(const unsigned long int num, bool val){
        return (this->_state[num] = val);
    }
    inline virtual unsigned long int size() const {return this->_state.size();}

    StateMachineFree & operator= (const StateMachineFree & one);
    StateMachineFree & operator= (const StateMachineBase & one);

    StateMachineFree &  operator+=(int val);

    StateMachineFree operator & (const StateMachineBase & one) const;
    StateMachineFree operator ^ (const StateMachineBase & one) const;
    StateMachineFree & operator ^= (const StateMachineBase & one);

    void resize(const unsigned long int size);

protected:
    virtual void _construct(const StateMachineBase *state);
    std::vector<char> _state;
};

#endif // STATEMACHINEFREE_H
