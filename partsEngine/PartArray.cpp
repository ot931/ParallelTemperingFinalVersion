/*
* File:   PartArray.cpp
* Author: uyras
*
* Created on 20 Ноябрь 2012 г., 17:22
*/

#include "PartArray.h"

PartArray::PartArray():
    state(this),
    minstate(),
    maxstate()
{
    this->changeSystem();
    this->_type="standart";
    _interactionRange = 0;
    setHamiltonian(hamiltonianDipolar);
}

PartArray::PartArray(const PartArray &sys):
    state(this),
    eInit(0.),
    eInitCalculated(false),
    stateChanged(true)
{
    this->_interactionRange = sys._interactionRange;
    this->_hamiltonian = sys._hamiltonian;

    //копируем частицы
    vector<Part*>::const_iterator iter = sys.parts.begin();
    while(iter!=sys.parts.end()){
        this->insert(new Part(*(*iter)));
        iter++;
    }

    this->eMin = sys.eMin;
    this->eMax = sys.eMax;
    this->_type = sys._type;

    minstate = sys.minstate;
    maxstate = sys.maxstate;
}

PartArray::~PartArray(){
    this->clear();
}

PartArray& PartArray::operator= (const PartArray& sys){
    if (this == &sys) return *this;

    this->clear();

    //копируем частицы
    vector<Part*>::const_iterator iter = sys.parts.begin();
    while(iter!=sys.parts.end()){
        this->insert(new Part(*(*iter)));
        iter++;
    }

    this->state = sys.state;
    this->eMin = sys.eMin;
    this->eMax = sys.eMax;
    this->eInit = 0;
    this->eInitCalculated = false;
    this->stateChanged =true;
    this->minstate = sys.minstate;
    this->maxstate = sys.maxstate;
    this->_interactionRange = sys._interactionRange;
    this->_type = sys._type;
    return *this;
}

Part *PartArray::operator[](const unsigned num)
{
    return parts[num];
}

bool PartArray::operator==(const PartArray &sys) const
{
    //сопоставляем число частиц
    if (this->count() != sys.count())
        return false;

    //сопоставление каждой частице
    vector<Part*>::const_iterator iter1, iter2;
    iter1= this->parts.begin();
    while (iter1!=this->parts.end()){
        bool foundFlag = false;
        iter2 = sys.parts.begin();
        while (iter2!=sys.parts.end()){
            if (*(*iter1)==*(*iter2)){
                foundFlag=true;
                break;
            }
            iter2++;
        }
        if (!foundFlag)
            return false;
        iter1++;
    }

    //сопоставляем тип
    if (this->type() != sys.type())
        return false;

    if (this->interactionRange()!=sys.interactionRange())
        return false;

    return true;
}

Part *PartArray::getById(unsigned id)
{
    if (id<this->size())
        return this->parts[id];
    else return 0;
}

Part *PartArray::findByPosition(const Vect &pos, double epsilon)
{
    vector<Part*>::reverse_iterator iter = this->parts.rbegin();
    Part* temp;
    while (iter!=this->parts.rend()){
        temp = *iter;
        if (fabs(temp->pos.x-pos.x)<epsilon && fabs(temp->pos.y-pos.y)<epsilon){
            return temp;
        }
        iter++;
    }
    return 0;
}


//Вспомогательная функция генерирующая спины
//Генерируется 4 спина (1 пирамида)
//Неуверен в правельности реализации функции и передачи самой системы
void PartArray::subTetrahedron(Part *tmp, double x, double y, double z, double vect, double rot, double r){

    double mLength=0;
    (void)mLength;//заглушка чтоб компиллятор не ругался
    if (tmp==0){
        tmp = new Part();
        mLength = config::Instance()->m;
    } else {
        mLength = tmp->m.length();
    }

    double x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3,cx,cy,cz;

    x0 = x + r * cos((rot + 0) * M_PI / 180);
    y0 = y + r * sin((rot + 0) * M_PI / 180);
    z0 = z;

    x1 = x + r * cos((rot + 120) * M_PI / 180);
    y1 = y + r * sin((rot + 120) * M_PI / 180);
    z1 = z;


    x2 = x + r * cos((rot + 240) * M_PI / 180);
    y2 = y + r * sin((rot + 240) * M_PI / 180);
    z2 = z;

    x3 = x;
    y3 = y;
    z3 = z + vect * r * sqrt(2);

    //центр пирамиды
    double l = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
    cx = x + 0;
    cy = y + 0;
    cz = z3 - vect * l / sqrt(3);
    double _o= sqrt((cx-x0)*(cx-x0)+(cy-y0)*(cy-y0)+(cz-z0)*(cz-z0));


    //посмотри вот этот ужас с temp1,2,3 и исправь
    //слаб в указателях и ссылках
    vector<Part*> hexPart;

    Part* temp1 = new Part(*tmp);
    temp1->pos.setXYZ(x0,y0,z0);
    temp1->m.setXYZ((cx-x0)/_o,(cy-y0)/_o,(cz-z0)/_o);
    hexPart.push_back(temp1);

    Part* temp2 = new Part(*tmp);
    temp2->pos.setXYZ(x1,y1,z1);
    temp2->m.setXYZ((cx-x1)/_o,(cy-y1)/_o,(cz-z1)/_o);
    hexPart.push_back(temp2);

    Part* temp3 = new Part(*tmp);
    temp3->pos.setXYZ(x2,y2,z2);
    temp3->m.setXYZ((cx-x2)/_o,(cy-y2)/_o,(cz-z2)/_o);
    hexPart.push_back(temp3);

    Part* temp4 = new Part(*tmp);
    temp4->pos.setXYZ(x3,y3,z3);
    temp4->m.setXYZ((cx-x3)/_o,(cy-y3)/_o,(cz-z3)/_o);
    hexPart.push_back(temp4);

    //цент пирамиды
    //Part* temp5 = tmp->copy();
    //temp5->pos.setXYZ(cx,cy,cz);
    //temp4->m.setXYZ(1,1,1);
    //hexPart.push_back(temp5);

    //Использовал вашу сортировку от повторений
    vector<Part*>::iterator iter = hexPart.begin();
    while (iter!=hexPart.end()){
        bool add=true;
        vector<Part*>::iterator iter2 = this->parts.begin();
        while(iter2!=this->parts.end()){
            if (
                    this->_double_equals((*iter2)->pos.x,(*iter)->pos.x) &&
                    this->_double_equals((*iter2)->pos.y,(*iter)->pos.y) &&
                    this->_double_equals((*iter2)->pos.z,(*iter)->pos.z)
                    )
                    add=false;
            iter2++;
        }
        if(add)
            this->insert(*iter);
        else
            delete (*iter); //удаляем из памяти
        iter++;
    }
}

void PartArray::reserveParts(unsigned count)
{
    parts.reserve(count);
    if (this->_interactionRange!=0.)
        neighbours.reserve(count);
}

void PartArray::changeSystem()
{
    this->eMin = this->eMax = this->eInit = 0;
    this->eInitCalculated = false;
    this->minstate.clear();this->maxstate.clear();
    this->stateChanged=true;
}

