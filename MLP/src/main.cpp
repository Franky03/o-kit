#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"
#include <iostream>
#include <climits>
#include <chrono>

using namespace std;

Solucao ILS(Solucao *solucao, Data *data, int maxIter, int maxIterILS, double alpha,  vector<vector<Subsequence>> &subseq_matrix){
    Solucao bestSolution;
    bestSolution.latency = INFINITY;

    for(int i = 0; i<maxIter; i++){
        Solucao newSolution = construction(solucao, data, alpha);
        UpdateAllSubsequences(&newSolution, subseq_matrix, data);
        LocalSearch(&newSolution, data, subseq_matrix);
        
        Solucao localBest = newSolution;

        int iterILS = 0;
        while(iterILS < maxIterILS){
            newSolution = DoubleBridge(localBest , data);
            UpdateAllSubsequences(&newSolution, subseq_matrix, data);
            LocalSearch(&newSolution, data, subseq_matrix);
            
            if(newSolution.latency + 1e-6 < localBest.latency){
                localBest = newSolution;
                iterILS = 0;
            }

            iterILS++;
        }

        if(localBest.latency < bestSolution.latency){
            bestSolution = localBest;
        }
    }

    return bestSolution;
}

double calculateMean(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) sum += value;
    return sum / values.size();
}

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    double alpha = argv[2] ? atof(argv[2]) : 0.5;
    data.read();
    size_t n = data.getDimension();

    vector<vector<Subsequence>> subseq_matrix(n + 1, vector<Subsequence>(n + 1));

    double sumCost = 0;
    double sumTime = 0;
    
    for (int i = 0; i < 10; i++) {
        Solucao solucao;
        for (int j = 1; j <= n; j++) {
            solucao.sequence.push_back(j);
        }
        solucao.sequence.push_back(1);

        auto start = std::chrono::high_resolution_clock::now();
        Solucao finalSolution = ILS(&solucao, &data, 10, n < 100 ? n : 100, alpha, subseq_matrix);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        double timeTaken = duration.count();
        
        sumCost += finalSolution.latency;
        sumTime += timeTaken;

    }

    // Cálculo das estatísticas
    double meanCost = sumCost / 10;
    double meanTime = sumTime / 10;

    // format: instance_name mean_time mean_cost 

    std::cout << data.getInstanceName();
    printf(";%.2lf;%.2lf\n", meanTime, meanCost);


    return 0;
}
