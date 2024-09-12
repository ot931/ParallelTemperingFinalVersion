/*
* File:   PartArray.h
* Author: uyras
*
* Created on 20 Ноябрь 2012 г., 17:22
*/

#ifndef PARTARRAY_H
#define PARTARRAY_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <string>
#include <sstream>
#include <map>
#include "Part.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include "loadhelper.h"
#include "savehelper.h"

using namespace std;
class Part;
class SysLoader;

double hamiltonianDipolar(Part* a, Part* b);

double hamiltonianIsing(Part* a, Part* b);

/**
 * @brief The PartArray class
 */

class PartArray {
    friend class SysLoader;
    friend class StateMachine;
    friend void LoadHelper::applyHeader(PartArray *, bool);
    friend void SaveHelper::writeHeader(PartArray *);
public:

    PartArray();
    /**
     * @brief PartArray Конструктор копирования класса
     * @param sys Копируемый класс
     *
     * Копирует список частиц минимальное, максимальное и текущее состояние, их энергии, строку типа системы,
     * радиус взаимодействия и гамильтониан.
     * Таблица энергий, история изменения состояний не копируется.
     * Список соседей создается заново в процессе копирования
     * В результате операции новая система будет идентична старой.
     */
    PartArray(const PartArray &sys);
    virtual ~PartArray();

    PartArray& operator= (const PartArray& sys);
    Part* operator[](const unsigned num);
    virtual bool operator==(const PartArray& sys) const;
    inline bool operator!=(const PartArray& sys) const{return !this->operator ==(sys);}

    ///получить частицу по уникальному идентификатору
    Part* getById(unsigned id);

    /**
     * @brief findByPosition Найти частицу по ее позиции
     * @param pos Позиция частицы, которую надо найти
     * @param epsilon Точность поиска
     * @return 0 если не найдена, ссылку на найденную частицу в случае нахождения
     */
    Part *findByPosition(const Vect& pos, double epsilon=1e-15);

    void insert(Part* part); //Добавляет частицу на образец
    void insert(const Part &part); //Добавляет копию частицы в образец
    void add(Part* part);

    virtual void dropTetrahedron(int x, int y, int z, double R = 1, Part * tmp = 0);

    /**
     * @brief setInteractionRange устанавливает радиус взаимодействия вокруг частицы.
     * Две частицы являются соседями, если радиус между ними меньше или равен установленному.
     * @param range Максимальный радиус. 0 - если взаимодействие "все со всеми"
     */
    void setInteractionRange(const double range);
    inline double interactionRange() const { return _interactionRange; }
    virtual bool isNeighbours(const Part* _a, const Part* _b) const;
    //virtual bool isNeighbours(double x, double y, const Part* _b) const;


    void setNeighbours(Part *_a, Part *_b);

    void shuffleM(); //хаотично развернуть магнитные моменты случайных частиц

    /**
     * @brief calcM1 считает общий магнитный момент системы
     * @return вектор намагниченности (сумма всех намагниченностей частиц)
     */
    Vect calcM1();
    Vect M();

	/**
	 * @brief calcM2 Считает сумму модулей магнитных моментов
	 * @return
	 */
    Vect calcM2();

	/**
	 * @brief calcM12 считает относительную сумму векторов (вектор / вектор по модулю)
	 * @return возвращает каждый вектор
	 */
	Vect calcM12();

    /**
    * Рассчитать поле взаимодействия для указанной частицы и записывает её в параметр Interaction
    * @param elem ссылка на элемент, над которым производить взаимодействие
    */
    void calcH(Part* elem);

    /**
    * рассчитывает поле взаимодействия для всех частиц в системе (каждой в параметрах прописывает поле взаимодействия)
    */
    void calcH();

    /**
    * Считать поле взаимодействия только для соседних 8 элементов
    * @param elem
    * @return
    */
    Vect calcInteractionNeighb(Part* elem);


    //энергии системы
    /**
     * @brief E Считает энергию системы. Метод подбирается автоматически исходя из конфигурации и свойств системы
     * @return
     */
    virtual double E(const StateMachineBase &s);

    virtual double E();

    void EFastUpdate(Part *p);

    double Check_dT(Part* p); // выдает dt энергии без переворота спина

