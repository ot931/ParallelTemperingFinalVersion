#ifndef BRICKWORKA_H
#define BRICKWORKA_H

#include "PartArray.h"
#include "squarespinicecell.h"

using namespace std;

class BrickWorkA : public PartArray
{

public:
    BrickWorkA(){}
    virtual ~BrickWorkA(){}

    void dropBrickWorkA(int hCells, int vCells, double l=1){
        if (config::Instance()->dimensions()!=2)
            qFatal("Square spin ice is possible only in 2 dimensionals model");

        this->clear();
        this->cells.clear();
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
                SquareSpinIceCell cell = SquareSpinIceCell();
                cell.pos = center;

                //добавляем верхнюю
                partPos.x = center.x;
                partPos.y = center.y+l_2;
                temp = new Part();
                temp->pos = partPos;
                temp->m = Vect(config::Instance()->m,0,0);
                this->insert(temp);
                cell.top = temp;

                //добавляем нижнюю
                partPos.x = center.x;
                partPos.y = center.y-l_2;

                if ((temp = this->findByPosition(partPos)) == 0){
                    temp = new Part();
                    temp->pos = partPos;
                    temp->m = Vect(config::Instance()->m,0,0);
                    this->insert(temp);
                }
                cell.bottom = temp;

                //добавляем левую
                if (j == 0){
                    if ((i+j)%2!=0){
                        partPos.x = center.x-l_2;
                        partPos.y = center.y;
                        temp = new Part();
                        temp->pos = partPos;
                        temp->m = Vect(0,config::Instance()->m,0);
                        this->insert(temp);
                    } else {
                        temp=nullptr;
                    }
                } else temp=nextLeft;
                cell.left = temp;

                //добавляем правую
                if ((i+j)%2==0){
                    partPos.x = center.x+l_2;
                    partPos.y = center.y;
                    temp = new Part();
                    temp->pos = partPos;
                    temp->m = Vect(0,config::Instance()->m,0);
                    this->insert(temp);
                } else {
                    temp=nullptr;
                }
                nextLeft=temp;

                cell.right = temp;
                cell.row = i;
                cell.column = j;

                this->cells.push_back(cell);

                center.x+=l;
            }
            center.y+=l;
        }
    }

    StateMachineFree groundState(){
        vector<SquareSpinIceCell>::iterator iter = this->cells.begin();
        SquareSpinIceCell *temp;
        while (iter!=this->cells.end()){
            temp = &(*iter);
            int direction = (temp->row+temp->column)%2*2-1; //либо +1 либо -1, в шахматном порядке

            if (temp->top && temp->top->m.scalar(Vect(direction,0,0))<0)
                temp->top->rotate();
            if (temp->bottom && temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
                temp->bottom->rotate();
            if (temp->left && temp->left->m.scalar(Vect(0,direction,0))<0)
                temp->left->rotate();
            if (temp->right && temp->right->m.scalar(Vect(0,direction*-1.,0))<0)
                temp->right->rotate();

            iter++;
        }
        return state;
    }
    StateMachineFree maximalState(){
        vector<SquareSpinIceCell>::iterator iter = this->cells.begin();
        SquareSpinIceCell *temp;
        while (iter!=this->cells.end()){
            temp = &(*iter);
            int direction = (temp->row+temp->column)%2*2-1;

            if (temp->top && temp->top->m.scalar(Vect(direction,0,0))<0)
                temp->top->rotate();
            if (temp->bottom && temp->bottom->m.scalar(Vect(direction*-1.,0,0))<0)
                temp->bottom->rotate();
            if (temp->left && temp->left->m.scalar(Vect(0,direction*-1.,0))<0)
                temp->left->rotate();
            if (temp->right && temp->right->m.scalar(Vect(0,direction,0))<0)
                temp->right->rotate();

            iter++;
        }
        return state;
    }

    vector <SquareSpinIceCell> cells;

};

#endif // BRICKWORKA_H
