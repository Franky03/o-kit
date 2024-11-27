#include "BnB.h"
#include "Rlag.h"
#include "Kruskal.h"

#include <iostream>
#include <vector>
#include <utility>
#include <list>

int choose_best_degree(vector<int> subgrads){
    int best_i = 0;
    int lowest = subgrads[0];

    for(int i = 1; i < subgrads.size(); i++){
        if(subgrads[i] < lowest){
            lowest = subgrads[i];
            best_i = i;
        }
    }

    return best_i;
}

std::vector<int> get_subtour(std::vector<std::vector<bool>> solution, int chosen){
    std::vector<int> subtour;
    
    for(int i = 0; i < solution.size(); i++){
        if(solution[chosen][i]){
            subtour.push_back(i);
        }
    }

    return subtour;
}

void reset_costs(vvi &alt_cost, const vvi &cost, int dimension){
    for(int i = 0; i < cost.size(); i++){
        for(int j = 0; j < cost[i].size(); j++){
            alt_cost[i][j] = cost[i][j];
        }
    }
}

Node BranchAndBound(Data *data, vvi & cost, double ub){

    Node root, best;
    std::list<Node> tree;
    int n = data->getDimension();

    vvi alt_cost = cost;
    Lagrangean *rlag = new Lagrangean;

    solve_relag(rlag, cost, ub, n);

    std::cout << "SOLVED" << std::endl;

    root.rlag = *rlag;
    root.chosen = choose_best_degree(rlag->subgrad);
    root.subtour = get_subtour(rlag->solution, root.chosen); // encontrar as conexões do menor grau

    tree.push_back(root);

    while(!tree.empty()){
        Node current = tree.back();
        tree.pop_back();

        if(current.rlag.sum_subgrad == 0 && current.rlag.cost < ub){
            ub = current.rlag.cost;
            best = current;
            continue;
        }

        for(int i = 0; i < current.subtour.size(); i++){

            Node child;
            child.forbidden_arcs = current.forbidden_arcs;
            child.rlag = current.rlag;

            child.forbidden_arcs.push_back(std::make_pair(current.chosen, current.subtour[i]));
            reset_costs(alt_cost, cost, n);

            for(int j=0; j<child.forbidden_arcs.size(); j++){
                alt_cost[child.forbidden_arcs[j].first][child.forbidden_arcs[j].second] = 999999;
                alt_cost[child.forbidden_arcs[j].second][child.forbidden_arcs[j].first] = 999999;
            }

            solve_relag(&child.rlag, alt_cost, ub, n);

            if(child.rlag.cost < ub){
                child.chosen = choose_best_degree(child.rlag.subgrad);
                child.subtour = get_subtour(child.rlag.solution, child.chosen);

                tree.push_back(child);
            }
            
        }
    }

    return best;

}