    /**
    * рассчитывает внутреннюю энергию во всей системе частиц (метод подсчета через магнитные моменты)
	* Возвращает её и записывает в параметр E1 класса
    * H - вектор внешнего поля
    */
    double EComplete(Vect& H) const;
    double EComplete() const;
    /**
    * рассчитывает внутреннюю энергию для определенной частицы
    * @param elem ссылка на элемент, для которого считать энергию
    * @return энергия
    */
    double EComplete(Part *elem) const;
    double ECompleteOld(Part *elem) const;
    double ECompleteFast();

    inline double EMin() const {return this->eMin;}
    inline double EMax() const {return this->eMax;}
    inline void setEMin(const double val) {this->eMin = val;}
    inline void setEMax(const double val) {this->eMax = val;}

    virtual StateMachineFree maximalState(); //находит состояние максимума энергии системы
    virtual StateMachineFree groundState(); //находит состояние минимума энергии

    inline const StateMachineFree & Minstate() const { return minstate; }
    inline void setMinstate(const StateMachineBase &s) { minstate = s; setEMin(E(minstate)); }

    inline const StateMachineFree & Maxstate() const { return maxstate; }
    inline void setMaxstate(const StateMachineBase &s) { maxstate = s; setEMax(E(maxstate)); }

    inline const StateMachine & State() const { return state; }
    inline void setState(const StateMachineBase &s) { this->state = s; changeState(); }

    inline void changeState(){this->stateChanged=true;} //вызывается когда конфигурация системы изменилась (но не поменялась геометрия)
    void changeSystem(); //вызывается когда изменилась вся система целиком

    /**
    * выдает все парамеры частиц на экран
    */
    void cout();

    void save_v1(string file, bool showNotifications = false);
    void save_v2(std::string file);
    virtual void save(std::string file);

    /**
     * @brief savePVPython Сохраняет систему в скрипте ParaView
     * @param file Имя скрипта
     * @param thteta Количество полигонов по горизонтали
     * @param phi Количество полигонов по вертикали
     */
    void savePVPython(string file="data.py", int thteta=8, int phi=8);

    /**
     * @brief savePVPythonAnimation Сохраняет скрипт в виде анимации
     * @param secondSystem
     * @param file
     * @param thteta
     * @param phi
     * @param frames
     */
    void savePVPythonAnimation(PartArray *secondSystem, string file, int thteta=8, int phi=8, int frames=100);

    void load_v1(string file, bool showNotifications = false);
    void load_v2(string file);
    virtual void load(string file);

    /**
     * @brief csv Генерирует csv-файл энергий парных взаимодействий.
     * Если частицы не взаимодействуют - то соответствующее поле
     * csv-файла остается пустым
     * @param filename Имя создавамого файла
     * @param hamiltonian Ссылка на функцию - гамильтониан парного взаимодействия.
     * Если значение не задано, то используется гамильтониан, определенный системой.
     */
    void csv(string filename, double (*hamiltonian)(Part*,Part*) = NULL);

    //чистим массив частиц
    virtual void clear();
    //события для удобной кастомизации процесса очистки
    virtual void beforeClear();
    virtual void afterClear();

    //возвращает количество частиц на объекте
    unsigned count() const;
    inline unsigned size() const{return this->parts.size();}

    // возвращает вектор(массив) энергий каждой частицы
    std::vector<double> getEVector();

    // возвращает вектор(массив) полей взаимодействия каждой частицы
    std::vector<double> getHVector();

    // возвращает вектор(массив) проекций полей взаимодействия на ось z каждой частицы
    std::vector<double> getHZVector();

    //настройка начальных конфигураций
    //void setAntiferr(); //антиферромсагнетизм (шахматный порядок)
    //void setLines(); //одна линия вверх, одна вниз
    void setMAllUp(); //все вверх
    void setMBruteLines(double segmentSize);
    void setMRandom(); //случайно распределяет ось магнитного момента

    void rotateAllUp(); //поворачивает все векторы вверх
    void rotateAllLines(double segmentSize); //делит весь образец на сегменты и поворачивает один сегмент вверх - второй вниз

    void turnUp(); //повернуть M частиц так, чтобы их угол с осью X был меньше 0.
    void turnRight(); //повернуть M частиц так, чтобы их угол с осью Y был меньше 0.
    void turnToDirection(Vect*); //повернуть все частицы вдоль определенного вектора (сделать так чтобы угол был не более 90гр.)
    void movePosRandomly(double d); //Сдвинуть каждую частицу в случайном направлении на расстояние d;
    void moveMRandomly(double fi); //Сдвинуть вектор магнитного момента каждой частицы на угол fi в случайном направлении



