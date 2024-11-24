#include "BnB.h"
#include "Rlag.h"
#include "Kruskal.h"

#include <iostream>
#include <vector>
#include <utility>
#include <list>

Node BranchAndBound(vvi &cost, double ub){
    std::list<Node> tree;
    int n = cost.size();

    vvi alt_cost = cost;
    Lagrangean *rlag = new Lagrangean;
    solveRlag(rlag, cost, ub);


}