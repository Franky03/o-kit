#include "Solucao.h"
#include "Data.h"
#include <vector>
#include <algorithm>


void printRoute(const Solucao *solucao){
    std::cout << "Rota: ";
    for (int i = 0; i < solucao->route.size(); i++){
        std::cout << solucao->route[i] << " ";
    }
    std::cout << std::endl;
}

void calculateCost(Solucao *solucao, Data *data){
    solucao->costSolution = 0;
    for(int i = 0; i < solucao->route.size() - 1; i++){
        solucao->costSolution += data->getDistance(solucao->route[i], solucao->route[i+1]);
    }
}

Solucao* generateRandomSolution(Solucao *solucao){
    Solucao *newSolution = new Solucao;
    newSolution->route = {1};

    while(newSolution->route.size() < 4 ){
        int position = (rand() % (solucao->route.size() - 2)) + 1;
        int number = solucao->route.at(position);
         
        if((std::find(newSolution->route.begin(), newSolution->route.end(), number)) == newSolution->route.end()){
            newSolution->route.push_back(number);
        }
    }

    newSolution->route.push_back(1);

    return newSolution;
}

Solucao *getRemainingNodes(Solucao *solucao, Solucao* generatedSolution){
    Solucao* remaining = new Solucao;
    for(int i = 1; i < solucao->route.size() - 1; i++){
        if((std::find(generatedSolution->route.begin(), generatedSolution->route.end(), solucao->route[i])) == generatedSolution->route.end()){
            remaining->route.push_back(solucao->route[i]);
        }
    }
    return remaining;
}

Solucao *DoubleBridge(Solucao *solucao, Data *data){

    int n = solucao->route.size();
    int pos1 = 1 + (rand() % (n/4));
    int pos2 = pos1 + 1 + (rand() % (n/4));
    int pos3 = pos2 + 1 + (rand() % (n/4));

    Solucao *newSolution = new Solucao;
    newSolution->route.reserve(n);


    auto itBegin = solucao->route.begin();
    auto itPos1 = solucao->route.begin() + pos1;
    auto itPos2 = solucao->route.begin() + pos2;
    auto itPos3 = solucao->route.begin() + pos3;

    newSolution->route.insert(newSolution->route.end(), itBegin, itPos1); // A
    newSolution->route.insert(newSolution->route.end(), itPos2, itPos3); // C
    newSolution->route.insert(newSolution->route.end(), itPos1, itPos2); // B
    newSolution->route.insert(newSolution->route.end(), itPos3, solucao->route.end()); // D

    solucao->route = std::move(newSolution->route);
    calculateCost(solucao, data);

    return solucao;
}
