#include "triangularlattice.h"

Triangularlattice::Triangularlattice()
{

}

void Triangularlattice::dropLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double p[][2]={
        {l/2., 0},
        {l/4., l*sqrt(3)/4.},
        {l*3/4., l*sqrt(3)/4.}
    };
    double m[][2]={
        {1,0},
        {1/2., sqrt(3)/2.},
        {1/2., -sqrt(3)/2.}
    };

    //vector<Part*> top(size.x*4),bottom(size.x*4),left(size.y*4),right(size.y*4),front(size.z*4),back(size.z*4);
    double lx,ly;

    for (int i=0; i<size.x; i++){
        for (int j=0; j<size.y; j++){
                lx=l*(double)i;
                ly=l*(double)j;
                for (int c=0;c<3;c++){
                    Part *temp = new Part();
                    temp->pos=Vect(p[c][0]+lx+(ly*1/2.),p[c][1]+(ly*sqrt(3)/2.),0);
                    temp->m=Vect(m[c][0],m[c][1],0);
                    this->add(temp);
                }
                //верхняя грань
                if(i==size.x-1){

                        Part *temp = new Part();
                        temp->pos=Vect(p[1][0]+lx+l+(ly*1/2.),p[1][1]+(ly*sqrt(3)/2.),0);
                        temp->m=Vect(m[1][0],m[1][1],0);
                        this->add(temp);


                }

                if(j==size.y-1){

                        Part *temp = new Part();
                        temp->pos=Vect(p[0][0]+lx+(ly*1/2.)+l*1/2.,p[0][1]+(ly*sqrt(3)/2.)+l*sqrt(3)/2.,0);
                        temp->m=Vect(m[0][0],m[0][1],0);
                        this->add(temp);


                }


        }
    }

    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}

void Triangularlattice::dropLattice2(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double p[][2]={
        {l/2., 0},
        {l/4., l*sqrt(3)/4.},
        {l*3/4., l*sqrt(3)/4.}
    };
    double m[][2]={
        {1,0},
        {1/2., sqrt(3)/2.},
        {1/2., -sqrt(3)/2.}
    };

    //vector<Part*> top(size.x*4),bottom(size.x*4),left(size.y*4),right(size.y*4),front(size.z*4),back(size.z*4);
    double lx,ly;

    for (int i=0; i<size.x; i++){
        for (int j=0; j<size.y; j++){
                lx=l*(double)i;
                ly=l*(double)j;
                if(j%2==0){
                    for (int c=0;c<3;c++){
                        Part *temp = new Part();
                        temp->pos=Vect(p[c][0]+lx+l*1/2.,p[c][1]+(ly*sqrt(3)/2.),0);
                        temp->m=Vect(m[c][0],m[c][1],0);
                        this->add(temp);
                    }
                }
                else{
                    for (int c=0;c<3;c++){
                        Part *temp = new Part();
                        temp->pos=Vect(p[c][0]+lx,p[c][1]+(ly*sqrt(3)/2.),0);
                        temp->m=Vect(m[c][0],m[c][1],0);
                        this->add(temp);
                    }
                }

                //верхняя грань
//                if(i==size.x-1){

//                        Part *temp = new Part();
//                        temp->pos=Vect(p[1][0]+lx+l+(ly*1/2.),p[1][1]+(ly*sqrt(3)/2.),0);
//                        temp->m=Vect(m[1][0],m[1][1],0);
//                        this->add(temp);


//                }

//                if(j==size.y-1){

//                        Part *temp = new Part();
//                        temp->pos=Vect(p[0][0]+lx+(ly*1/2.)+l*1/2.,p[0][1]+(ly*sqrt(3)/2.)+l*sqrt(3)/2.,0);
//                        temp->m=Vect(m[0][0],m[0][1],0);
//                        this->add(temp);


//                }


        }
    }

    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}

void Triangularlattice::dropSymmetricLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;

    double lx=size.x;

    for(int vert=0;vert<2*lx+1;vert++){
        if(vert<lx){
            for(int horiz=0;horiz<2*lx+2*vert+2;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(-l/4.+(horiz*0.5*l)-(0.5*l*vert),l*sqrt(3)/4. + vert*l*sqrt(3)/2.,0);
                if(horiz%2==0){
                    temp->m=Vect(1/2., -sqrt(3)/2.,0);
                }
                else{
                    temp->m=Vect(1/2., sqrt(3)/2.,0);
                }

                this->add(temp);
            }
            for(int horiz=0;horiz<vert+lx;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(l/2+(horiz*l)-(0.5*l*vert),vert*l*sqrt(3)/2.,0);
                temp->m=Vect(-1,0,0);
                this->add(temp);
            }


        }
        else{
            for(int horiz=0;horiz<(2*lx-vert)+lx;horiz++){
                Part *temp = new Part();
                temp->pos=Vect(l/2+(horiz*l)-(0.5*l*(2*lx-vert)),vert*l*sqrt(3)/2.,0);
                temp->m=Vect(-1,0,0);
                this->add(temp);
            }

            if(vert<2*lx){
                for(int horiz=0;horiz<4*lx+(2*lx-2*vert);horiz++){
                    Part *temp = new Part();
                    temp->pos=Vect(l/4.+(horiz*0.5*l)-((0.5*(2*lx-vert))*l),l*sqrt(3)/4. + vert*l*sqrt(3)/2.,0);
                    if(horiz%2==0){
                        temp->m=Vect(1/2., sqrt(3)/2.,0);
                    }
                    else{
                        temp->m=Vect(1/2., -sqrt(3)/2.,0);
                    }

                    this->add(temp);
                }
            }

        }
    }

    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}


