#ifndef CLUSTERMACHINE_H
#define CLUSTERMACHINE_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include "PartArray.h"
#include "StateMachineFree.h"


class ClusterMachine
{
public:
    typedef vector<Part*> cluster;
    typedef vector<cluster> clusterArray;

    ClusterMachine(PartArray *sys, double interactionRange);

    cluster max();
    cluster min();
    clusterArray &all();
    double averageSize();
    double maximalSize();
    double minimalSize();


private:
    PartArray *sys;
    double interactionRange;
    unsigned lastSize;
    void initNeightbours();
    bool isConnected(const Part* p1, const Part* p2) const;
    bool isNeightbours(const Part* a, const Part* b) const;
    unordered_map<unsigned,forward_list <Part*> > neighbours;

    void findClusters();
    void walkNeighbours(Part *part, cluster &currentCluster, unordered_set<unsigned> &unWalked);
    StateMachineFree oldState;
    clusterArray _clusters;
};

#endif // CLUSTERMACHINE_H
