#include "Construcao.h"
#include "Solucao.h"
#include <algorithm>

Solucao construction(Solucao* solucao, Data* data, double alpha){
    Solucao sol;

    std::vector<int> CL; // Candidate List
    int r = 1; // nó inicial

    for(int i = 2; i <= data->getDimension(); i++){
        CL.push_back(i);
    }

    sol.sequence.push_back(r);
    while(!CL.empty()){
        // ordena CL de acordo com a distancia de r
        std::sort(CL.begin(), CL.end(), [r, data](int a, int b){
            return data->getDistance(r, a) < data->getDistance(r, b);
        });

        int numCandidates = std::max(1, static_cast<int>(CL.size() * alpha));
        std::vector<int> RCL(CL.begin(), CL.begin() + numCandidates);

        int c = RCL[rand() % RCL.size()]; // escolhe um nó aleatório de RCL
        sol.sequence.push_back(c);
        
        sol.latency += data->getDistance(r, c);
        r = c;
        CL.erase(std::remove(CL.begin(), CL.end(), c), CL.end()); // remove c de CL
        
    }

    sol.sequence.push_back(1);

    return sol;
}