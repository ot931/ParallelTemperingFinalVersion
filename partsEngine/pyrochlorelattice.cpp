#include "pyrochlorelattice.h"

PyrochloreLattice::PyrochloreLattice()
{

}

void PyrochloreLattice::dropLattice(double l, Vect size)
{
    this->clear();
    totalSize=size*l;
    double p[][3]={
        {l/4., l/4., 0.},
        {l/2., l/2., 0.},
        {(3.*l)/4., (3.*l)/4., 0.},
        {l, l, 0.},
        //{0., 0., l},
        //{l/4., l/4., l},
        //{l/2., l/2., l},
        //{(3.*l)/4., (3.*l)/4., l},
        {0, (3.*l)/4., l/4.},
        {0, l/2., l/2.},
        {0, l/4., (3.*l)/4.},
        //{l, (3.*l)/4., l/4.},
        //{l, l/2., l/2.},
        //{l, l/4., (3.*l)/4.},
        //{l/4., l, (3.*l)/4.},
        //{l/2., l, l/2.},
        //{(3.*l)/4., l, l/4.},
        {l/4., 0, (3.*l)/4.},
        {l/2., 0, l/2.},
        {(3.*l)/4.,0, l/4.},
        {l/2., (3.*l)/4., (3.*l)/4.},
        {(3.*l)/4., l/2., (3.*l)/4.},
        {l/4., (3.*l)/4., l/2.},
        {l/4., l/2., l/4.},
        {(3.*l)/4., l/4., l/2.},
        {l/2., l/4., l/4.}
    };
    double m[][3]={
        {1/sqrt(3.), 1/sqrt(3.), 1/sqrt(3.)},
        {-(1/sqrt(3.)), -(1/sqrt(3.)), 1/sqrt(3.)},
        {1/sqrt(3.), 1/sqrt(3.), 1/sqrt(3.)},
        {-(1/sqrt(3.)), -(1/sqrt(3.)), 1/sqrt(3.)},
        //{1/sqrt(3.), 1/sqrt(3.), -(1/sqrt(3.))},
        //{-(1/sqrt(3.)), -(1/sqrt(3.)), -(1/sqrt(3.))},
        //{1/sqrt(3.), 1/sqrt(3.), -(1/sqrt(3.))},
        //{-(1/sqrt(3.)), -(1/sqrt(3.)), -(1/sqrt(3.))},
        {1/sqrt(3.), -(1/sqrt(3.)), 1/sqrt(3.)},
        {1/sqrt(3.), 1/sqrt(3.), -(1/sqrt(3.))},
        {1/sqrt(3.), -(1/sqrt(3.)), 1/sqrt(3.)},
        //{-(1/sqrt(3.)), 1/sqrt(3.), -(1/sqrt(3.))},
        //{-(1/sqrt(3.)), -(1/sqrt(3.)), 1/sqrt(3.)},
        //{-(1/sqrt(3.)), 1/sqrt(3.), -(1/sqrt(3.))},
        //{1/sqrt(3.), -(1/sqrt(3.)), -(1/sqrt(3.))},
        //{-(1/sqrt(3.)), -(1/sqrt(3.)), 1/sqrt(3.)},
        //{1/sqrt(3.), -(1/sqrt(3.)), -(1/sqrt(3.))},
        {-(1/sqrt(3.)), 1/sqrt(3.), 1/sqrt(3.)},
        {1/sqrt(3.), 1/sqrt(3.), -(1/sqrt(3.))},
        {-(1/sqrt(3.)), 1/sqrt(3.), 1/sqrt(3.)},
        {1/sqrt(3.), -(1/sqrt(3.)), 1/sqrt(3.)},
        {-(1/sqrt(3.)), 1/sqrt(3.), 1/sqrt(3.)},
        {1/sqrt(3.), 1/sqrt(3.), 1/sqrt(3.)},
        {1/sqrt(3.), -(1/sqrt(3.)), -(1/sqrt(3.))},
        {-(1/sqrt(3.)), -(1/sqrt(3.)), -(1/sqrt(3.))},
        {1/sqrt(3.), -(1/sqrt(3.)), 1/sqrt(3.)}
    };

    //vector<Part*> top(size.x*4),bottom(size.x*4),left(size.y*4),right(size.y*4),front(size.z*4),back(size.z*4);
    double lx,ly,lz;

    for (int i=0; i<size.x; i++){
        for (int j=0; j<size.y; j++){
            for (int k=0; k<size.z; k++){
                lx=l*(double)i;
                ly=l*(double)j;
                lz=l*(double)k;
                for (int c=0;c<16;c++){
                    Part *temp = new Part();
                    temp->pos=Vect(lx+p[c][0],ly+p[c][1],lz+p[c][2]);
                    temp->m=Vect(m[c][0],m[c][1],m[c][2]);
                    this->add(temp);
                }
                //верхняя грань
                if(k==size.z-1){

                    for (int c=0;c<3;c++){
                        Part *temp = new Part();
                        temp->pos=Vect(lx+p[c][0],ly+p[c][1],lz+p[c][2]+l);
                        temp->m=Vect(m[c][0],m[c][1],m[c][2]);
                        this->add(temp);
                    }

                }

                if(i==size.x-1){
                    for (int c=4;c<7;c++){
                        Part *temp = new Part();
                        temp->pos=Vect(lx+p[c][0]+l,ly+p[c][1],lz+p[c][2]);
                        temp->m=Vect(m[c][0],m[c][1],m[c][2]);
                        this->add(temp);
                    }

                }

                if(j==size.y-1){
                    for (int c=7;c<10;c++){
                        Part *temp = new Part();
                        temp->pos=Vect(lx+p[c][0],ly+p[c][1]+l,lz+p[c][2]);
                        temp->m=Vect(m[c][0],m[c][1],m[c][2]);
                        this->add(temp);
                    }

                }
                if(i==0||j==0||k==size.z-1){
                    Part *temp = new Part();
                    temp->pos=Vect(lx+0,ly+0,lz+l);
                    temp->m=Vect(1/sqrt(3.), 1/sqrt(3.), -(1/sqrt(3.)));
                    this->add(temp);
                }
            }
        }
    }
    // add border spins
//    for(int i=1;i<size.z+1;++i){
//        Part *temp = new Part();
//        temp->pos=Vect(0,0,l*(double)i);
//        temp->m=Vect(l/6, l/6, -(l/6));
//        this->add(temp);
//    }





    this->setInteractionRange(0); //this->setInteractionRange(l/sqrt(8)*1.05);
}

