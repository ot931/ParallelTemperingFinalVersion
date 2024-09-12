#include "honeycombspinicecell.h"

HoneycombSpinIceCell::HoneycombSpinIceCell()
{

}

HoneycombSpinIceCell::~HoneycombSpinIceCell()
{

}

void HoneycombSpinIceCell::rotateClockWise()
{
    Part *part1 = this->parts[5], *part2 = this->parts[0];
    int i=0;
    do {
        if (part2->m.scalar((part1->pos) - (part2->pos))<0){
            part2->rotate();
        }

        i++;
        part1 = this->parts[i-1];
        part2 = this->parts[i];
    } while (i<6);
}

void HoneycombSpinIceCell::rotateAntiClockWise()
{
    Part *part1 = this->parts[5], *part2 = this->parts[0];
    int i=0;
    do {
        if (part2->m.scalar(part1->pos - part2->pos)>0){
            part2->rotate();
        }

        i++;
        part1 = this->parts[i-1];
        part2 = this->parts[i];
    } while (i<6);
}

void HoneycombSpinIceCell::rotateChaotic()
{
    Part *part1 = this->parts[5], *part2 = this->parts[0];
    int i=0;
    do {
        if (i%2==0){
            if (part2->m.scalar(part1->pos - part2->pos)>0){
                part2->rotate();
            }
        } else {
            if (part2->m.scalar(part1->pos - part2->pos)<0){
                part2->rotate();
            }
        }

        i++;
        part1 = this->parts[i-1];
        part2 = this->parts[i];
    } while (i<6);
}

