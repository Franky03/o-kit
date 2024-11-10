#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"
#include <iostream>
#include <climits>
#include <chrono>

using namespace std;

Solucao* finalSolution;

Solucao *ILS(Solucao *solucao, Data *data, int maxIter, int maxIterILS, double alpha){
    Solucao *bestSolution = new Solucao;
    bestSolution->latency = INT_MAX;
    vector<vector<Subsequence>> subseq_matrix(data->getDimension() + 1, vector<Subsequence>(data->getDimension() + 1));

    for(int i = 0; i<maxIter; i++){
        Solucao *newSolution = construction(solucao, data, alpha);
        UpdateAllSubsequences(newSolution, subseq_matrix, data);
        LocalSearch(newSolution, data, subseq_matrix);
        
        Solucao *localBest = newSolution;

        int iterILS = 0;
        while(iterILS < maxIterILS){
            newSolution = DoubleBridge(localBest , data);
            UpdateAllSubsequences(newSolution, subseq_matrix, data);
            LocalSearch(newSolution, data, subseq_matrix);

            if(newSolution->latency < localBest->latency){
                localBest = newSolution;
                iterILS = 0;

                std::cout << "Iteracao: " << i << " Custo: " << localBest->latency << std::endl;
            }

            iterILS++;
        }

        if(localBest->latency < bestSolution->latency){
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

    double sumCost = 0;
    double sumTime = 0;
    
    for (int i = 0; i < 10; i++) {
        Solucao solucao;
        for (int j = 1; j <= n; j++) {
            solucao.sequence.push_back(j);
        }
        solucao.sequence.push_back(1);

        auto start = std::chrono::high_resolution_clock::now();
        Solucao* finalSolution = ILS(&solucao, &data, 10, n < 100 ? n : 100, alpha);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        double timeTaken = duration.count();
        
        sumCost += finalSolution->latency;
        sumTime += timeTaken;

        std::cout << "Execução " << (i + 1) << " - Custo: " << finalSolution->latency << " - Tempo: " << timeTaken << " segundos" << std::endl;
    }

    // Cálculo das estatísticas
    double meanCost = sumCost / 10;
    double meanTime = sumTime / 10;

    // nome do arquivo: benchmarks/benchmark_<instancia>.txt
    std::string filename = "./benchmarks/benchmark_" + std::string(argv[1]) + ".txt";
    std::ofstream outFile(filename);
    outFile << "Instância;Alpha;Custo Médio;Tempo Médio" << std::endl;
    outFile << argv[1] << ";" << alpha << ";" << meanCost << ";" << meanTime << std::endl;
    outFile.close();


    return 0;
}
