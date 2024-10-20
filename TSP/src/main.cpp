#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"
#include <iostream>
#include <climits>

using namespace std;

Solucao* finalSolution;

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
            }

            iterILS++;
        }

        if(localBest->costSolution < bestSolution->costSolution){
            *bestSolution = *localBest;
            std::cout << "Iteracao: " << i << " Custo: " << bestSolution->costSolution << std::endl;
        }
    }

    return bestSolution;
}

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    double alpha = argv[2] ? atof(argv[2]) : 0.5;
    data.read();
    size_t n = data.getDimension();

    Solucao solucao;
    for(int i = 1; i <= n; i++){
        solucao.route.push_back(i);
    }
    solucao.route.push_back(1);

    finalSolution = ILS(&solucao, &data, 50, n > 150 ? n/2 : n, alpha);
    printRoute(finalSolution);
    cout << "Custo da Solucao Construida: " << finalSolution->costSolution << endl;

    return 0;
}
