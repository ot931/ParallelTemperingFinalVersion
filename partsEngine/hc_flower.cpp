#include "hc_flower.h"

HC_Flower::HC_Flower()
{

}


void HC_Flower::dropSymmetricLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
//    double p[][2]={
//        {l/2., 0},
//        {l/4., l*sqrt(3)/4.},
//        {l*3/4., l*sqrt(3)/4.}
//    };
//    double m[][2]={
//        {1,0},
//        {1/2., sqrt(3)/2.},
//        {1/2., -sqrt(3)/2.}
//    };

    double lx=size.x;
    double q3=sqrt(3.);
    double mx05=0.5;
    double mx075=sqrt(0.75);

    for(int vert=0;vert<2*size.x;vert++){
        if(vert<size.x){
            for(int horiz=0;horiz<(2*size.x+2*vert);horiz++){
                Part *temp = new Part();
                temp->pos=Vect(0.25*sqrt(3.)*l+l*q3/4+l*q3/2.*horiz-l*vert*q3*0.5,-0.75*l+vert*l*1.5,0);
                if(horiz%2==0)
                    temp->m=Vect(-mx075,mx05,0);
                else
                    temp->m=Vect(mx075,mx05,0);
                this->add(temp);

            }
        }
        else{
            for(int horiz=0;horiz<(2*size.x-vert+(size.x-2))*2+2;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(0.25*sqrt(3.)*l+l*q3/4-(size.x-1)*q3/2.*l+l*(vert-(size.x-1))*0.5*q3+l*horiz*q3/2-l*q3*0.5,-0.75*l+vert*l*1.5,0);
                if(horiz%2==0)
                    temp->m=Vect(mx075,mx05,0);
                else
                    temp->m=Vect(-mx075,mx05,0);
                this->add(temp);
                //center_of_circle.push_back(0-(size.x-1)*sqrt(3)/2.+(vert-(size.x-1))*0.5*sqrt(3)+horiz*q3);
                //center_of_circle.push_back(1.5*vert);
            }
        }

    }

    for(int vert=0;vert<2*size.x-1;vert++){
        if(vert<size.x){
            for(int horiz=0;horiz<(size.x+vert+1);horiz++){
                Part *temp = new Part();
                temp->pos=Vect(0.25*sqrt(3.)*l+l*q3*horiz-l*vert*q3*0.5,vert*l*1.5,0);
                temp->m=Vect(0,1,0);
                this->add(temp);

            }
        }
        else{
            for(int horiz=0;horiz<2*size.x-vert+(size.x-2)+1;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(0.25*sqrt(3.)*l-(size.x-1)*q3/2.*l+l*(vert-(size.x-1))*0.5*q3+l*horiz*q3,vert*l*1.5,0);
                temp->m=Vect(0,1,0);
                this->add(temp);

            }
        }

    }






    this->setInteractionRange(0);//this->setInteractionRange(1.8); //this->setInteractionRange(l/sqrt(8)*1.05);
}