void PartArray::dropTetrahedron(int n, int m, int h, double R, Part *tmp)
{
    this->clear();
    double mx[6];
    double my[6];
    double r = 2*R; //2R
    double a = sqrt(2);

//инициализация начального шестиугольника
    for(int i = 0; i < 6; i++)
    {
        mx[i] = (r * cos((60 * i) * M_PI / 180));
        my[i] = (r * sin((60 * i) * M_PI / 180));
    }

// расчет растояния между шестиугольниками
    double deleyx = sqrt(12) * R * cos(90 * M_PI / 180);
    double deleyy = sqrt(12) * R * sin(90 * M_PI / 180);
    double deleyx2 = sqrt(12) * R * cos(M_PI / 6);
    double deleyy2 = sqrt(12) * R * sin(M_PI / 6);


    for (int z = 0; z < m; z++)
    {
        for (int k = 0; k < n; k++)
        {
            for (int j = 0; j < h; j += 2)
            {
                for (int i = 0; i < 6; i += 2)
                {   //12 пирамид (2 кольца)
                    this->subTetrahedron(tmp, mx[i] - r * j + deleyx * k + deleyx2 * z, my[i] + deleyy * k + deleyy2 * z, 2 * j * a);
                    this->subTetrahedron(tmp, mx[i + 1] - r * j + deleyx * k + deleyx2 * z, my[i + 1] + deleyy * k + deleyy2 * z, 2 * j * a, -1, 180);
                    this->subTetrahedron(tmp, mx[i] - r * (j + 1) + deleyx * k + deleyx2 * z, my[i] + deleyy * k + deleyy2 * z, 2 * (j + 1) * a, 1);
                    this->subTetrahedron(tmp, mx[i + 1] - r * (j + 1) + deleyx * k + deleyx2 * z, my[i + 1] + deleyy * k + deleyy2 * z, 2 * (j + 1) * a, -1, 180);
                }
            }
        }
    }
}

void PartArray::setInteractionRange(const double range)
{
    if (_interactionRange!=range){
        _interactionRange = range;
        neighbours.clear();

        //определяем соседей частицы
        if (this->_interactionRange!=0.){ //только если не все со всеми
            neighbours.resize(size());
            Part *part, *temp;
            for (unsigned i=0; i<size(); i++){
                neighbours[i].clear();
                part = (*this)[i];
                vector<Part*>::iterator iter = this->parts.begin();
                while(iter!=this->parts.end()){
                    temp = *iter;
                    if (isNeighbours(part,temp)){
                        neighbours[i].push_front(temp);
                    }
                    iter++;
                }
            }
        }

        changeSystem();
    }
}

bool PartArray::isNeighbours(const Part *_a, const Part *_b) const
{
    if (_a==_b) return false;
    if (_interactionRange==0) return true;
    else return _a->pos.space_2(_b->pos)<=(_interactionRange*_interactionRange);
}

void PartArray::setNeighbours(Part *_a, Part *_b)
{
    neighbours[_a->Id()].push_front(_b);
    neighbours[_b->Id()].push_front(_a);
}

//перемешать магнитные моменты частиц M
void PartArray::shuffleM(){
    bool rotate;
    for (unsigned i=0;i<this->count();i++){
        rotate = (double)config::Instance()->rand()/(double)config::Instance()->rand_max > 0.5;
        if (rotate)
            this->parts[i]->rotate();
    }
}

void PartArray::insert(Part * part){

    this->add(part);

    //определяем соседей частицы
    if (this->_interactionRange!=0.){ //только если не дальнодействие
        neighbours.push_back(forward_list<Part*>());//добавляем соседа, если его еще нет
        vector<Part*>::iterator iter = this->parts.begin();
        Part* temp;
        while(iter!=this->parts.end()){
            temp = *iter;
            if (isNeighbours(part,temp)){
                neighbours[temp->Id()].push_front(part);
                neighbours[part->Id()].push_front(temp);
            }
            iter++;
        }
    }
}

void PartArray::insert(const Part &part)
{
    insert(new Part(part));
}

void PartArray::add(Part *part)
{
    this->changeSystem();
    if (part->parent!=0){
        std::ostringstream s;
        s<<"part is already attached to system "<<part->parent<<". Declined to attach it to "<<this;
        throw s.str();
    }

    //прописываем родителя
    part->parent = this;

    if (part->id==-1) { //если ИД частицы не задан, назначаем новый ИД и вставляем в конец системы
        part->id = this->count();
        this->parts.push_back(part);
    } else {
        this->parts[part->id] = part;
    }
}



Vect PartArray::M(){
    return this->calcM1();
}

Vect PartArray::calcM1(){
    Vect temp;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp.x += (*iter)->m.x;
        temp.y += (*iter)->m.y;
        if (config::Instance()->dimensions()==3)
            temp.z += (*iter)->m.z;
        iter++;
    }
    return temp;
}

Vect PartArray::calcM2(){
    Vect temp;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp.x += fabs((*iter)->m.x);
        temp.y += fabs((*iter)->m.y);
        if (config::Instance()->dimensions()==3)
            temp.z += fabs((*iter)->m.z);
        iter++;
    }
    return temp;
}

Vect PartArray::calcM12(){
    Vect temp1, temp2;
    vector<Part*>::iterator iter = this->parts.begin();
    while (iter!=this->parts.end()){
        temp1.x += (*iter)->m.x;
        temp2.x += fabs((*iter)->m.x);

        temp1.y += (*iter)->m.y;
        temp2.y += fabs((*iter)->m.y);

        if (config::Instance()->dimensions()==3) {
            temp1.z += (*iter)->m.z;
            temp2.z += fabs((*iter)->m.z);
        }

        iter++;
    }
    temp1.x /= temp2.x;
    temp1.y /= temp2.y;
    if (config::Instance()->dimensions()==3)
        temp1.z /= temp2.z;

    return temp1;
}

void PartArray::calcH(Part* elem) {
    std::vector < Part* >::iterator iterator1;
    elem->h.x = 0;
    elem->h.y = 0;
    elem->h.z = 0;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (elem->pos.x != (*iterator1)->pos.x || elem->pos.y != (*iterator1)->pos.y || elem->pos.z != (*iterator1)->pos.z) { //не считать взаимодействие частицы на себя
            elem->h += (*iterator1)->interact(elem->pos);
        }
        ++iterator1;
    }
}

void PartArray::calcH() {
    std::vector < Part* >::iterator iterator2;
    iterator2 = this->parts.begin();
    while (iterator2 != this->parts.end()) {
        this->calcH(*iterator2);
        iterator2++;
    }
}

double PartArray::E(const StateMachineBase &s)
{
    if (size()<2) //если 1 либо 0 частиц, ничего не возвращаем
        return 0;

    if (!this->eInitCalculated){ //если начальная энергия не посчитана, считаем ее
        this->EInit();
    }

    return EUpdate(s);
}

double PartArray::E()
{
    if (size()<2) //если 1 либо 0 частиц, ничего не возвращаем
        return 0;

    if (!this->eInitCalculated){ //если начальная энергия не посчитана, считаем ее
        this->EInit();
    }

    if (this->stateChanged){
        this->EUpdate();
        this->stateChanged = false;
    }

    return this->eInit;
}

void PartArray::EInit(){
    this->eInitState = this->state;
    this->eInit = 0;

    double eTemp;

    this->eMatrix.clear();
    this->eMatrix.resize(this->size());

    for (Part* temp : this->parts) {
        if (this->_interactionRange!=0.){ //для близкодействия проходим по соседям
            for (Part* neigh : neighbours.at(temp->Id())){
                    eTemp=_hamiltonian(neigh, temp);
                    this->eInit += eTemp;
                    //В матрицу надо помещать энергии только в неперевернутых состояниях
                    if (neigh->state!=temp->state)
                        eTemp*=-1.;
                    this->eMatrix[temp->Id()].push_back(eTemp);
            }
        } else { //для дальнодействия проходим по всем
            for (Part* neigh : this->parts){
                if (temp!=neigh){
                    eTemp=_hamiltonian(neigh, temp);
                    this->eInit += eTemp;
                    //В матрицу надо помещать энергии только в неперевернутых состояниях
                    if (neigh->state!=temp->state)
                        eTemp*=-1.;
                    this->eMatrix[temp->Id()].push_back(eTemp);
                }
            }
        }
    }

    this->stateChanged=false;
    this->eInitCalculated=true;
    this->eInit *= 0.5;
}

