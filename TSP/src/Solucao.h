#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include "Data.h"

class Solucao {
    public:
    std::vector<int> route;
    double costSolution = 0;
};

void printRoute(const Solucao *solucao);
void calculateCost(Solucao *solucao, Data *data);
Solucao *generateRandomSolution(Solucao *solucao);

#endif