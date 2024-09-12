#include "clustermachine.h"

ClusterMachine::ClusterMachine(PartArray *sys, double interactionRange):
    sys(sys),
    interactionRange(interactionRange),
    lastSize(0)
{
    initNeightbours();
}

ClusterMachine::cluster ClusterMachine::max()
{
    findClusters();
    cluster maxCluster; unsigned max=0;
    for (const cluster& c : _clusters){
        if (c.size()>max){
            maxCluster = c;
            max = c.size();
        }
    }
    return maxCluster;
}

ClusterMachine::cluster ClusterMachine::min()
{
    findClusters();
    cluster minCluster = _clusters.at(0); unsigned min=minCluster.size();
    for (const cluster& c : _clusters){
        if (c.size()<min){
            minCluster = c;
            min = c.size();
        }
    }
    return minCluster;
}

ClusterMachine::clusterArray& ClusterMachine::all()
{
    findClusters();
    return _clusters;
}

double ClusterMachine::averageSize()
{
    findClusters();
    double avSize = 0;
    for (const cluster& c: _clusters)
        avSize+=c.size();
    avSize /= (double)_clusters.size();
    return avSize;
}

double ClusterMachine::maximalSize()
{
    findClusters();
    unsigned max=0;
    for (const cluster& c : _clusters){
        if (c.size()>max)
            max = c.size();
    }
    return max;
}

double ClusterMachine::minimalSize()
{
    findClusters();
    unsigned min=_clusters.at(0).size();
    for (const cluster& c : _clusters){
        if (c.size()<min)
            min = c.size();
    }
    return min;
}

void ClusterMachine::initNeightbours()
{
    if (lastSize!=sys->size()){
        neighbours.clear();
        for (Part* p1 : sys->parts){
            for (Part* p2 : sys->parts){
                if (isNeightbours(p1,p2))
                    neighbours[p1->Id()].push_front(p2);
            }
        }
        lastSize = sys->size();
    }
}

bool ClusterMachine::isConnected(const Part *p1, const Part *p2) const
{
    if (p1==p2) return false;
    return p2->interact(p1->pos).scalar(p1->m)>0;
}

bool ClusterMachine::isNeightbours(const Part *a, const Part *b) const
{
    if (a==b) return false;
    if (interactionRange==0) return true;
    else return a->pos.space(b->pos)<=interactionRange;
}

void ClusterMachine::findClusters()
{
    if (sys->state!=oldState){
        initNeightbours();
        _clusters.clear();

        unordered_set<unsigned> unWalked; //флаг,учавствует (или учавствовала) ли частица в кластере

        for (Part* temp : sys->parts){
            unWalked.insert(temp->Id());
        }

        Part* temp;
        temp = sys->getById(*unWalked.begin());
        while (temp !=0){
            cluster currentCluster;
            currentCluster.push_back(temp);
            unWalked.erase(temp->Id());//помечаем как пройденную

            walkNeighbours(temp,currentCluster, unWalked);

            _clusters.push_back(currentCluster);

            //получаем еще не пройденную частицу
            if (unWalked.size()>0)
                temp = sys->getById(*unWalked.begin());
            else
                temp=0;
        }
    }
}

void ClusterMachine::walkNeighbours(Part *part, ClusterMachine::cluster &currentCluster, unordered_set<unsigned> &unWalked)
{
    try{
        for(Part* temp: neighbours.at(part->Id())){
            bool walked = (unWalked.find(temp->Id())==unWalked.end());
            if (!walked && isConnected(part,temp)){
                unWalked.erase(temp->Id());
                currentCluster.push_back(temp);
                this->walkNeighbours(temp,currentCluster, unWalked);
            }
        }
    } catch (std::out_of_range& oor){(void)oor;};
}
