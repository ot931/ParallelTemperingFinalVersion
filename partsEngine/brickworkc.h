#ifndef BRICKWORKB_H
#define BRICKWORKB_H

#include "PartArray.h"
#include "squarespinicecell.h"

using namespace std;

class BrickWorkC : public PartArray
{

public:
    BrickWorkC(){}
    virtual ~BrickWorkC(){}

    void dropBrickWorkC(int hCells, int vCells, double l=1){
        if (config::Instance()->dimensions()!=2)
            qFatal("Square spin ice is possible only in 2 dimensionals model");

        this->clear();
        this->cells.clear();
        this->reserveParts(unsigned(2*(hCells*vCells)+hCells+vCells));

        double l_2 = l/2.; //полурешетка
        Vect center(0,0,0);

        Part *temp;
        Vect partPos(0,0,0);
        center.y = l_2;
        for (int i=0;i<vCells;i++){
            center.x = l_2;
            for (int j=0;j<hCells;j++){

                const int squareType = (int(i/2)%2+int(j/2))%2; //тип квадрата кирпичной кладки.
                                     //Если 0 - с горизонтальной перегородкой,
                                     //если 1 - с вертикальной

                const int cellPos = (i%2)*2 + j%2 + squareType*4; //0 - top left, 1 - top right, 2 - bottom left, 3 - bottom right


                //создаем ячейку
                SquareSpinIceCell cell = SquareSpinIceCell();
                cell.pos = center;

                //добавляем верхнюю
                if (cellPos!=4 && cellPos!=5){
                    partPos.x = center.x;
                    partPos.y = center.y+l_2;
                    if ((temp = this->findByPosition(partPos)) == 0){
                        temp = new Part();
                        temp->pos = partPos;
                        temp->m = Vect(config::Instance()->m,0,0);
                        this->insert(temp);
                    }
                    cell.top = temp;
                } else {
                    cell.top = nullptr;
                }

                //добавляем нижнюю
                if (cellPos!=6 && cellPos!=7){
                    partPos.x = center.x;
                    partPos.y = center.y-l_2;

                    if ((temp = this->findByPosition(partPos)) == 0){
                        temp = new Part();
                        temp->pos = partPos;
                        temp->m = Vect(config::Instance()->m,0,0);
                        this->insert(temp);
                    }
                    cell.bottom = temp;
                } else {
                    cell.bottom = nullptr;
                }

                //добавляем левую
                if (cellPos!=1 && cellPos!=3){
                    partPos.x = center.x-l_2;
                    partPos.y = center.y;
                    if ((temp = this->findByPosition(partPos)) == 0){
                        temp = new Part();
                        temp->pos = partPos;
                        temp->m = Vect(0,config::Instance()->m,0);
                        this->insert(temp);
                    }
                    cell.left = temp;
                } else {
                    cell.left = nullptr;
                }

                //добавляем правую
                if (cellPos!=0 && cellPos!=2){
                    partPos.x = center.x+l_2;
                    partPos.y = center.y;
                    if ((temp = this->findByPosition(partPos)) == 0){
                        temp = new Part();
                        temp->pos = partPos;
                        temp->m = Vect(0,config::Instance()->m,0);
                        this->insert(temp);
                    }
                    cell.right = temp;
                } else {
                    cell.right = nullptr;
                }

                cell.row = i;
                cell.column = j;
                this->cells.push_back(cell);

                center.x+=l;
            }
            center.y+=l;
        }
    }

    StateMachineFree maximalState(){
        StateMachineFree initState = state;
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
        StateMachineFree maxstate = this->state;
        this->state =initState;
        return maxstate;
    }

    vector <SquareSpinIceCell> cells;

};

#endif // BRICKWORKB_H
