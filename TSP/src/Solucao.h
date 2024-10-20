#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include "Data.h"

typedef struct {
    public:
    std::vector<int> route;
    double costSolution = 0;
} Solucao;

void printRoute(const Solucao *solucao);
void calculateCost(Solucao *solucao, Data *data);
Solucao *generateRandomSolution(Solucao *solucao);
Solucao *getRemainingNodes(Solucao *solucao, Solucao* generatedSolution);
Solucao *DoubleBridge(Solucao *solucao, Data *data);
;

#endif