void PartArray::EUpdate(){
    int j=0;

    unsigned ssize=this->size();
    vector<char> changedBits(ssize); //чар потому что бул очень долго работает в векторе
    bool tempState;
    for (unsigned i=0;i<ssize;i++){
        tempState=parts[i]->state;
        changedBits[i]=tempState ^ eInitState[i];
        eInitState.set(i,tempState);
    }


    //рассчитываем энергию
    //обходим все спины
    Part* temp;
    for (unsigned i=0; i<ssize; i++){
        //если состояние поменялось
        if ( changedBits[i] == (char)true){
            tempState=parts[i]->state;
            temp = (*this)[i];
            j=0;
            if (this->_interactionRange!=0.){
                for (Part* neigh: neighbours.at(i)){
                    unsigned nnum=neigh->Id();
                    if (!changedBits[nnum]){
                        if (tempState!=neigh->state)
                            eInit -=  2. * this->eMatrix[i][j];
                        else
                            eInit +=  2. * this->eMatrix[i][j];
                    }
                    j++;
                }
            } else {
                for (Part* neigh: this->parts){
                    if (temp != neigh){
                        unsigned nnum=neigh->Id();
                        if (!changedBits[nnum]){
                            if (tempState!=neigh->state)
                                eInit -=  2. * this->eMatrix[i][j];
                            else
                                eInit +=  2. * this->eMatrix[i][j];
                        }
                        j++;
                    }
                }
            }
        }
    }
}

double PartArray::EUpdate(const StateMachineBase &s){
    int j=0;

    unsigned ssize=s.size();
    vector<char> changedBits(ssize); //чар потому что бул очень долго работает в векторе
    for (unsigned i=0;i<ssize;i++)
        changedBits[i]=s[i]^eInitState[i];

    //рассчитываем энергию
    double E=this->eInit;
    Part* temp;
    bool thisState;

    //обходим все спины
    for (unsigned i=0; i<ssize; i++){
        //если состояние поменялось
        if ( changedBits[i] == (char)true){
            thisState=s[i];
            temp = (*this)[i];
            j=0;
            if (this->_interactionRange!=0.){
                for (Part* neigh: neighbours.at(i)){
                    unsigned nnum=neigh->Id();
                    if (!changedBits[nnum]){
                        if (thisState!=s[nnum])
                            E -=  2. * this->eMatrix[i][j];
                        else
                            E +=  2. * this->eMatrix[i][j];
                    }
                    j++;
                }
            } else {
                for (Part* neigh: this->parts){
                    if (temp != neigh){
                        unsigned nnum=neigh->Id();
                        if (!changedBits[nnum]){
                            if (thisState!=s[nnum])
                                E -=  2. * this->eMatrix[i][j];
                            else
                                E +=  2. * this->eMatrix[i][j];
                        }
                        j++;
                    }
                }
            }
        }
    }

    return E;
}

void PartArray::EFastUpdate(Part* p){
    if (!this->eInitCalculated || stateChanged){
        //если начальная энергия не посчитана,
        //либо текущее состояние не пересчитано,
        //пропускаем обновление энергии
        //this->EInit();
        return;
    } else {
        unsigned pid=p->Id();
        unsigned j=0;
        if (this->_interactionRange!=0.){
            for (Part* neigh : neighbours[pid]){
                if (neigh->state!=p->state)
                    this->eInit -=  2. * this->eMatrix[pid][j];
                else
                    this->eInit += 2. * this->eMatrix[pid][j];
                j++;
            }
        } else {
            for (Part* neigh : parts){
                if (p!=neigh){
                    if (neigh->state!=p->state)
                        this->eInit -=  2. * this->eMatrix[pid][j];
                    else
                        this->eInit += 2. * this->eMatrix[pid][j];
                    j++;
                }
            }
        }

        eInitState.set(pid, !(eInitState[pid])); //обновляем соответствующий спин состояния. Подразумевается, что в state изменился только этот спин.
    }
}

// вычисление разницы энергий от гипотетического переворота частицы p
double PartArray::Check_dT(Part* p){
    double dt1=0;
    unsigned pid=p->Id();
    unsigned j=0;

    if (this->_interactionRange!=0.){
        for (Part* neigh : neighbours[pid]){
            if (neigh->state!=p->state)
                dt1 -=  2. * this->eMatrix[pid][j];
            else
                dt1 += 2. * this->eMatrix[pid][j];
            j++;
        }
    }
    else{
        for (Part* neigh : parts){
            if (p!=neigh){
                if (neigh->state!=p->state)
                    dt1 -=  2. * this->eMatrix[pid][j];
                else
                    dt1 += 2. * this->eMatrix[pid][j];
                j++;
            }
        }

    }
        return -dt1;
}


double PartArray::EComplete(Vect& H) const {
    std::vector < Part* >::const_iterator iterator2;
    double E1=0;
    iterator2 = this->parts.begin();
    while (iterator2 != this->parts.end()) {
        E1 += this->EComplete(*iterator2) + H.scalar((*iterator2)->m);
        iterator2++;
    }
    E1 *= 0.5;
    return E1;
}

double PartArray::EComplete() const{
    Vect temp = Vect(0,0,0);
    return this->EComplete(temp);
}

double PartArray::EComplete(Part* elem) const {
    double r, r2, r5, E = 0;
    Vect rij;
    if (_interactionRange!=0.){
        for(Part* neigh: neighbours.at(elem->Id())){
            if (neigh != elem) { //не считать взаимодействие частицы на себя
                E += _hamiltonian(neigh, elem);
            }
        }
    } else {
        for(Part* neigh: this->parts){
            if (neigh != elem) { //не считать взаимодействие частицы на себя
                E += _hamiltonian(neigh, elem);
            }
        }
    }
    return E;
}

double PartArray::ECompleteOld(Part* elem) const {
    std::vector < Part* >::const_iterator iterator1;
    double r, r2, r5, E = 0;
    Vect rij;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (elem->pos.x != (*iterator1)->pos.x || elem->pos.y != (*iterator1)->pos.y || elem->pos.z != (*iterator1)->pos.z) { //не считать взаимодействие частицы на себя
            rij = (*iterator1)->pos.radius(elem->pos);
            r = rij.length();
            r2 = r * r; //радиус в кубе
            r5 = r2 * r * r * r; //радиус в пятой
            E += //энергии отличаются от формулы потому что дроби внесены под общий знаменатель
                    (((*iterator1)->m.scalar(elem->m) * r2)
                     -
                     (3 * elem->m.scalar(rij) * (*iterator1)->m.scalar(rij))) / r5; //энергия считается векторным методом, так как она не нужна для каждой оси
        }
        ++iterator1;
    }
    return E;
}

