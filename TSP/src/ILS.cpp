#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"
#include <iostream>
#include <climits>

Solucao *ILS(Solucao *solucao, Data *data, int maxIter, int maxIterILS, double alpha){
    Solucao *bestSolution = new Solucao;
    bestSolution->costSolution = INT_MAX;

    for(int i = 0; i<maxIter; i++){
        Solucao *newSolution = construction(solucao, data, alpha);
        LocalSearch(newSolution, data);
        Solucao *localBest = new Solucao;
        *localBest = *newSolution;

        int iterILS = 0;
        while(iterILS < maxIterILS){
            newSolution = DoubleBridge(localBest, data);
            LocalSearch(newSolution, data);

            if(newSolution->costSolution < localBest->costSolution){
                localBest = newSolution;
                iterILS = 0;
            }

            iterILS++;
        }

        if(localBest->costSolution < bestSolution->costSolution){
            *bestSolution = *localBest;
            //std::cout << "Iteracao: " << i << " Custo: " << bestSolution->costSolution << std::endl;
        }
    }

    return bestSolution;
}

