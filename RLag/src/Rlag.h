#ifndef RLAG_H
#define RLAG_H
#include <iostream>
#include <vector>
#include "Kruskal.h"

 struct Lagrangean {
    vector<double> lambdas;
    vector<vector<bool>> solution;
    vector<int> subgrad;
    double cost = 0;
    int sum_subgrad = 0;
};

void solve_relag(Lagrangean &rlag, vvi &cost, double ub);

#endif