#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Rlag.h"
#include <limits>
#include <cmath>

#define MIN_EPSILON 5e-4
#define ITER_MAX 30
#define B 2

vvi calculate_costs(vvi &cost, vector<double> &lambdas){
    vvi alt_costs = cost;
    for(int i = 0; i < cost.size(); i++){
        for(int j = 0; j < cost[i].size(); j++){
            alt_costs[i][j] = cost[i][j] - lambdas[i] - lambdas[j];
        }
    }
    return alt_costs;
}

vector<vector<bool>> create_solution(vector<pair<int, int>> &edges){
    vector<vector<bool>> solution(edges.size(), vector<bool>(edges.size(), false));
    std::cout << "DEBUG 15" << std::endl;
    for(int i = 0; i < edges.size(); i++){
        solution[edges[i].first][edges[i].second] = true;
        solution[edges[i].second][edges[i].first] = true;
        std::cout << "i / size: " << i << " / " << edges.size() << std::endl;
    }
    std::cout << "DEBUG 16" << std::endl;
    return solution;
}


pair<int, int> add_0_(vvi &alt_costs, double mst_cost, int nodes, vector<pair<int, int>> &edges){
    // encontrar os nós que estão mais próximos de 0
    double best_cost_a = numeric_limits<double>::max();
    double best_cost_b = numeric_limits<double>::max();

    pair<int, int> best_nodes = make_pair(-1, -1);

    for(int i=0; i < nodes; i++){
        if(alt_costs[0][i] < best_cost_a){
            best_cost_a = alt_costs[0][i];
            best_nodes.first = i;
        }
    }

    for(int i=0; i < nodes; i++){
        if(alt_costs[0][i] < best_cost_b && i != best_nodes.first){
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
    vector<int> subgrad(solution.size(), 0); 
    subgrad[0] = 0;

    for(int i = 1; i < solution.size(); ++i){
        int count = B;

        for(int j = i+1; j < solution[i].size(); ++j){ // examina arestas pra frente
            if(solution[i][j]){
                count--;
            }
        }

        for(int j = i-1; j >= 0; --j){ // examina arestas pra trás
            if(solution[j][i]){
                count--;
            }
        }

        subgrad[i] = count;
    }

    return subgrad;
}

int sum_subgradientes(vector<int> &subgrad){
    int sum = 0;
    for(int i = 0; i < subgrad.size(); i++){
        sum += subgrad[i] * subgrad[i];
    }
    return sum;
}

void solve_relag(Lagrangean *rlag, vvi &cost, double ub, int nodes){
    double epsilon = 1.0;
    int iter = 0;
    double lr = 0;

    vector<double> lambdas;
    vector<vector<bool>> solution;

    Lagrangean best_rlag = *rlag;
    lambdas = rlag->lambdas.empty() ? vector<double>(cost.size(), 0.0) : rlag->lambdas;
    rlag->lambdas = lambdas;

    vvi alt_costs;
    
    while(epsilon > MIN_EPSILON){
        std::cout << "START" << std::endl;
        alt_costs = calculate_costs(cost, lambdas);

        Kruskal mst(alt_costs);
        std::cout << "DEBUG 7" << std::endl;
        double mst_cost = mst.MST(nodes);
        std::cout << "DEBUG 8" << std::endl;
        
        mst_cost += (2 * sum_lambdas(lambdas));
        std::cout << "DEBUG 9" << std::endl;

        vector<pair<int, int>> mst_edges = mst.getEdges();
        std::cout << "DEBUG 10" << std::endl;
        pair<int, int> best_nodes_to_0 = add_0_(alt_costs, mst_cost, nodes, mst_edges);
        std::cout << "DEBUG 11" << std::endl;
        
        mst_edges.push_back(make_pair(0, best_nodes_to_0.first));
        mst_edges.push_back(make_pair(0, best_nodes_to_0.second));

        std::cout << "DEBUG 12" << std::endl;

        mst_cost += alt_costs[0][best_nodes_to_0.first] + alt_costs[0][best_nodes_to_0.second];
        vector<int> graus = get_degrees(mst_edges);

        std::cout << "DEBUG 13" << std::endl;

        solution = create_solution(mst_edges);

        std::cout << "DEBUG 14" << std::endl;

        std::cout << "DEBUG 4" << std::endl;
        vector<int> subgrad = get_subgradient(solution);
        std::cout << "DEBUG 5" << std::endl;
        int sum_subgrad = sum_subgradientes(subgrad);
        std::cout << "DEBUG 6" << std::endl;
        

        if(mst_cost > best_rlag.cost){
            std::cout << "DEBUG A" << std::endl;
            best_rlag.cost = mst_cost;
            best_rlag.lambdas = lambdas;
            best_rlag.solution = solution;
            best_rlag.subgrad = subgrad;
            best_rlag.sum_subgrad = sum_subgrad;
            iter = 0;
            std::cout << "DEBUG" << std::endl;
        }

        else{
            std::cout << "DEBUG 1" << std::endl;
            iter++;
            if (iter >= ITER_MAX){
                iter = 0;
                epsilon /= 2;
            }
            std::cout << "DEBUG B" << std::endl;
        }
        std::cout << "DEBUG 2" << std::endl;
        lr = sum_subgrad == 0 ? 0 : (epsilon * (ub - mst_cost)) / sum_subgrad;
        update_lambda(lambdas, lr, graus);
        std::cout << "DEBUG 3" << std::endl;

        if(lr <= 1e-6){
            best_rlag.cost = mst_cost;
            best_rlag.lambdas = lambdas;
            best_rlag.solution = solution;
            best_rlag.subgrad = subgrad;
            best_rlag.sum_subgrad = sum_subgrad;
            break;
        }

        else if (ub - best_rlag.cost < 1e-6){
            best_rlag.cost = INFINITY;
            break;
        }
    }

    *rlag = best_rlag;
}