double PartArray::ECompleteFast(){
    std::vector < Part* >::iterator iterator1, iterator2, beginIterator, endIterator;
    double r, r2, r5, rijx, rijy;

    double E1 = 0;
    beginIterator = iterator2 = this->parts.begin();
    endIterator = this->parts.end();

    while (iterator2 != endIterator) {
        iterator1 = beginIterator;
        while (iterator1 != endIterator) {

            if (iterator2 != iterator1) { //не считать взаимодействие частицы на себя
                rijx = (*iterator2)->pos.x - (*iterator1)->pos.x;
                rijy = (*iterator2)->pos.y - (*iterator1)->pos.y;
                r2 = rijx*rijx+rijy*rijy;
                r = sqrt(r2); //трудное место, заменить бы
                r5 = r2 * r2 * r; //радиус в пятой
                E1 += //энергии отличаются от формулы потому что дроби внесены под общий знаменатель
                        (( ((*iterator1)->m.x*(*iterator2)->m.x+(*iterator1)->m.y*(*iterator2)->m.y) * r2)
                         -
                         (3 * ((*iterator2)->m.x * rijx + (*iterator2)->m.y * rijy) * ((*iterator1)->m.x * rijx + (*iterator1)->m.y * rijy)  )) / r5; //энергия считается векторным методом, так как она не нужна для каждой оси

            }

            ++iterator1;
        }
        iterator2++;
    }

    return E1 *= 0.5;
}

void PartArray::cout() {
    std::vector < Part* >::iterator iterator1;
    std::cout << "X\tY\tZ\tMx\tMy\tMz\t\tHx\tHy\tHz\t|H|" << std::endl;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        std::cout
                << (*iterator1)->pos.x << "\t"
                << (*iterator1)->pos.y << "\t"
                << (*iterator1)->pos.z << "\t"
                << (*iterator1)->m.x << "\t"
                << (*iterator1)->m.y << "\t"
                << (*iterator1)->m.z << "\t\t"/*
                           << (*iterator1)->h.x << "\t"
                           << (*iterator1)->h.y << "\t"
                           << (*iterator1)->h.z << "\t"*/
                << std::endl;
        ++iterator1;
    }
}

std::vector<double> PartArray::getEVector() {
    std::vector<double> e;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        e.push_back((*iterator1)->e);
        ++iterator1;
    }
    return e;
}

std::vector<double> PartArray::getHVector() {
    std::vector<double> h;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        h.push_back((*iterator1)->h.length());
        ++iterator1;
    }
    return h;
}

std::vector<double> PartArray::getHZVector() {
    std::vector<double> h;
    std::vector < Part* >::iterator iterator1 = this->parts.begin();

    while (iterator1 != this->parts.end()) {
        h.push_back((*iterator1)->h.z);
        ++iterator1;
    }
    return h;
}

//void PartArray::setAntiferr() {
//    std::vector < Part >::iterator iterator1;
//    iterator1 = this->parts.begin();
//    while (iterator1 != this->parts.end()) {
//        if ((int) ((*iterator1).pos.x + (*iterator1).pos.z) % 2 == 1) (*iterator1).axis.z = -1.;
//        else (*iterator1).axis.z = 1;
//        (*iterator1).axis.y = 0.;
//        (*iterator1).axis.x = 0.;
//
//        ++iterator1;
//    }
//    this->calcM();
//
//    this->calcInteraction();
//    this->calcEnergy1();
//    this->calcEnergy2();
//}

//void PartArray::setLines() {
//    std::vector < Part >::iterator iterator1;
//    iterator1 = this->parts.begin();
//    while (iterator1 != this->parts.end()) {
//        if ((int) ((*iterator1).pos.x) % 2 == 1) (*iterator1).axis.z = -1.;
//        else (*iterator1).axis.z = 1;
//        (*iterator1).axis.y = 0.;
//        (*iterator1).axis.x = 0.;
//
//        ++iterator1;
//    }
//    this->calcM();
//
//    this->calcInteraction();
//    this->calcEnergy1();
//    this->calcEnergy2();
//}

void PartArray::setMAllUp() {
    std::vector < Part* >::iterator iterator1;
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {

        (*iterator1)->m.x = 0.;
        (*iterator1)->m.y = 1.;
        (*iterator1)->m.z = 0.;
        ++iterator1;
    }
}

void PartArray::setMRandom() {
    std::vector < Part* >::iterator iterator1;
    srand((unsigned int)time(NULL));
    iterator1 = this->parts.begin();
    while (iterator1 != this->parts.end()) {
        if (config::Instance()->rand() % 2 == 1) (*iterator1)->m.z = 1.;
        else (*iterator1)->m.z = -1.;
        (*iterator1)->m.y = 0.;
        (*iterator1)->m.x = 0.;
        ++iterator1;
    }
}

std::vector<double> PartArray::processStep() {
    std::vector<Part*>::iterator iter;
    std::vector<double> history;
    iter = this->parts.begin();
    this->calcH();
    history.push_back(E());
    while (iter != this->parts.end()) {
        if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
            //std::cout << "rotate with x=" << (*iter).pos.x << "; y=" << (*iter).pos.y << std::endl;
            (*iter)->m.rotate();
            this->calcH();
            history.push_back(E());
            iter = this->parts.begin();
        } else {
            //std::cout << "normal with x=" << (*iter).pos.x << "; y=" << (*iter).pos.y << std::endl;
            iter++;
        }
        if (history.size()>10000)
            break;
    }
    return history;
}

std::vector<double> PartArray::processMaxH() {
    std::vector<double> history; //история переворота
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    Part* mElem = NULL; //тут будет храниться максимальная частица. по умолчанию максимальная - первая
    this->calcH();
    bool hasUnstable = true;
    while (hasUnstable) {
        hasUnstable = false;
        mElem = NULL; //чистим максимум
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                hasUnstable = true;
                //на первом шаге цикла нужно просто присвоить максимуму значение, а на остальных уже сравнивать
                if (mElem == NULL) {
                    mElem = *iter;
                } else {
                    if (mElem->h.length() < (*iter)->h.length())
                        mElem = *iter;
                }
            }
            iter++;
        }

        //если найдена нестабильная частица - вращаем её
        if (hasUnstable) {
            mElem->m.rotate();
            this->calcH();
            //сохраняем новую энергию системы в историю
            history.push_back(this->E());
            //std::cout << "rotate with x=" << mElem->pos.x << "; z=" << mElem->pos.z << std::endl;
        }
        if (history.size()>10000)
            break;
    }
    return history;
}

std::vector<double> PartArray::processGroupMaxH() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    int maxH = 0; //максимальный модуль поля взаимодействия
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {
        int rSize = 0; //счетчик, нужен для баловства
        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        maxH = 0;

        //step 1 - сначала находим максимальный магнитный момент системы
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                unstable.push_back(*iter);
                hasUnstable = true;
                if ((int) (*iter)->h.length() > maxH)
                    maxH = (int) (*iter)->h.length();
            }
            iter++;
        }

        //step 2 - переворачиваем ТОЛЬКО частицы с максимальной нестабильностью
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            if ((int) (*iter2)->h.length() == maxH) {
                (*iter2)->m.rotate();
                rSize++;
            }
            iter2++;
        }

        //step 3 - если чтото было перевернуто - обновляем поля взаимодействия
        if (hasUnstable) {
            //std::cout << "rotate group from " << rSize << " elements" << std::endl;
            this->calcH();
            //сохраняем новую энергию системы в историю
            history.push_back(this->E());
        }

        if (history.size()>10000)
            break;
    }

    return history;
}

