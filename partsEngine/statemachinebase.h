#ifndef STATEMACHINEBASE_H
#define STATEMACHINEBASE_H

#include <string>
#include <vector>

class StateMachineBase
{
public:
    StateMachineBase();
    virtual ~StateMachineBase();

    /**
     * @brief reset сбросить состояние в начальное, т.е. все состояния по нулям
     */
    virtual void reset()=0;


    virtual void setLast()=0;

    /**
     * @brief randomize Совершает несколько случайных переворотов системы
     * @param count Количество переворотов которые нужно совершить
     * @return если перевернуть одну частицу, возвращает ее номер, иначе count. При ошибке возвращает -1.
     */
    virtual unsigned randomize(unsigned count=1)=0;

    /**
     * @brief isFirst Проверяет, находится ли система в начальном состоянии
     * @return true если поворот всех спинов - 0.
     */
    virtual bool isFirst()=0;
    virtual bool isLast()=0;
    virtual bool isHalfLast()=0;

    /**
     * @brief next переключиться на следующее состояние
     * @return false если система возвращается в начальное состояние
     */
    virtual bool next()=0;

    /**
     * @brief halfNext аналог next, но перебирается только половина состояний
     * @return false если система прошла половину состояний
     */
    virtual bool halfNext()=0;

    virtual bool prev()=0;

    virtual bool halfPrev()=0;

    virtual bool operator++()=0;
    virtual bool operator--()=0;
    virtual bool operator++(int)=0;
    virtual bool operator--(int)=0;

    virtual std::string toString() const=0;

    virtual bool fromString(const std::string&)=0;

    virtual bool operator[](const unsigned long int) const=0;
    virtual bool at(const unsigned long int) const;

    virtual unsigned long int size() const=0;

    virtual bool operator==(const StateMachineBase &one) const;
    virtual bool operator!=(const StateMachineBase &one) const;

    /*!
     * \brief rotatedCount Число перевернутых спинов от 0 до N/2
     * \return Число перевернутых спинов от 0 до N/2
     */
    unsigned long int rotatedCount();
};

#endif // STATEMACHINEBASE_H
