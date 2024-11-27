#ifndef RLAG_H
#define RLAG_H
#include <iostream>
#include <vector>
#include "Kruskal.h"

typedef struct Lagrangean {
    vector<double> lambdas;
    vector<vector<bool>> solution;
    vector<int> subgrad;
    double cost;
    int sum_subgrad;

    Lagrangean() : sum_subgrad(0), cost(0) {}

    Lagrangean(const Lagrangean& other)
        : solution(other.solution), 
        lambdas(other.lambdas), 
        subgrad(other.subgrad),
        cost(other.cost),
        sum_subgrad(other.sum_subgrad) {}
} Lagrangean;

Lagrangean* solve_relag(Lagrangean *rlag, vvi &cost, double ub);

#endif