#include "cagomelattice.h"

Cagomelattice::Cagomelattice()
{

}

void Cagomelattice::dropSymmetricLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double lx=size.x;

    for(int vert=0;vert<lx+1;vert++){
           if(vert==0){
                for(int horiz=0;horiz<(lx);horiz++){
                    Part *temp = new Part();
                    temp->pos=Vect(l/2+(horiz*2*l)+0.75*l,-l*sqrt(3)/4,0);
                    temp->m=Vect(1/2., -sqrt(3)/2.,0);
                    this->add(temp);

                    Part *temp1 = new Part();
                    temp1->pos=Vect(l/2+(horiz*2*l)+1.25*l,-l*sqrt(3)/4,0);
                    temp1->m=Vect(1/2., sqrt(3)/2.,0);

                    this->add(temp1);

                }
            }
            else{

                for(int horiz=0;horiz<lx+vert;horiz++){
                    Part *temp = new Part();
                    temp->pos=Vect(l/2+(horiz*2*l)+0.75*l-vert*l,-l*sqrt(3)/4+vert*l*sqrt(3)-l*sqrt(3)/2,0);
                    temp->m=Vect(1/2., sqrt(3)/2.,0);
                    this->add(temp);

                    Part *temp1 = new Part();
                    temp1->pos=Vect(l/2+(horiz*2*l)+1.25*l-vert*l,-l*sqrt(3)/4+vert*l*sqrt(3)-l*sqrt(3)/2,0);
                    temp1->m=Vect(1/2., -sqrt(3)/2.,0);
                    this->add(temp1);

                    Part *temp2 = new Part();
                    temp2->pos=Vect(l/2+(horiz*2*l)+0.75*l-vert*l,-l*sqrt(3)/4+l*sqrt(3)/2+vert*l*sqrt(3)-l*sqrt(3)/2,0);
                    temp2->m=Vect(1/2., -sqrt(3)/2.,0);
                    this->add(temp2);

                    Part *temp3 = new Part();
                    temp3->pos=Vect(l/2+(horiz*2*l)+1.25*l-vert*l,-l*sqrt(3)/4+l*sqrt(3)/2+vert*l*sqrt(3)-l*sqrt(3)/2,0);
                    temp3->m=Vect(1/2., sqrt(3)/2.,0);

                    this->add(temp3);

                }
            }
       }

    for(int vert=0;vert<lx;vert++){
           if(vert==0){
               for(int horiz=0;horiz<(lx);horiz++){
                   Part *temp = new Part();
                   temp->pos=Vect(l/2+(horiz*2*l)+0.75*l,-l*sqrt(3)/4 + 2*(4*lx-1)*l*sqrt(3)/4.,0);
                   temp->m=Vect(1/2., sqrt(3)/2.,0);
                   this->add(temp);

                   Part *temp1 = new Part();
                   temp1->pos=Vect(l/2+(horiz*2*l)+1.25*l,-l*sqrt(3)/4 + 2*(4*lx-1)*l*sqrt(3)/4.,0);
                   temp1->m=Vect(1/2., -sqrt(3)/2.,0);

                   this->add(temp1);

               }
            }
            else{
                for(int horiz=0;horiz<lx+vert;horiz++){
                    Part *temp = new Part();
                    temp->pos=Vect(l/2+(horiz*2*l)+0.75*l-vert*l,-l*sqrt(3)/4 - vert*l*sqrt(3) - l*sqrt(3)/2 + 2*(4*lx-1)*l*sqrt(3)/4. + l*sqrt(3)/2. ,0);
                    temp->m=Vect(1/2., sqrt(3)/2.,0);
                    this->add(temp);

                    Part *temp1 = new Part();
                    temp1->pos=Vect(l/2+(horiz*2*l)+1.25*l-vert*l,-l*sqrt(3)/4 - vert*l*sqrt(3) - l*sqrt(3)/2 + 2*(4*lx-1)*l*sqrt(3)/4. + l*sqrt(3)/2.,0);
                    temp1->m=Vect(1/2., -sqrt(3)/2.,0);
                    this->add(temp1);

                    Part *temp2 = new Part();
                    temp2->pos=Vect(l/2+(horiz*2*l)+0.75*l-vert*l,-l*sqrt(3)/4 + l*sqrt(3)/2 - vert*l*sqrt(3) - l*sqrt(3)/2 + 2*(4*lx-1)*l*sqrt(3)/4. + l*sqrt(3)/2.,0);
                    temp2->m=Vect(1/2., -sqrt(3)/2.,0);
                    this->add(temp2);

                    Part *temp3 = new Part();
                    temp3->pos=Vect(l/2+(horiz*2*l)+1.25*l-vert*l,-l*sqrt(3)/4 + l*sqrt(3)/2 - vert*l*sqrt(3) - l*sqrt(3)/2 + 2*(4*lx-1)*l*sqrt(3)/4. + l*sqrt(3)/2.,0);
                    temp3->m=Vect(1/2., sqrt(3)/2.,0);

                    this->add(temp3);

                }
            }
       }



    for(int vert=0;vert<2*lx;vert++){
        if(vert<lx){
            for(int horiz=0;horiz<(2*lx+1)+vert*2;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(l/2+(horiz*l)-(l*vert),vert*l*sqrt(3),0);
                temp->m=Vect(1,0,0);
                this->add(temp);

            }
        }
        else{
            for(int horiz=0;horiz<(2*lx+1)+(2*lx-vert)*2-2;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(-l/2+(horiz*l)+(l*(-2*lx+vert+2)),vert*l*sqrt(3),0);
                temp->m=Vect(1,0,0);
                this->add(temp);
            }

        }
    }





    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}

