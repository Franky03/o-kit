#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Rlag.h"
#include <limits>
#include <cassert>
#include <cmath>

#define MIN_EPSILON 5e-4
#define ITER_MAX 30
#define B 2

void calculate_costs(vvi &cost, vector<double> &lambdas){
    for(int i = 0; i < cost.size(); i++){
        for(int j = 0; j < cost.size(); j++){
            if(i == j){
                continue;
            }
            assert(i < lambdas.size() && j < lambdas.size());
            cost[i][j] = cost[i][j] - lambdas[i] - lambdas[j];
        }
    }
}

vector<vector<bool>> create_solution(vii &edges, ii &best_nodes_to_0){
    vector<vector<bool>> solution(edges.size() + 2, vector<bool>(edges.size() + 2, false));

    solution[0][best_nodes_to_0.first] = solution[best_nodes_to_0.first][0] = 
    solution[0][best_nodes_to_0.second] = solution[best_nodes_to_0.second][0] = true;

    for(auto node : edges){
        solution[node.first+1][node.second+1] = solution[node.second+1][node.first+1] = true;
    }
    return solution;
}


pair<int, int> add_0_(vvi &alt_costs){
    double best_cost_a = INFINITY;
    double best_cost_b;

    pair<int, int> best_nodes = {0, 0};

    for(int i = 1; i < alt_costs[0].size(); i++){
        if(alt_costs[0][i] < best_cost_a){
            best_cost_b = best_cost_a;
            best_nodes.second = best_nodes.first;
            
            best_cost_a = alt_costs[0][i];
            best_nodes.first = i;

        } else if(alt_costs[0][i] < best_cost_b){
            best_cost_b = alt_costs[0][i];
            best_nodes.second = i;
        }
    }
    
    return best_nodes;
}


double sum_lambdas(vector<double> &lambdas){
    double sum = 0;
    for(int i = 0; i < lambdas.size(); i++){
        sum += lambdas[i];
    }
    return sum;
}

vector<int> get_degrees(vector<pair<int,int>> &edges){
    vector<int> graus(edges.size()+1, 0);
    for(int i = 0; i < edges.size(); i++){  
        graus[edges[i].first]++;
        graus[edges[i].second]++;
    }
    return graus;
}

inline void update_lambda(vector<double> &lambdas, double lr ,vector<int> subgrad){
    for(int i = 0; i < lambdas.size(); i++){
        lambdas[i] += (lr * subgrad[i]);
    }
}

vector<int> get_subgradient(const vector<vector<bool>> &solution){
    // indica o grau residual de cada nó, quanto falta ou sobra para ele ter grau B
    vector<int> subgrad; 
    subgrad.push_back(0);

    for(int i = 1; i < solution.size(); ++i){
        int count = B;

        for(int j = i+1; j < solution.size(); ++j){ // examina arestas pra frente
            if(solution[i][j]){
                count--;
            }
        }

        for(int j = i-1; j >= 0; --j){ // examina arestas pra trás
            if(solution[j][i]){
                count--;
            }
        }

        subgrad.push_back(count);
    }

    return subgrad;
}

int sum_subgradientes(vector<int> &subgrad){
    int sum = 0;
    for(int i = 0; i < subgrad.size(); i++){
        sum += (subgrad[i] * subgrad[i]);
    }
    return sum;
}

vvi remove_nodes(vvi costs){
    costs.erase(costs.begin());
    for(auto &v : costs){
        v.erase(v.begin());
    }
    return costs;
}

void solve_relag(Lagrangean &rlag, vvi &cost, double ub) {
    double epsilon = 1.0;
    int iter = 0;
    double lr = 0;

    vector<double> lambdas;

    lambdas = rlag.lambdas.empty() ? vector<double>(cost.size(), 0) : rlag.lambdas;
    rlag.lambdas = lambdas;

    while (epsilon > MIN_EPSILON) {
        vvi alt_costs = cost;
        calculate_costs(alt_costs, lambdas);

        Kruskal mst(remove_nodes(alt_costs));
        double mst_cost = mst.MST(alt_costs.size() - 1);

        mst_cost += (2 * sum_lambdas(lambdas));

        vector<pair<int, int>> mst_edges = mst.getEdges();
        pair<int, int> best_nodes_to_0 = add_0_(alt_costs);

        mst_cost += alt_costs[0][best_nodes_to_0.first] + alt_costs[0][best_nodes_to_0.second];

        vector<int> graus = get_degrees(mst_edges);

        vector<vector<bool>> solution(mst_edges.size() + 2, vector<bool>(mst_edges.size() + 2, false));
        solution = create_solution(mst_edges, best_nodes_to_0);

        vector<int> subgrad = get_subgradient(solution);
        int sum_subgrad = sum_subgradientes(subgrad);

        if (mst_cost > rlag.cost) {
            rlag.cost = mst_cost;
            rlag.lambdas = lambdas;
            rlag.solution = solution;
            rlag.subgrad = subgrad;
            rlag.sum_subgrad = sum_subgrad;
            iter = 0;
        } else {
            iter++;
            if (iter >= ITER_MAX) {
                iter = 0;
                epsilon /= 2;
            }
        }

        if (sum_subgrad == 0) {
            rlag.cost = mst_cost;
            rlag.lambdas = lambdas;
            rlag.solution = solution;
            rlag.subgrad = subgrad;
            rlag.sum_subgrad = sum_subgrad;
            break;
        } else if (ub - mst_cost < 1 - 1e-2) {
            rlag.cost = INFINITY;
            break;
        }
        lr = (epsilon * (ub - mst_cost)) / sum_subgrad;

        update_lambda(lambdas, lr, subgrad);
    }
}
