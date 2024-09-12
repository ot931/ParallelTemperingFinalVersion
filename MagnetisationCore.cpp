#include "MagnetisationCore.h"

MagnetisationCore::MagnetisationCore(const std::string & parameterId, 
    PartArray * prototype,
    const Vect & vector, 
    const std::vector<uint64_t> & spins):
CalculationParameter(parameterId,prototype),
vector(vector),
spins(spins),
mv(0,1024*8),
mv2(0,2048*8),
mOld(0),
_sumModule(false)
{
    if (this->spins.size() == 0) {
        this->spins.resize(this->prototype->size(),0);
        for (uint64_t i=0; i<this->prototype->size(); ++i) this->spins[i]=i;
    }
    this->vector.setUnitary();

    this->prototypeInit(prototype);
}

bool MagnetisationCore::check(unsigned N) const
{
    return true;
}

void MagnetisationCore::printHeader(unsigned num) const
{
    // get saturation magnetisation
    double saturation=0;
    for (auto s : this->spins){
        saturation += fabs(this->prototype->parts[s]->m.scalar(this->vector));
    }
    saturation /= this->spins.size();


    printf("##### calculation param #%d #####\n",num);

    if (this->_sumModule)
        printf("# type: magnetisation module\n");
    else
        printf("# type: magnetisation\n");

    printf("# id: %s\n",this->parameterId().c_str());

    printf("# magnetisation vector: (%g|%g|%g); initial value %g\n",
        this->vector.x,
        this->vector.y,
        this->vector.z,
        this->getFullTotal(this->prototype)/this->spins.size());
    printf("# saturation magnetisation / N: %g\n", saturation);
    printf("# spins: ");
    if (this->spins.size()==this->prototype->size()){
        printf("All\n");
    } else {
        printf("%d",this->spins[0]);
        for (int ss=1; ss<this->spins.size(); ++ss){
            printf(",%d",this->spins[ss]);
        }
        printf("\n");
    }

    printf("#\n");
    return;
}

bool MagnetisationCore::init(PartArray * sys)
{

    //clear
    magnetisationValues.clear();
    
    this->sys = sys;
    magnetisationValues.resize(sys->size(),0);

    for (auto spinId: spins){
        Part* part = sys->parts[spinId];
        magnetisationValues[spinId] = part->m.scalar(vector);
        if (part->state)
            magnetisationValues[spinId] *= -1;
        
    }

    this->mOld = getFullTotal(this->sys);

    return true;
}

void MagnetisationCore::iterate(unsigned id){
    this->mOld += 2*this->method(id,this->sys);
    if (_debug){
        double res = this->getFullTotal(this->sys);
        if (fabs(res-this->mOld)>0.01) 
            cerr<<"# (dbg MagnetisationCore#"<<this->parameterId()<<") total value is different: iterative="<<this->mOld<<", full="<<res<<endl;
    }
}

void MagnetisationCore::incrementTotal(){
    double addVal = double(this->mOld) / this->spins.size();
    if (this->_sumModule)
        this->mv += fabs(addVal);
    else
        this->mv += addVal;
    this->mv2 += addVal*addVal;
}

double MagnetisationCore::getFullTotal(const PartArray * _sys) const
{
    double res = 0;
    for (auto spinId: spins){
        res += this->method(spinId, _sys);
    }
    return res;
}

double MagnetisationCore::method(unsigned spinId, const PartArray * _sys) const
{
        return magnetisationValues[spinId]*((_sys->parts[spinId]->state)?-1:+1);
}

void MagnetisationCore::setModule(bool module)
{
    this->_sumModule = module;
}