std::vector<double> PartArray::processGroupStep() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {

        this->calcH();
        //сохраняем новую энергию системы в историю
        history.push_back(this->E());

        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        //step 1 - сначала находим все неустойчивые элементы
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                unstable.push_back(*iter);
                hasUnstable = true;
            }
            iter++;
        }

        //step 2 - переворачиваем все настабильные частицы
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            (*iter2)->m.rotate();
            iter2++;
        }

        if (history.size()>10000)
            break;
    }

    return history;
}


std::vector<double> PartArray::processHEffective() {
    this->calcH();
    std::vector<double> history; //история переворота
    std::vector<Part*> unstable; //частицы, подлежащие перевороту при текущей итерации
    std::vector<Part*>::iterator iter; //итератор для перебора массива частиц
    std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
    double averH = 0.; //максимальный модуль поля взаимодействия
    int averCount = 0; //количество значений, положеных в среднее
    bool hasUnstable = true; //есть нестабильные частицы или нет
    while (hasUnstable) {
        int rSize = 0; //счетчик, нужен для баловства, всерьёз не воспринимать
        hasUnstable = false;
        unstable.clear(); //чистим набор нестабильных частиц

        averH = 0.;
        averCount = 0;

        //step 1 - сначала находим средний магнитный момент всех частиц
        iter = this->parts.begin();
        while (iter != this->parts.end()) {
            if ((*iter)->h.length() > config::Instance()->hc && (*iter)->h.scalar((*iter)->m) < 0) {
                averH = (averH * averCount + (*iter)->h.length()) / (double) (averCount + 1);
                averCount++;
                unstable.push_back(*iter);
                hasUnstable = true;
            }
            iter++;
        }

        //step 2 - переворачиваем ТОЛЬКО частицы у которых энергия выше или равна среднему
        iter2 = unstable.begin();
        while (iter2 != unstable.end()) {
            if ((int) (*iter2)->h.length() >= (int) averH) {
                (*iter2)->m.rotate();
                rSize++;
            }
            iter2++;
        }

        //step 3 - если чтото было перевернуто - обновляем поля взаимодействия
        if (hasUnstable) {
            //std::cout << "rotate group from " << rSize << " elements" << std::endl;
            this->calcH();
            //сохраняем новую энергию системы в историю
            history.push_back(this->E());
        }

        if (history.size()>10000)
            break;
    }

    return history;
}

void PartArray::save_v1(string file, bool showNotifications) {

    if (showNotifications)
        std::cout<<"save "<<file<<" file start"<<endl;
    std::ofstream f(file.c_str());

    //сначала сохраняем xyz
    f << 0 << endl;
    f << 0 << endl;
    f << 0 << endl;

    //строка заголовков
    f << "x\ty\tz\tMx\tMy\tMz\tr" << endl;

    //затем все магнитные моменты системы и положения точек
    vector<Part*>::iterator iter = this->parts.begin();

    string shape;
    if (iter!=this->parts.end())
            shape="CIRCLE";

    while (iter != this->parts.end()) {
        f << (*iter)->pos.x << "\t";// << endl;
        f << (*iter)->pos.y << "\t";// << endl;
        f << (*iter)->pos.z << "\t";// << endl;
        f << (*iter)->m.x << "\t";// << endl;
        f << (*iter)->m.y << "\t";// << endl;
        f << (*iter)->m.z << "\t";// << endl;
        f << (*iter)->w1 << "\t";// << endl;
        f << (*iter)->h1 << "\t";// << endl;
        //f << (*iter)->sector << "\t";// << endl;
        f << (*iter)->r << "\t";
        f << shape << endl;
        iter++;
    }
    f.close();

    if (showNotifications)
        std::cout<<"save "<<file<<" file complete"<<endl;

}

void PartArray::save_v2(std::string file)
{
    SaveHelper helper(file,true);
    helper.writeDumped(this->_unusedFileContent);
    helper.writeHeader(this);
    helper.go("parts");

    vector<Part*>::iterator iter = this->parts.begin();
    while (iter != this->parts.end()) {
        helper << (*iter)->id;
        helper << (*iter)->pos.x;
        helper << (*iter)->pos.y;
        helper << (*iter)->pos.z;
        helper << (*iter)->m.x;
        helper << (*iter)->m.y;
        helper << (*iter)->m.z;
        helper << (*iter)->state;
        helper.line();
        iter++;
    }

    helper.close();
}

void PartArray::save(std::string file)
{
    this->save_v2(file);
}

void PartArray::savePVPython(string file, int thteta, int phi)
{
    //Vect delta(size.x/2.,size.y/2.,size.z/2.);
    Vect delta(0.,0.,0.);
    std::ofstream f(file.c_str());
    f<<"try: paraview.simple"<<endl<<
       "except: from paraview.simple import *"<<endl<<
       "paraview.simple._DisableFirstRenderCameraReset()"<<endl;
    f<<"spheres = ["<<endl;

    vector<Part*>::iterator iter = this->parts.begin();
    while (iter != this->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;
    f<<"i=0"<<endl;
    f<<"for [x,y,z,r,ax,ay,az] in spheres:"<<endl;
    f<<"\tprint(str(i)+\" of \"+str(len(spheres)))"<<endl;
    f<<"\tSphere( Radius=r/4, Center=[x, y, z], ThetaResolution="<<thteta<<", PhiResolution="<<phi<<" )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tLine( Point1=[x, y, z], Point2=[x+ax, y+ay, z+az], Resolution=1 )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\ti+=1"<<endl;
    f<<endl;
    f<<"DataRepresentation12 = Show()"<<endl;
    f<<"DataRepresentation12.EdgeColor = [0.0, 0.0, 0.5]"<<endl;
    f<<"DataRepresentation12.SelectionPointFieldDataArrayName = 'Normals'"<<endl;
    f<<"DataRepresentation12.Representation = 'Wireframe'"<<endl;
    f<<"DataRepresentation12.ScaleFactor = 0.1"<<endl;
    f<<"DataRepresentation12.CubeAxesVisibility = 1"<<endl;
    f<<"Render()"<<endl;

    f.close();
}

void PartArray::savePVPythonAnimation(PartArray* secondSystem, string file, int thteta, int phi, int frames)
{
    if (this->count()!=secondSystem->count())
        return;
    //Vect delta(size.x/2.,size.y/2.,size.z/2.);
    Vect delta(0.,0.,0.);
    std::ofstream f(file.c_str());
    f<<"try: paraview.simple"<<endl<<
       "except: from paraview.simple import *"<<endl<<
       "paraview.simple._DisableFirstRenderCameraReset()"<<endl<<endl;

    f<<"def getCue(el,param,num,oldVal,newVal):"<<endl<<
       "\tcue = GetAnimationTrack(el.GetProperty(param),num)"<<endl<<
       "\tkeyf0 = servermanager.animation.CompositeKeyFrame()"<<endl<<
       "\tkeyf0.KeyTime = 0"<<endl<<
       "\tkeyf0.KeyValues= [oldVal]"<<endl<<
       "\tkeyf1 = servermanager.animation.CompositeKeyFrame()"<<endl<<
       "\tkeyf1.KeyTime = 1.0"<<endl<<
       "\tkeyf1.KeyValues= [newVal]"<<endl<<
       "\tcue.KeyFrames = [keyf0, keyf1]"<<endl<<
       "\treturn cue"<<endl<<endl;

    vector<Part*>::iterator iter;
    f<<"scene = GetAnimationScene()"<<endl;
    f<<"scene.EndTime = 100.0"<<endl;
    f<<"scene.NumberOfFrames = "<<frames<<endl;

    f<<"spheres = ["<<endl;
    iter = this->parts.begin();
    while (iter != this->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;

    f<<"newSpheres = ["<<endl;
    iter = secondSystem->parts.begin();
    while (iter != secondSystem->parts.end()) {
        f << "\t";
        f<<"["<<(*iter)->pos.x-delta.x<<", "<<(*iter)->pos.y-delta.y<<", "<<(*iter)->pos.z-delta.z<<", "<<(*iter)->r<<", ";
        f<<(*iter)->m.x-delta.x<<", "<<(*iter)->m.y-delta.y<<", "<<(*iter)->m.z-delta.z<<"],"<<endl;
        iter++;
    }
    f<<"\t]"<<endl;

    f<<"i=0"<<endl;
    f<<"for [x,y,z,r,ax,ay,az] in spheres:"<<endl;
    f<<"\t[nx,ny,nz,nr,nax,nay,naz] = newSpheres[i]"<<endl;
    f<<"\tprint(str(i)+\" of \"+str(len(spheres)))"<<endl;
    f<<"\ts = Sphere( Radius=r, Center=[x, y, z], ThetaResolution="<<thteta<<", PhiResolution="<<phi<<" )"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tl = Line( Point1=[x, y, z], Point2=[x+ax, y+ay, z+az], Resolution=1 )"<<endl;



    f<<"\tp = GetDisplayProperties( l )"<<endl;
    f<<"\tp.DiffuseColor = [0.6, 0.0, 0.0]"<<endl;
    f<<"\tp.LineWidth = 10.0"<<endl;
    f<<"\tShow()"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",0,x,nx))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",1,y,ny))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point1\",2,z,nz))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",0,x+ax,nx+nax))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",1,y+ay,ny+nay))"<<endl;
    f<<"\tscene.Cues.append(getCue(l,\"Point2\",2,z+az,nz+naz))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",0,x,nx))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",1,y,ny))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Center\",2,z,nz))"<<endl;
    f<<"\tscene.Cues.append(getCue(s,\"Radius\",0,r,nr))"<<endl;
    f<<"\ti+=1"<<endl;
    f<<endl;

    f<<"DataRepresentation12 = Show()"<<endl;
    f<<"DataRepresentation12.EdgeColor = [0.0, 0.0, 0.5]"<<endl;
    f<<"DataRepresentation12.SelectionPointFieldDataArrayName = 'Normals'"<<endl;
    f<<"DataRepresentation12.Representation = 'Wireframe'"<<endl;
    f<<"DataRepresentation12.ScaleFactor = 0.1"<<endl;
    f<<"DataRepresentation12.CubeAxesVisibility = 1"<<endl;
    f<<"Render()"<<endl;

    f.close();
}

