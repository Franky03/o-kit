#ifndef BNB_H
#define BNB_H
#include <iostream>
#include <vector>

#include "Rlag.h"
#include "Kruskal.h"

struct Node {
    std::vector<std::pair<int, int>> forbidden_arcs; // lista de arcos proibidos desse nó
    std::vector<std::vector<int>> subtour; // conjunto de subtour 
    Lagrangean rlag;
    double lower_bound; // limite inferior
    int chosen; // indice do menor subtour
    bool feasible; // se é viável
};

Node BranchAndBound(vvi & cost, double ub);

#endif
