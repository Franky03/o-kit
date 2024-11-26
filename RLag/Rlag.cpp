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

pair<int, int> find_best_egdes(vvi &alt_costs, double mst_cost, int nodes, vector<pair<int, int>> &edges){
    double best_cost = numeric_limits<double>::max();
    pair<int, int> best_edges;
    
    return best_edges;
}

double sum_lambdas(vector<double> &lambdas){
    double sum = 0;
    for(int i = 0; i < lambdas.size(); i++){
        sum += lambdas[i];
    }
    return sum;
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
        mst_cost += (2 * sum_lambdas(lambdas));

        vector<pair<int, int>> mst_edges = mst.getEdges();
        pair<int, int> edges = find_best_egdes(alt_costs, mst_cost, alt_costs.size() - 1, mst_edges);

        
    }
}