void PartArray::load_v1(string file,bool showNotifications) {
    if (showNotifications)
        std::cout<<"load "<<file<<" file start"<<endl;
    std::ifstream f(file.c_str());

    this->clear(); //удаляем все частицы

    //сначала сохраняем xyz
    double ttt;
    f >> ttt;
    f >> ttt;
    f >> ttt;

    int i=0;

    //пропускаем строку с заголовками
    char c[256];
    f.getline(c,256,'\n');
    f.getline(c,256,'\n');

    //затем читаем все магнитные моменты системы и положения точек
    double radius = 0;
    Part* temp;
    string shape;
    while (!f.eof()) {
        temp = new Part();
        if (!(f >> temp->pos.x).good()) break; //если не получилось считать - значит конец файла
        f >> temp->pos.y;
        f >> temp->pos.z;
        f >> temp->m.x;
        f >> temp->m.y;
        f >> temp->m.z;
        f >> temp->w1;
        f >> temp->h1;
        //f >> temp.sector; для MPI реализации, @todo потом перегрузить
        f >> temp->r;

        f >> shape;


        this->insert(temp);
        radius = temp->r;
        //if (i%1000==0) std::cout<<"load "<<i<<" particle"<<std::endl;
        i++;
    }

    f.close();
    if (showNotifications)
        std::cout<<"load "<<file<<" file complete"<<endl;

    config::Instance()->partR = radius;

    config::Instance()->set2D();

}

void PartArray::load_v2(string file)
{
    this->clear();
    Part *temp;
    LoadHelper helper(file);
    if (helper.validate()){
        helper.parseHeader();
        this->parts.resize(std::stoi(helper.params["size"]));
        helper.go("parts");
        while (!helper.end()){
            temp = new Part();
            helper>>temp->id;

            helper >> temp->pos.x;
            helper >> temp->pos.y;
            helper >> temp->pos.z;
            helper >> temp->m.x;
            helper >> temp->m.y;
            helper >> temp->m.z;
            helper >> temp->state;

            this->insert(temp);
            helper.line();
        }

        helper.applyHeader(this, true);

        _unusedFileContent = helper.dumpFileContent();
    }

    helper.close();
}

void PartArray::load(string file)
{
    switch(LoadHelper::version(file)){
        case 1: this->load_v1(file); break;
        case 2: this->load_v2(file); break;
    }
}

void PartArray::csv(string filename, double (*hamiltonian)(Part *, Part *))
{
    if (hamiltonian==NULL){
        hamiltonian= _hamiltonian;
    }
    ofstream f(filename.c_str());
    if (_interactionRange==0.){ //true - для неограниченного радиуса взаимодействия
        for (Part* p1 : this->parts){
            unsigned i=0;
            for (Part* p2: this->parts){
                if (p2->Id()!=p1->Id()){
                    f<<hamiltonian(p1,p2);
                }
                if (i != this->size()-1)
                    f<<";";
                ++i;
            }
            f<<endl;
        }
    } else {
        for (Part* p1 : this->parts){
            for (unsigned i=0; i< this->size(); ++i){
                for (Part* p2: this->neighbours[p1->Id()]){
                    if (p2->Id()==i){
                        f<<hamiltonian(p1,p2);
                        break;
                    }
                }
                if (i!=this->size()-1)
                    f<<";";
            }
            f<<endl;
        }
    }
    f.close();
}


void PartArray::clear(){
    this->beforeClear();
    vector<Part*>::iterator iter = this->parts.begin();
    Part *temp;
    while (iter!=this->parts.end()){
        temp = *iter;
        delete temp; //удаляем все что по есть по ссылкам на частицы
        iter++;
    }
    this->parts.clear();
    this->eMatrix.clear();
    this->changeSystem();
    this->afterClear();
}

void PartArray::beforeClear(){}

void PartArray::afterClear(){}

//временные функции, in process, deprecated and trash

//проверяем сколько частиц стоят по полю, а сколько против поля
void PartArray::checkFM(string file, double c){
    std::ofstream f(file.c_str(), ios_base::app);

    std::vector<Part*>::iterator iter1;
    const int total = this->parts.size();
    double scalar; //Number of H greather Zero; Number of H less Zero
    int i=0;
    iter1 = this->parts.begin();

    while (iter1!= this->parts.end()) {

        Vect h;
        int Nhgz = 0, Nhlz = 0, Nhez = 0;
        std::vector<Part*>::iterator iter2 = this->parts.begin();
        //считаем количество сонаправленных и обратнонаправленных частий

        while (iter2 != this->parts.end()){
            if (iter1!=iter2){
                h = (*iter2)->interact((*iter1)->pos); //считаем поле взаимодействия двух частиц
                scalar = h.scalar((*iter1)->m)/(h.length()*(*iter1)->m.length());

                if (scalar > 0.08) Nhgz++; else
                    if (scalar < -0.08) Nhlz++; else
                        Nhez++;

            }
            iter2++;
        }

        this->calcH(*iter1);

        f
                << (double)Nhgz/(double)total << "\t"
                << (double)Nhlz/(double)total << "\t"
                << (double)Nhez/(double)total << "\t"
                << (*iter1)->h.length() << "\t"
                << c <<endl;
        if (i%1000==0 || i<1000)
            std::cout<<i<<" particle"<<std::endl;
        i++;
        iter1++;
    }
    f.close();
}


