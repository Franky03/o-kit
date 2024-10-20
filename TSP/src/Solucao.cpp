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
    Solucao *newSolution = new Solucao;
    *newSolution = *solucao;

    std::vector<int> first_block, second_block;
    int b1_size, b2_size;

    b1_size = (rand() % (newSolution->route.size()/10)) + 2;
    b2_size = (rand() % (newSolution->route.size()/10)) + 2;

    first_block.reserve(b1_size);
    second_block.reserve(b2_size);


    int pb1 = (rand() % (newSolution->route.size() - (b1_size+1))) + 1;
    int pb2 = (rand() % (newSolution->route.size() - (b2_size+1))) + 1;

    while(pb2 < pb1 + b1_size && pb1 < pb2 + b2_size){
        pb2 = (rand() % (newSolution->route.size() - (b2_size+1))) + 1;
    }

    for(int i = 0; i < b1_size; i++){
        first_block.push_back(newSolution->route[pb1 + i]);
    }

    for(int i = 0; i < b2_size; i++){
        second_block.push_back(newSolution->route[pb2 + i]);
    }
   
    if(pb1 > pb2){
        newSolution->route.erase(newSolution->route.begin() + pb1, newSolution->route.begin() + pb1 + b1_size);
        newSolution->route.insert(newSolution->route.begin() + pb1, second_block.begin(), second_block.end());
        newSolution->route.erase(newSolution->route.begin() + pb2, newSolution->route.begin() + pb2 + b2_size);
        newSolution->route.insert(newSolution->route.begin() + pb2, first_block.begin(), first_block.end());
        
    }else{
        newSolution->route.erase(newSolution->route.begin() + pb2, newSolution->route.begin() + pb2 + b2_size);
        newSolution->route.insert(newSolution->route.begin() + pb2, first_block.begin(), first_block.end());
        newSolution->route.erase(newSolution->route.begin() + pb1, newSolution->route.begin() + pb1 + b1_size);
        newSolution->route.insert(newSolution->route.begin() + pb1, second_block.begin(), second_block.end());
    }
    
    calculateCost(newSolution, data);
    return newSolution;
}
