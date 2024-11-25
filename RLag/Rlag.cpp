#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Rlag.h"
#include <limits>

#define MIN_EPSILON 5e-4

vvi calculate_costs(vvi &cost, vector<double> &lambdas){
    vvi alt_costs = cost;
    for(int i = 0; i < cost.size(); i++){
        for(int j = 0; j < cost[i].size(); j++){
            alt_costs[i][j] = cost[i][j] - lambdas[i] - lambdas[j];
        }
    }
    return alt_costs;
}

void solveRlag(Lagrangean *rlag, vvi &cost, double ub){
    double epsilon = 1.0;
    int iter = 0;

    vector<double> lambdas;
    vector<vector<double>> solution;

    Lagrangean best_rlag = *rlag;
    lambdas = rlag->lambdas.empty() ? vector<double>(cost.size(), 0.0) : rlag->lambdas;
    rlag->lambdas = lambdas;

    vvi alt_costs = calculate_costs(cost, lambdas);

    while(epsilon > MIN_EPSILON){
        Kruskal mst(alt_costs);
        double mst_cost = mst.MST(alt_costs.size());

        
    }
}