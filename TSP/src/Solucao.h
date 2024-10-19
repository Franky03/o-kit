#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include "Data.h"

class Solucao {
    std::vector<int> route;
    double costSolution = 0;
};

void printRoute(const Solucao *solucao);
void calculateCost(Solucao *solucao, const Data *data);
Solucao *constructSolution(Solucao *solucao, const Data *data);

#endif