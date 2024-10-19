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
    Solucao *newSolution = new Solucao();
    newSolution->route = {1};

    while(newSolution->route.size() < 4 ){
        int position = (rand() % (solucao->route.size() - 1));
        int number = solucao->route.at(position);
        
        if((std::find(newSolution->route.begin(), newSolution->route.end(), number)) == newSolution->route.end()){
            newSolution->route.push_back(number);
        }
    }

    newSolution->route.push_back(1);

    return newSolution;
}