void PartArray::saveEachMagnetization(string file) {
    std::ofstream f(file.c_str());

    vector<Part*>::iterator iter = this->parts.begin();
    this->calcH();
    while (iter != this->parts.end()) {
        f <<
             (*iter)->pos.x << "\t" <<
             (*iter)->pos.y << "\t" <<
             (*iter)->m.x << "\t" <<
             (*iter)->m.y << "\t" <<
             (*iter)->h.x << "\t" <<
             (*iter)->h.y << "\t" << endl;
        iter++;
    }

    f.close();
}

unsigned PartArray::count() const{
    return this->parts.size();
}

void PartArray::rotateAllUp(){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        if ((*iter)->m.y<0) (*iter)->m.y *= -1.;
        iter++;
    }
}

void PartArray::rotateAllLines(double segmentSize){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        if ((int)( (*iter)->pos.x / segmentSize) % 2 == 0) { //если в четном сегменте
            if ((*iter)->m.y < 0) (*iter)->m.y*= -1.; //поворачиваем вверх
        } else {
            if ((*iter)->m.y > 0) (*iter)->m.y*= -1.; //поворачиваем вниз
        }
        iter++;
    }
}

void PartArray::setMBruteLines(double segmentSize){
    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end())
    {
        (*iter)->m.x = (*iter)->m.z = 0;
        if ((int)( (*iter)->pos.x / segmentSize) % 2  == 0) { //если целая часть от деления делится на 2 без остатка, то в четном сегменте
            (*iter)->m.y = config::Instance()->m; //поворачиваем вверх
        } else {
            (*iter)->m.y = -config::Instance()->m; //поворачиваем вниз
        }
        iter++;
    }
}

std::string PartArray::type() const
{
    return this->_type;
}

void PartArray::setType(std::string type)
{
    this->_type = type;
}

bool PartArray::_double_equals(double a, double b)
{
    return fabs(a - b) < 1e-12;
}


double PartArray::calcJ(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) /  (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J+= ( Jxx + Jxy + Jyx + Jyy );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J+= ( Jxx + Jxy + Jxz + Jyx + Jyy + Jyz + Jzx + Jzy + Jzz);
                }

            }
            iter2++;
        }

        iter1++;
    }

    J /= this->count();
    return J;
}


double PartArray::calcJ2(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) /  (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J+= (
                                fabs(Jxx) + fabs(Jxy) +
                                fabs(Jyx) + fabs(Jyy)
                                );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J+= (
                                fabs(Jxx) + fabs(Jxy) + fabs(Jxz) +
                                fabs(Jyx) + fabs(Jyy) + fabs(Jyz) +
                                fabs(Jzx) + fabs(Jzy) + fabs(Jzz)
                                );
                }

            }
            iter2++;
        }

        iter1++;
    }

    J /= this->count();
    return J;
}

double PartArray::calcJ12(){
    vector<Part*>::iterator iter1,iter2;
    Vect r; //радиус-вектор
    double rL;
    double Jxx,Jxy,Jyx,Jyy,J1=0,J2=0;

    //для 3D случая
    double Jxz,Jyz,Jzx,Jzy,Jzz;

    iter1 = this->parts.begin();
    while (iter1!=this->parts.end()){
        iter2 = iter1;
        while (iter2!=this->parts.end()){
            if (iter2!=iter1){
                r = (*iter1)->pos.radius((*iter2)->pos);
                rL = r.length();


                if (config::Instance()->dimensions()==2){
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;
                    J1+= ( Jxx + Jxy + Jyx + Jyy );
                    J2+= (
                                fabs(Jxx) + fabs(Jxy) +
                                fabs(Jyx) + fabs(Jyy)
                                );
                } else {
                    Jxx = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.x*r.x) / (rL * rL) - 1.0 );
                    Jyy = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.y*r.y) / (rL * rL) - 1.0 );
                    Jzz = ( 1.0 / (rL*rL*rL) ) *  ( (3*r.z*r.z) / (rL * rL) - 1.0 );

                    Jxy = ( 3 * r.x * r.y) / ( rL * rL * rL * rL * rL );
                    Jyx = Jxy;

                    Jxz = ( 3 * r.x * r.z) / ( rL * rL * rL * rL * rL );
                    Jzx = Jxz;

                    Jzy = ( 3 * r.z * r.y) / ( rL * rL * rL * rL * rL );
                    Jyz = Jzy;

                    J1+= ( Jxx + Jxy + Jxz + Jyx + Jyy + Jyz + Jzx + Jzy + Jzz);
                    J2+= (
                                fabs(Jxx) + fabs(Jxy) + fabs(Jxz) +
                                fabs(Jyx) + fabs(Jyy) + fabs(Jyz) +
                                fabs(Jzx) + fabs(Jzy) + fabs(Jzz)
                                );
                }

            }
            iter2++;
        }

        iter1++;
    }

    //J1 *= (1./this->count());
    //J2 *= (1./this->count());
    return J1/J2;
}

StateMachineFree PartArray::maximalState()
{
    StateMachineFree oldState = state;
    this->state.reset();

    StateMachineFree mstate;
    double max;
    bool first = true;
    do {
        if (first){
            max = E();
            mstate = this->state;
            first = false;
        } else
            if (E() > max) {
                max = E();
                mstate = this->state;
            }
    } while (this->state.halfNext());

    state = oldState;

    return mstate;
}

StateMachineFree PartArray::groundState()
{
    StateMachineFree oldState = state;
    this->state.reset();

    StateMachineFree mstate;
    double min;
    bool first = true;
    do {
        if (first){
            min = E();
            mstate = this->state;
            first = false;
        } else
            if (E() < min) {
                min = E();
                mstate = this->state;
            }
    } while (this->state.halfNext());

    state = oldState;

    return mstate;
}


bool PartArray::setToMonteCarloGroundState(const double t, int steps){
    int tryingCount=0; //количество попыток переворота

    do {
        if (this->processMonteCarloStep(t))
            tryingCount = 0;
        else
            tryingCount++;
    } while (tryingCount<steps);
    return true;
}

bool PartArray::processMonteCarloStep(const double t){
    const double Eold = this->E();
    const int num = this->state.randomize();

    //если переворот привел к увеличению энергии, либо повлияла температура, возвращаем всё обратно
    const double
            de = Eold-this->E(),
            p = exp(de/t), //вероятность оставления системы
            randNum = (double)config::Instance()->rand()/(double)config::Instance()->rand_max; //случайное число от 0 до 1
    //если de>0 значит, оставляем переворот (новое E меньше старого), или
    //если выпавшее число меньше вероятности, тоже оставляем
    if (de>0 || (t>0 && randNum <= p)){
        return true;
    } else {
        this->parts[num]->rotate();
        return false;
    }
}