void Cagomelattice::dropLattice2(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double p[][2]={
        {l/2., l},
        {l/4., l+l*sqrt(3)/4.},
        {l*3/4., l+l*sqrt(3)/4.},
        {l*3/2.,l},
        {l*5/4.,l-l*sqrt(3)/4.},
        {l*7/4.,l-l*sqrt(3)/4.}

    };
    double p2[][2]={
        {l/4., l+l*3*sqrt(3)/4.},
        {l*3/4., l+l*3*sqrt(3)/4.},
        {l/2., l+l*sqrt(3)},
        {l*3/2.,l+l*sqrt(3)},
        {l*5/4.,l+5*l*sqrt(3)/4.},
        {l*7/4.,l+5*l*sqrt(3)/4.}

    };
    double m[][2]={
        {1,0},
        {1/2., sqrt(3)/2.},
        {1/2., -sqrt(3)/2.},
        {1,0},
        {1/2., -sqrt(3)/2.},
        {-1/2., -sqrt(3)/2.}
    };
    double m2[][2]={
        {1/2., -sqrt(3)/2.},
        {-1/2., -sqrt(3)/2.},
        {1, 0},
        {1,0},
        {1/2., sqrt(3)/2.},
        {1/2., -sqrt(3)/2.}
    };

    //vector<Part*> top(size.x*4),bottom(size.x*4),left(size.y*4),right(size.y*4),front(size.z*4),back(size.z*4);
    double lx,ly,ly2;

    for (int i=0; i<size.x; i++){
        for (int j=0; j<size.y; j++){
            lx=l*(double)i;
            ly=l*(double)j;
            ly2=l*(double)(j-1);
            if(j%2!=0)
            {
                for (int c=0;c<6;c++){
                    Part *temp = new Part();
                    temp->pos=Vect(p[c][0]+2*lx,p[c][1]+(2*ly*sqrt(3)/2.),0);
                    temp->m=Vect(m[c][0],m[c][1],0);
                    this->add(temp);
                }
            }
            else {
                for (int c=0;c<6;c++){
                    Part *temp = new Part();
                    temp->pos=Vect(p2[c][0]+2*lx,p2[c][1]+(2*(ly2)*sqrt(3)/2.),0);
                    temp->m=Vect(m2[c][0],m2[c][1],0);
                    this->add(temp);
                }
            }



        }
    }

    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}
void Cagomelattice::dropLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double p[][2]={
        {l/2., l},
        {l/4., l+l*sqrt(3)/4.},
        {l*3/4., l+l*sqrt(3)/4.},
        {l*3/2.,l},
        {l*5/4.,l-l*sqrt(3)/4.},
        {l*7/4.,l-l*sqrt(3)/4.}

    };
    double m[][2]={
        {1,0},
        {1/2., sqrt(3)/2.},
        {1/2., -sqrt(3)/2.},
        {1,0},
        {1/2., -sqrt(3)/2.},
        {-1/2., -sqrt(3)/2.}
    };

    //vector<Part*> top(size.x*4),bottom(size.x*4),left(size.y*4),right(size.y*4),front(size.z*4),back(size.z*4);
    double lx,ly;

    for (int i=0; i<size.x; i++){
        for (int j=0; j<size.y; j++){
                lx=l*(double)i;
                ly=l*(double)j;
                for (int c=0;c<6;c++){
                    Part *temp = new Part();
                    temp->pos=Vect(p[c][0]+2*lx+ly,p[c][1]+(2*ly*sqrt(3)/2.),0);
                    temp->m=Vect(m[c][0],m[c][1],0);
                    this->add(temp);
                }



        }
    }

    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}
