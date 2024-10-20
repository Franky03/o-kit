#ifndef CONSTRUCAO_H
#define CONSTRUCAO_H

#include <vector>
#include "Data.h"
#include "Solucao.h"

typedef struct {
    int k_inserted;
    int inserted_position;
    int removed_position;
    double cost; // delta ao inserir k na aresta {i,j}
}Insercao; 

void printInsertionCost(std::vector<Insercao> *insertionCost);
void insertInSolution(Solucao *solucao, Solucao* remainingNodes, Insercao *insercao);
std::vector<Insercao> calculateInsertionCost(Solucao *solucao, Solucao* remainingNodes, Data *data);
Solucao* construction(Solucao* solucao, Data* data, double alpha);

#endif