bool PartArray::setToMonteCarloGroundState2(){
    //if (this->count() > 32)
    //return false;


    int num,rotatedCount,tryingCount=0;
    const int count = this->count(), maxTryigCount = count*count*count;
    double Eold,Enew;
    vector<int> rot(count);
    do {
        Eold = this->E();
        rotatedCount = ( config::Instance()->rand()%(count-2) ) + 1; //решаем сколько частиц подворачивать
        for (int i=0;i<rotatedCount;i++){
            num = config::Instance()->rand()%count; //решаем какую частицу подворачивать
            this->parts.at(num)->rotate();
            rot[i] = num; //запоминаем подвернутую частицу
        }
        Enew = this->E();
        //если переворот привел к увеличению энергии, возвращаем всё обратно
        if (Eold<=Enew){
            for (int i=0;i<rotatedCount;i++){
                num = rot[i]; //решаем какую частицу подворачивать
                this->parts.at(num)->rotate();
            }
            tryingCount++;
        } else tryingCount=0;
    } while (tryingCount<maxTryigCount);


    return true;
}

bool PartArray::setToPTGroundState(int replicas, int totalSteps, double tMin, double tMax){
    //создали набор-шаблон частиц
    vector<PartArray*> systems;

    //создали набор температур
    vector <double> t;

    PartArray *temp;
    double tempInterval = 0;

    if (replicas>1)
        tempInterval = (tMax-tMin)/double(replicas-1);

    //плодим системы - копии оригинала и раздаём температуры
    for (int i=0;i<replicas;i++){
        systems.push_back(new PartArray(*this));

        //раздаём температуры
        t.push_back(tMin+tempInterval*i);
    }


    //magic
    bool mcResult, //результат работы монте-карло шага (применены ли изменения или нет)
            solved=false; //флаг, решено ли уравнение
    int i,
            eRepeatNum=0, //количество повторений энергии
            steps=0; //количество шагов

    //данные о минимуме
    StateMachineFree minState;
    double eMin=9999;


    vector<PartArray*>::iterator iter, iter2;

    do{
        //монтекарлим 1 раз все системы
        iter=systems.begin(); i=0;
        while (iter!=systems.end()){
            temp = *iter;
            mcResult = temp->processMonteCarloStep(t[i]);
            if (i==0){
                if (!mcResult)
                    eRepeatNum++;
                else
                    eRepeatNum = 0;
            }
            iter++; i++;
        }



        //производим обмен системы, если выпал случай
        iter=iter2=systems.begin(); i=0;
        iter2++;
        while (iter2!=systems.end()){
            double randNum = (double)config::Instance()->rand()/(double)config::Instance()->rand_max;
            double de = ((*iter)->E()) - ((*iter2)->E());
            double p = std::min(1.,exp(de/(0.75)));

            if (randNum<p){
                temp = (*iter);
                (*iter) = (*iter2);
                (*iter2) = temp;

                eRepeatNum = 0;
            }
            iter++; iter2++; i++;
        }

        //случай когда найден минимум
        if (systems[0]->E()<=eMin){
            minState = systems[0]->state;
            eMin = systems[0]->E();
            //std::cout << "E=" << minSystem->E1 <<endl;
        }

        steps++;
        if (steps>=totalSteps){
            //минимум достигнут
            solved = true;
        }

    } while(!solved);



    //под конец чистим память
    iter=systems.begin();
    while (iter!=systems.end()){
        temp = *iter;
        delete temp;
        iter++;
    }
    systems.clear();
    t.clear();

    this->state = minState;

    return true;
}

void PartArray::turnUp(){
    this->turnToDirection(new Vect(0,1,0));
}

void PartArray::turnRight(){
    this->turnToDirection(new Vect(1,0,0));
}

void PartArray::turnToDirection(Vect *v){
    vector<Part*>::iterator iter = this->parts.begin();
    iter = this->parts.begin();
    while(iter!=this->parts.end()){
        if ((*iter)->m.scalar(*v)<0){
            (*iter)->rotate();
        }
        iter++;
    }
}

void PartArray::movePosRandomly(double d){
    Vect dir; //направление, в которое двигать частицу.
    for(unsigned i=0;i<this->count();i++){
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude=0;
        switch (config::Instance()->dimensions()) {
        case 2:
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
            break;
        case 3:
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2.-1.;
            break;
        }
        dir.x = d * cos(longitude) * sqrt(1-lattitude*lattitude);
        dir.y = d * sin(longitude) * sqrt(1-lattitude*lattitude);
        dir.z = 0 * lattitude;
        this->parts[i]->pos += dir;
    }
    this->changeSystem();
}

void PartArray::moveMRandomly(double fi){
    if (config::Instance()->dimensions()==2){
        double side = 1.;
        double oldFi;

        for(unsigned i=0;i<this->count();i++){
            double oldLen = this->parts[i]->m.length();
            if ((double)config::Instance()->rand()/(double)config::Instance()->rand_max>0.5)
                side = -1.;
            else
                side = 1.;
            oldFi = this->parts[i]->m.angle();
            double longitude = oldFi+(fi*side);
            this->parts[i]->m.x = oldLen * cos(longitude);
            this->parts[i]->m.y = oldLen * sin(longitude);
            this->parts[i]->m.z = 0;
        }
    } else {
        for(unsigned i=0;i<this->count();i++){
            Part* temp = this->parts[i];
            Vect ox,oy,oz,newV;
            //1. нормализуем вектор частицы, считаем его длину
            oz = temp->m.normalize();

            //2. генерируем ортонормированный базис, где oz-магнитный момент частицы
            oy = Vect::crossProduct(oz,Vect(0.5,0,0));
            ox = Vect::normal(oy,oz);
            oy = Vect::normal(ox,oz);

            //3. генерируем направление сдвига вектора
            double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI; //[0;2pi]

            //4. получаем положение вдоль оси z
            //double lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*(1-cos(fi))+cos(fi); //[cos(fi);1]
            double lattitude=cos(fi); //задано параметром функции, cos(fi)

            //5. получаем сдвинутый вектор
            newV = oz*lattitude + (ox*cos(longitude) + oy*sin(longitude)) * sqrt(1-lattitude*lattitude);
            newV *= temp->m.length();
            temp->m = newV;
        }
    }
    this->changeSystem();
}

double hamiltonianDipolar(Part *a, Part *b)
{
    double rijx = b->pos.x - a->pos.x,
        rijy = b->pos.y - a->pos.y,
        rijz = b->pos.z - a->pos.z, r2, r, r5,E;
    if (config::Instance()->dimensions()==2)
        r2 = rijx*rijx+rijy*rijy;
    else
        r2 = rijx*rijx+rijy*rijy+rijz*rijz;
    r = sqrt(r2); //трудное место, заменить бы
    r5 = r2 * r2 * r; //радиус в пятой
    if (config::Instance()->dimensions()==2)
        E = //энергия считается векторным методом, так как она не нужна для каждой оси
                (( (a->m.x * b->m.x + a->m.y * b->m.y) * r2)
                 -
                 (3 * (b->m.x * rijx + b->m.y * rijy) * (a->m.x * rijx + a->m.y * rijy)  )) / r5;
    else
        E = //энергия считается векторным методом, так как она не нужна для каждой оси
                (( (a->m.x*b->m.x+a->m.y*b->m.y+a->m.z*b->m.z) * r2)
                 -
                 (3 * (b->m.x * rijx + b->m.y * rijy + b->m.z * rijz) * (a->m.x * rijx + a->m.y * rijy + a->m.z * rijz)  )) / r5;
    return E;
}

double hamiltonianIsing(Part *a, Part *b)
{
    return -1. * a->m.scalar(b->m);
}
