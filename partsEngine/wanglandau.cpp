#include "wanglandau.h"

WangLandau::WangLandau(PartArray *sys, unsigned intervals, double accuracy, double fmin):
    showMessages(false),
    saveEach(0),
    sys(sys),
    intervals(intervals),
    accuracy(accuracy),
    fMin(fmin),
    f(0)
{

    //считаем минимум и максимум системы
    if (sys->EMin()==0 || sys->EMax()==0)
        fprintf(stderr,"Min or max state is unknown. DOS calculation is impossible.");

    //инициируем DOS
    h.resize(sys->EMin(), sys->EMax(), intervals);
    g.resize(sys->EMin(), sys->EMax(), intervals);
}

WangLandau::~WangLandau()
{

}

void WangLandau::run(unsigned steps)
{
    const StateMachineFree initState = sys->state;
    this->f = exp(1);
    long unsigned accepted=0, rejected=0, totalSteps=0;
    this->resetH();
    updateGH(sys->E());

    fprintf(stderr,"steps=%u",steps);

    double eOld = sys->E(),eNew=sys->E();

    while (f>fMin){
        //повторяем алгоритм сколько-то шагов
        for (unsigned i=0;i<steps;i++){
            int partNum = sys->state.randomize();

            eNew = sys->E();
            if (rnd::nextDouble() <= exp(g[eOld]-g[eNew])) {
                eOld = eNew;
                ++accepted;
            } else {
                sys->parts[partNum]->rotate(true); //откатываем состояние
                ++rejected;
            }

            updateGH(eOld);
            ++totalSteps;

            if (saveEach && totalSteps%saveEach==0){
                ostringstream fn;
                fn<<"g_"<<totalSteps<<".dat";
                saveG(fn.str());

                fn.str("");
                fn.clear();
                fn<<"h_"<<totalSteps<<".dat";
                saveH(fn.str());
            }
        }

        //проверяем ровность диаграммы
        if (this->isFlat()){
            f=sqrt(f);
            this->resetH();
            msg("accepted ",(int)accepted);
            msg("rejected ",(int)rejected);
            msg("h is flat, new f is ",f);
            accepted=0; rejected=0;

        }
    }


    sys->state = initState;
    return;
}

void WangLandau::saveH(const string filename) const
{
    ofstream f(filename);
    for (unsigned i=0;i<h.Intervals();i++){
        if (h.at(i)!=0.)
            f<<i<<"\t"<<h.at(i)<<"\t"<<h.val(i)<<"\t"<<h.val(i+1)<<endl;
    }

    f.close();
}

void WangLandau::saveG(const string filename) const
{
    ofstream f(filename);
    for (unsigned i=0;i<g.Intervals();i++){
        if (g.at(i)!=0.)
            f<<i<<"\t"<<g.at(i)<<"\t"<<g.val(i)<<"\t"<<g.val(i+1)<<endl;
    }

    f.close();
}

//критерий плоскости гистограммы
bool WangLandau::isFlat()
{
    for (unsigned i=0; i<h.Intervals(); i++){//плоскость гистограммы только в своем интервале
        if (h.at(i)!=0. && fabs(h.at(i)-average)/average > (1.0 - accuracy)) //критерий плоскости
            return false;
    }
    return true;
}

void WangLandau::updateGH(double E)
{
    if (E==0.){
        E = this->sys->E();
    }

    g[E]+=log(f);

    bool increased = (h[E]+=1) == 1;

    if (increased){ //прибавляем h и одновременно считаем среднее значение
        //случай если изменилось число ненулевых элементов
        hCount++;
        average = (average * (hCount-1) + 1) / hCount;
    } else {
        average += (1./(double)hCount);
    }
}

void WangLandau::resetH()
{
    for (unsigned i=0; i<h.Intervals(); i++){
        h.at(i)=0;
    }
    average=0.0;
    hCount=0;
}

void WangLandau::normalizeG()
{
    const double gFirst=g.at(0);
    for (unsigned i=0; i<g.Intervals(); i++){
        g.at(i)-=gFirst;
    }
}
