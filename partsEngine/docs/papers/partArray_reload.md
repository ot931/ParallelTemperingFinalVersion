# Перегрузка PartArray
Готовая реализация примера доступна в файлах [squarelattice.h](https://github.com/uyras/partsEngine/blob/master/squarelattice.h) и [squarelattice.cpp](https://github.com/uyras/partsEngine/blob/master/squarelattice.cpp)

Для определения нового формата магнитной системы необходимо перегрузить основной класс PartArray.
Перегрузка класса нужна для описания сложных решеток, их закономерностей и проведения экспериментов над этими решетками.
# Основной каркас класса
Например, мы хотим создать двумерные цепочки частиц. Назовем его SquareLattice.
Для этого создадим файл squarelattice.h, где перегрузим базовый:
```cpp
#ifndef SQUARELATTICE_H
#define SQUARELATTICE_H

#include "PartArray.h"
#include "Part.h"

class SquareLattice : public PartArray
{
public:
    SquareLattice();

private:
    int m,n;
    double l;
};

#endif // SQUARELATTICE_H
```

Добавим файл реализации класса squarelattice.cpp:
```cpp
#include "squarelattice.h"

SquareLattice::SquareLattice():
    m(0),
    n(0),
    l(1)
{
    _type = "squarelattice"; //резервируем имя типа массива для корректной идентификации. По большей части нужно при массива в файл.
}
```

Так мы создали класс, определяющий свой тип массива частиц. В классе добавлена функция dropSquareLattice как пример генерирующей функции, однако она пока пустая. Таких функций может быть сколько угодно, и данные они могут генерировать какие угодно. Все зависит от реализации класса. 
# Генерация решетки
Давайте сделаем набросок основной функции генерации решетки. 

Договоримся, что диполи будут направлены вверх либо вниз и будут расположены в узлах квадратной сетки размером _m_ на _n_ узлов. Параметр решетки будет _l_. Все магнитные моменты будут определенной длины _len_. Для этого в заголовочный файл squarelattice.h добавим следующее определение:

```cpp
    void dropSquareLattice(int m, int n, double l=1, double len=1, Part * example=0);
```

Здесь Part * example - Шаблон добавляемой частицы. В процессе создания системы будут перезаписываться координаты и направление магнитного момента (длина сохраняется).

А код функции будет следующим:
```cpp
void SquareLattice::dropSquareLattice(int m, int n, double l, double len, Part *example)
{
    this->clear(); //очищаем систему

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
```
Таким образом, мы создали простой класс генерации квадратных решеток.

Однако, в этом классе не хранятся дополнительные поля и структуры данных. В связи с этим при сохранении и загрузке они не будут попадать в файл и не будут загружаться из файл (например, поля класса m,n и l). Далее мы будем усложнять эту структуру.

## Привод в минимум и максимум энергии
Много функций библиотеки требуют для своей работы быстрые функции привода энергии в минимум и максимум.
По умолчанию, PartArray::setToGroundState() перебирает все возможные конфигурации системы и выбирает минимальную из них по энергии. Кроме того, этот метод отслеживает, есть ли уже такая конфигурация в памяти. Если есть, просто применяет её.

Однако, в решетках различных типов возможны закономерные состояния, которые приведут систему в минимум намного быстрее чем полный перебор. Приведем пример такой реализации. Для этого перегрузим унаследованный метод PartArray::groundState() и PartArray::maximalState().

```cpp
StateMachineFree SquareLattice::groundState()
{
    StateMachineFree oldState = (*state), newState; //сохраняем текущее состояние

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

    newState = (*state); //сохраняем состояние минимума
    (*state) = oldState; //возвращаем начальное состояние

    return newState;
}
```
По аналогии перегружаем PartArray::maximalState():
```cpp
StateMachineFree SquareLattice::maximalState()
{
    StateMachineFree oldState = (*state), newState;
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

    newState = (*state);
    (*state) = oldState;

    return newState;
}
```

## Сохранение файла
По умолчанию в файл сохраняются все данные из базового класса PartArray. Подробное описание формата можно найти [здесь](https://github.com/uyras/partsEngine/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0-%D0%BC%D0%B0%D0%B3%D0%BD%D0%B8%D1%82%D0%BD%D0%BE%D0%B9-%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%8B).
Для сохранения необходимо перегрузить базовую функцию PartArray::save(QString).
Согласно [формату](https://github.com/uyras/partsEngine/wiki/%D0%A4%D0%BE%D1%80%D0%BC%D0%B0%D1%82-%D1%84%D0%B0%D0%B9%D0%BB%D0%B0-%D0%BC%D0%B0%D0%B3%D0%BD%D0%B8%D1%82%D0%BD%D0%BE%D0%B9-%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%8B), в начале файла идут секции основных переменных и частиц. За ее формирование отвечает код основного класса PartArray::save(). Он перезаписывает указанный файл (или создает новый при отсутствии такового).
Для дописывания просто нужно открыть этот файл в режиме добавления и дописать свои данные.
В нашем случае, нам необходимо сохранить всего три параметра m,n и l. Определим для них секцию [square] и запишем данные:
```cpp
//Сохраняем основную часть файла
    PartArray::save(file);

    //Открываем в режиме добавления
    ofstream f(file.toStdString().c_str(), ios_base::app | ios_base::out);

    //пишем секцию
    f<<"[square]"<<endl;

    //пишем свои данные
    f<<this->m<<endl;
    f<<this->n<<endl;
    f<<this->l<<endl;

    //закрываем файл
    f.close();
```
## Загрузка из файла
В общем случае, механизм загрузки файла определяет тип системы и определяет связанный с ним класс. Следовательно, у каждого типа магнитной системы должно быть свое уникальное имя. Мы его уже задали в конструкторе. Однако, его еще нужно зарегистрировать в файловом загрузчике. Для этого немного изменим конструктор:
```cpp
SquareLattice::SquareLattice():
    m(0),
    n(0),
    l(1)
{
    _type = "squarelattice";
    SysLoader::reg<SquareLattice>(type());
}
```
Теперь загрузчик знает что система с именем "squarelattice" связана с классом "SquareLattice", и при загрузке системы нужно звать конструктор именно этого класса. Например, так:
```cpp
SquareLattice sys1;
sys1.dropSquareLattice(3,3);
sys1.save("square.dat");
SquareLattice *sys2 = (SquareLattice*)SysLoader::load("square.dat");
sys2->save("square2.dat");
delete sys2;
```
Однако, в данном случае в sys2 не загрузятся m,n и l поля.
Для этого перегрузим функцию PartArray::load(QString) в своем классе:
```cpp
void SquareLattice::load(QString file)
{
    this->clear(); //чистим систему
    
    //грузим основной набор частиц
    PartArray::load(file);

    //читаем файл
    QFile infile(file);
    infile.open(QFile::ReadOnly);
    QTextStream f(&infile);

    //пропускаем до своей секции
    QString s;
    while (s!="[square]"){
        s = f.readLine();
    }

    //читаем три своих поля
    f>>this->m;
    f>>this->n;
    f>>this->l;

    //закрываем файл
    infile.close();
}
```
Готово! Теперь все данные будут корректно загружаться из файла.