    //просчет массива, возвращает набор энергий на каждом шаге
    //	std::vector<double> processStepXYZ(); //перебор в порядке объявления (сначала Z потом Y потом X)
    std::vector<double> processStep(); //перебор решетки и переворот всех по очереди
    std::vector<double> processMaxH(); //выбирается максимально неустойчивая частица и переворачивается
    std::vector<double> processGroupMaxH(); //выбирается группа максимально неустойчивых частиц и переворачиваются разом
    std::vector<double> processGroupStep(); //все неустойчивые частицы вращаются разом
    std::vector<double> processFromCenter(int x, int y, int z); //обработка из центра к краям, в параметрах координаты центральной частицы
    std::vector<double> processHEffective(); //переворот только тех частиц, для которых энергия выше среднего



    /**************************************************************
    ******временные функции, in process, deprecated and trash******
    **************************************************************/

	//находит состояние минимума энергии системы и переводит её в это состояние методом монте-карло
    //steps - количество попыток
    bool setToMonteCarloGroundState(const double t = 0, int steps=1000);

	//выполняет 1 шаг монте-карло при t температуре (относительная единица)
	//возвращает true если система изменилась и false если нет
	bool processMonteCarloStep(const double t=0);

	//находит минимум, но только переворачивает срау несколько частиц
	bool setToMonteCarloGroundState2();

	//ищем GS методом PT
    //tmin - минимальная температура системы
    //tmax - максимальная температура системы
	bool setToPTGroundState(int replicas, int totalSteps=1000, double tMin=0, double tMax=3);

    //проверяем сколько частиц стоят по полю, а сколько против поля
    void checkFM(string file, double c);

    //сохраняет намагниченность каждой частицы
    void saveEachMagnetization(string file);

	//рассчитываем обменный интеграл системы
    double calcJ();

	//сумма модулей обменных интегралов
	double calcJ2();

    //рассчитываем относительный обменный интеграл (отношение |J| к J)
	double calcJ12();

    /************************
     * Глобальные переменные *
     ***********************/

    //сам массив частиц, над которым проводятся исследования
    std::vector < Part* > parts;
    StateMachine state;

    virtual std::string type() const;
    void setType(std::string type);

    void setHamiltonian(double (*ham)(Part*,Part*)){this->_hamiltonian=ham; this->changeSystem();}

    inline unsigned neighbourSize(unsigned i){
        if (this->_interactionRange!=0.)
            return std::distance(neighbours[i].begin(), neighbours[i].end());
        else return this->size()-1;
    }

    double eAt(unsigned id1, unsigned id2) const { return this->eMatrix[id1][id2]; }
    vector< forward_list<Part*> > neighbours; //соседи, упорядоченные по id частицы

protected:
    double calcEnergy1FastIncremental(double initEnergy, const StateMachineBase &state); //state - новое состояние системы
    double calcEnergy1FastIncrementalFirst(); //считает начальную энергию системы и попутно записывает части энергий в параметры частиц

    double eMin,eMax,eInit;
    bool eInitCalculated;
    StateMachineFree minstate, maxstate, eInitState;
    bool _double_equals(double a, double b); //сравнение double
    virtual void subTetrahedron(Part * tmp, double x, double y, double z, double vect=1, double rot=0, double r=1);

    double _interactionRange; //длина взаимодействия
    bool _closeEdges; //замыкание краев

    std::string _type;

    /**
     * @brief preInsert Разметить нужное число ячеек в памяти для размещения частиц (чтобы лишний раз не переразмечать память)
     * @param count
     */
    void reserveParts(unsigned count);

    void EInit();
    double EUpdate(const StateMachineBase &s);
    void EUpdate();
private:
    std::map<std::string, std::string> _unusedFileContent; //при загрузке файла производного формата необходимо оставлять содержимое этого файла на случай дальнейшего сохранения
    double (*_hamiltonian)(Part*,Part*); //функция - гамильтониан системы

    vector< vector <double> > eMatrix;

    bool stateChanged;
};
#endif // PARTARRAY_H
