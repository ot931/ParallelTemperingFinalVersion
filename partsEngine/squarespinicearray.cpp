#include "squarespinicearray.h"

SquareSpinIceArray::SquareSpinIceArray()
{
    this->_type = "squarespinice";
}

SquareSpinIceArray::SquareSpinIceArray(const SquareSpinIceArray &sys)
    :PartArray(sys)
{
    //копируем содержание ячеек
    vector<SquareSpinIceCell*>::const_iterator iter = sys.cells.begin();
    SquareSpinIceCell *tempCell, *oldCell;
    while(iter!= sys.cells.end()){
        tempCell = new SquareSpinIceCell();
        oldCell = *iter;
        for (unsigned i=0;i<this->count();i++){
            if (*(oldCell->top)==*(this->parts[i]))
                tempCell->top=this->parts[i];

            if (*(oldCell->right)==*(this->parts[i]))
                tempCell->right=this->parts[i];

            if (*(oldCell->bottom)==*(this->parts[i]))
                tempCell->bottom=this->parts[i];

            if (*(oldCell->left)==*(this->parts[i]))
                tempCell->left=this->parts[i];
        }

        tempCell->column = oldCell->column;
        tempCell->row = oldCell->row;
        tempCell->pos = oldCell->pos;
        this->cells.push_back(tempCell);
        iter++;
    }
}

SquareSpinIceArray::~SquareSpinIceArray()
{
    this->clearCells();
}

SquareSpinIceArray &SquareSpinIceArray::operator =(const SquareSpinIceArray &sys)
{
    if (this == &sys) return *this;

    this->PartArray::operator =(sys);

    //копируем содержание ячеек
    vector<SquareSpinIceCell*>::const_iterator iter = sys.cells.begin();
    SquareSpinIceCell *tempCell, *oldCell;
    while(iter!= sys.cells.end()){
        tempCell = new SquareSpinIceCell();
        oldCell = *iter;
        for (unsigned i=0;i<this->count();i++){
            if (*(oldCell->top)==*(this->parts[i]))
                tempCell->top=this->parts[i];

            if (*(oldCell->right)==*(this->parts[i]))
                tempCell->right=this->parts[i];

            if (*(oldCell->bottom)==*(this->parts[i]))
                tempCell->bottom=this->parts[i];

            if (*(oldCell->left)==*(this->parts[i]))
                tempCell->left=this->parts[i];
        }

        tempCell->column = oldCell->column;
        tempCell->row = oldCell->row;
        tempCell->pos = oldCell->pos;
        this->cells.push_back(tempCell);
        iter++;
    }

    return *this;
}

void SquareSpinIceArray::dropSpinIce(int hCells, int vCells, double l)
{
    if (config::Instance()->dimensions()!=2)
        fprintf(stderr,"Square spin ice is possible only in 2 dimensionals model");

    this->clear();
    this->reserveParts(unsigned(2*(hCells*vCells)+hCells+vCells));

    double l_2 = l/2.; //полурешетка
    Vect center(0,0,0);

    Part *temp,
            *nextLeft; //следующая левая для новой ячейки будет предыдущей правой
    Vect partPos(0,0,0);
    center.y = l_2;
    for (int i=0;i<vCells;i++){
        center.x = l_2;
        for (int j=0;j<hCells;j++){
            //создаем ячейку
            SquareSpinIceCell *cell = new SquareSpinIceCell();
            cell->pos = center;

            //добавляем верхнюю
            partPos.x = center.x;
            partPos.y = center.y+l_2;
            temp = new Part();
            temp->pos = partPos;
            temp->m = Vect(config::Instance()->m,0,0);
            this->insert(temp);
            cell->top = temp;

            //добавляем нижнюю
            partPos.x = center.x;
            partPos.y = center.y-l_2;

            if ((temp = this->findByPosition(partPos)) == 0){
                temp = new Part();
                temp->pos = partPos;
                temp->m = Vect(config::Instance()->m,0,0);
                this->insert(temp);
            }
            cell->bottom = temp;

            //добавляем левую
            partPos.x = center.x-l_2;
            partPos.y = center.y;

            if (j == 0){
                temp = new Part();
                temp->pos = partPos;
                temp->m = Vect(0,config::Instance()->m,0);
                this->insert(temp);
            } else temp=nextLeft;
            cell->left = temp;

            //добавляем правую
            partPos.x = center.x+l_2;
            partPos.y = center.y;

            temp = new Part();
            temp->pos = partPos;
            temp->m = Vect(0,config::Instance()->m,0);
            this->insert(temp);
            nextLeft=temp;

            cell->right = temp;
            cell->row = i;
            cell->column = j;

            this->cells.push_back(cell);

            center.x+=l;
        }
        center.y+=l;
    }
}

