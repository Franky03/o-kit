#include "Construcao.h"
#include "Solucao.h"
#include <algorithm>

bool compareCost(Insercao a, Insercao b){
    return a.cost < b.cost;
}

void printInsertionCost(std::vector<Insercao> *insertionCost){
    for(auto i: *(insertionCost)){
        std::cout << "Inserted Node: " << i.k_inserted << " at position: " << i.inserted_position << " removed node at position: " << i.removed_position << " with cost: " << i.cost << std::endl;
    }
    printf("\n");
}

void insertInSolution(Solucao *solucao, Solucao* remainingNodes, Insercao *insercao){
    solucao->route.insert(solucao->route.begin()+insercao->removed_position, insercao->k_inserted); // adiciona o nó na posição removida
    remainingNodes->route.erase(remainingNodes->route.begin() + insercao->inserted_position); // remove o nó da lista de nós restantes
}

std::vector<Insercao> calculateInsertionCost(Solucao *solucao, Solucao* remainingNodes, Data *data){
    std::vector<Insercao> insertionCost((solucao->route.size()-1) * remainingNodes->route.size());
    
    int l = 0;

    for(int i = 1; i < solucao->route.size(); ++i){
        int prev = solucao->route[i-1];
        int actual = solucao->route[i];

        for(int j = 0; j < remainingNodes->route.size(); ++j){
            int k = remainingNodes->route[j];
            insertionCost[l].k_inserted = k;
            insertionCost[l].inserted_position = j;
            insertionCost[l].removed_position = i;
            insertionCost[l].cost = data->getDistance(prev, k) + data->getDistance(k, actual) - data->getDistance(prev, actual);
            l++;
        }

    }
    return insertionCost;
}

Solucao* construction(Solucao* solucao, Data* data, double alpha){
    Solucao* sol = generateRandomSolution(solucao);
    Solucao* remainingNodes = getRemainingNodes(solucao, sol);
    

    while(!remainingNodes->route.empty()){
        std::vector<Insercao> insertionCost = calculateInsertionCost(sol, remainingNodes, data);
        std::sort(insertionCost.begin(), insertionCost.end(), compareCost);
        int q = rand() % (int)(ceil(alpha * insertionCost.size()));
        insertInSolution(sol, remainingNodes, &insertionCost[q]);
    }

    std::cout << "Solucao Construida: ";

    calculateCost(sol, data);
    delete remainingNodes;
    return sol;
}