//bool PyrochloreLattice::isNeighbours(const Part *_a, const Part *_b) const
//{
//    if (_a==_b) return false;
//    if (_interactionRange==0) return true;
//    if (_a->pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//        return true;


//    Vect pos;
//    unsigned in=_a->Id()%16;

//    if (in==0 || in==4 || in==5 || in==6 || in==7){
//        pos = _a->pos;
//        pos.x+=totalSize.x;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==2 || in==3 || in==11 || in==14 || in==9){
//        pos = _a->pos;
//        pos.x-=totalSize.x;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==2 || in==3 || in==4 || in==10 || in==12){
//        pos = _a->pos;
//        pos.y-=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }


//    if (in==7 || in==8 || in==9 || in==0 || in==6){
//        pos = _a->pos;
//        pos.y+=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }


//    if (in==0 || in==1 || in==2 || in==3){
//        pos = _a->pos;
//        pos.z+=totalSize.z;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==6 || in==7 || in==10 || in==11){
//        pos = _a->pos;
//        pos.z-=totalSize.z;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==3){
//        pos = _a->pos;
//        //x,-y,+z
//        pos.y-=totalSize.y;
//        pos.z+=totalSize.z;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;

//        //-x,y,+z
//        pos.y+=totalSize.y;
//        pos.x-=totalSize.x;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;

//        //-x,-y,z
//        pos.z-=totalSize.z;
//        pos.y-=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;

//        //-x,-y,+z
//        pos.z+=totalSize.z;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }
//    if (in==6 || in==7){
//        //+x,y,-z
//        pos = _a->pos;
//        pos.x+=totalSize.x;
//        pos.z-=totalSize.z;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;

//        //+x,+y,-z
//        pos.y+=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;

//        //x,+y,-z
//        pos.x-=totalSize.x;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==0 || in==6 || in==7){
//        pos = _a->pos;
//        pos.x+=totalSize.x;
//        pos.y+=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    if (in==9){
//        pos = _a->pos;
//        pos.x-=totalSize.x;
//        pos.y+=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }
//    if (in==4){
//        pos = _a->pos;
//        pos.x+=totalSize.x;
//        pos.y-=totalSize.y;
//        if (pos.space_2(_b->pos)<=(_interactionRange*_interactionRange))
//            return true;
//    }

//    return false;
//}
