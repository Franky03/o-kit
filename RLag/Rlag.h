#ifndef RLAG_H
#define RLAG_H
#include <iostream>
#include <vector>
#include "Kruskal.h"

struct Lagrangean {
    vector<double> lambdas;
    vector<vector<double>> solution;
    vector<int> subgrad;
    double cost;
    int sum_subgrad;
};

void solveRlag(Lagrangean *rlag, vvi &cost, double ub);

#endif