StateMachineFree SquareSpinIceArray::groundState()
{
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    SquareSpinIceCell *temp;
    while (iter!=this->cells.end()){
        temp = *iter;
        int direction = (temp->row+temp->column)%2*2-1; //либо +1 либо -1, в шахматном порядке

        if (temp->top->m.scalar(Vect(direction,0,0))<0)
            temp->top->rotate();
        if (temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
            temp->bottom->rotate();
        if (temp->left->m.scalar(Vect(0,direction,0))<0)
            temp->left->rotate();
        if (temp->right->m.scalar(Vect(0,direction*-1.,0))<0)
            temp->right->rotate();

        iter++;
    }
    return state;
}

StateMachineFree SquareSpinIceArray::maximalState()
{
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    SquareSpinIceCell *temp;
    while (iter!=this->cells.end()){
        temp = *iter;
        int direction = (temp->row+temp->column)%2*2-1;

        if (temp->top->m.scalar(Vect(direction,0,0))<0)
            temp->top->rotate();
        if (temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
            temp->bottom->rotate();
        if (temp->left->m.scalar(Vect(0,direction*-1.,0))<0)
            temp->left->rotate();
        if (temp->right->m.scalar(Vect(0,direction,0))<0)
            temp->right->rotate();

        iter++;
    }
    return state;
}

void SquareSpinIceArray::clear()
{
    this->clearCells();

    PartArray::clear();
}

void SquareSpinIceArray::load(std::string file)
{
    this->clear();
    //load base part of file
    PartArray::load(file);
    LoadHelper helper(file);

    //skip to cells section
    helper.go("cells");

    //read cells data
    SquareSpinIceCell *tempCell;
    int id;
    while(!helper.end()){
        tempCell = new SquareSpinIceCell();
        helper>>tempCell->pos.x;
        helper>>tempCell->pos.y;
        tempCell->pos.z = 1;
        helper>>tempCell->row;
        helper>>tempCell->column;

        helper>>id;
        tempCell->top = this->getById(id);

        helper>>id;
        tempCell->bottom = this->getById(id);

        helper>>id;
        tempCell->left = this->getById(id);

        helper>>id;
        tempCell->right = this->getById(id);


        this->cells.push_back(tempCell);
        helper.line();
    }

    helper.close();
}

void SquareSpinIceArray::save(std::string file)
{
    //save base part of file
    PartArray::save(file);
    SaveHelper helper(file);

    //write header
    helper.go("cells");

    //write particles
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    while (iter != this->cells.end()) {
        helper<<(*iter)->pos.x;
        helper<<(*iter)->pos.y;
        helper<<(*iter)->row;
        helper<<(*iter)->column;
        helper<<(*iter)->top->Id();
        helper<<(*iter)->bottom->Id();
        helper<<(*iter)->left->Id();
        helper<<(*iter)->right->Id();

        helper.line();
        iter++;
    }
    helper.close();
}

void SquareSpinIceArray::clearCells()
{
    vector<SquareSpinIceCell*>::iterator iter = this->cells.begin();
    while (iter!=this->cells.end()){
        delete (*iter); //удаляем из памяти все ячейки
        iter++;
    }
    this->cells.clear();
}

