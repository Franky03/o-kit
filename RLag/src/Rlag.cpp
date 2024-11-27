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
            cost[i][j] -= lambdas[i] + lambdas[j];
        }
    }
}

vector<vector<bool>> create_solution(vii &edges, ii &best_nodes_to_0){
    vector<vector<bool>> solution(edges.size() + 2, vector<bool>(edges.size() + 2, false));
    std::cout << "first: " << best_nodes_to_0.first << " second: " << best_nodes_to_0.second << std::endl;
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

    pair<int, int> best_nodes;


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
    vector<int> graus(edges.size(), 0);
    for(int i = 0; i < edges.size(); i++){  
        graus[edges[i].first]++;
        graus[edges[i].second]++;
    }
    return graus;
}

void update_lambda(vector<double> &lambdas, double lr, vector<int> &graus){
    lambdas[0] = 0;
    for(int i = 1; i < lambdas.size(); i++){
        lambdas[i] += (lr * (B - graus[i]));
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

void solve_relag(Lagrangean *rlag, vvi &cost, double ub, int nodes){
    double epsilon = 1.0;
    int iter = 0;
    double lr = 0;

    vector<double> lambdas;
    vector<vector<bool>> solution;

    Lagrangean best_rlag = *rlag;
    lambdas = best_rlag.lambdas.empty() ? vector<double>(cost.size(), 0) : rlag->lambdas;
    rlag->lambdas = lambdas;

    while(epsilon > MIN_EPSILON){
        std::cout << "Start: " << best_rlag.cost << std::endl;
        vvi alt_costs = cost;
        calculate_costs(alt_costs, lambdas);

        Kruskal mst(remove_nodes(alt_costs));
        double mst_cost = mst.MST(alt_costs.size() - 1);
        
        mst_cost += (2 * sum_lambdas(lambdas));

        vector<pair<int, int>> mst_edges = mst.getEdges();

        pair<int, int> best_nodes_to_0 = add_0_(alt_costs);
        
        mst_cost += alt_costs[0][best_nodes_to_0.first] + alt_costs[0][best_nodes_to_0.second];

        vector<int> graus = get_degrees(mst_edges);
        std::cout << "Before create_solution" << std::endl;
        solution = create_solution(mst_edges, best_nodes_to_0);
        std::cout << "After create_solution" << std::endl;

        vector<int> subgrad = get_subgradient(solution);
        int sum_subgrad = sum_subgradientes(subgrad);
        std::cout << "After get_subgradient" << std::endl;

        if(mst_cost > best_rlag.cost){
            best_rlag.cost = mst_cost;
            best_rlag.lambdas = lambdas;
            best_rlag.solution = solution;
            best_rlag.subgrad = subgrad;
            best_rlag.sum_subgrad = sum_subgrad;
            iter = 0;
        }

        else{
            iter++;
            if (iter >= ITER_MAX){
                iter = 0;
                epsilon /= 2;
            }
        }

        if(sum_subgrad == 0){
            best_rlag.cost = mst_cost;
            best_rlag.lambdas = lambdas;
            best_rlag.solution = solution;
            best_rlag.subgrad = subgrad;
            best_rlag.sum_subgrad = sum_subgrad;
            break;
        }
        else if (ub - best_rlag.cost < 1 - 1e-2){
            best_rlag.cost = INFINITY;
            break;
        }

        lr = sum_subgrad == 0 ? 0 : (epsilon * (ub - mst_cost)) / sum_subgrad;

        for(int i = 0; i < lambdas.size(); i++){
            lambdas[i] += lr * subgrad[i];
        }

        std::cout << "Final: " << best_rlag.cost << std::endl;
    }

    *rlag = best_rlag;
}