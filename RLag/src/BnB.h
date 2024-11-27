#ifndef BNB_H
#define BNB_H
#include <iostream>
#include <vector>

#include "Rlag.h"
#include "Data.h"
#include "Kruskal.h"

typedef struct Node {
    std::vector<std::pair<int, int>> forbidden_arcs; // lista de arcos proibidos desse nó
    std::vector<int> subtour; // conjunto de subtour 
    Lagrangean rlag;
    double lower_bound; // limite inferior
    int chosen; // indice do menor subtour
    bool feasible; // se é viável
} Node;

Node BranchAndBound(Data *data, vvi & cost, double ub);

#endif
