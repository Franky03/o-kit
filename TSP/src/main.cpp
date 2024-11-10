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
            std::cout << "Iteracao: " << i << " Custo: " << bestSolution->costSolution << std::endl;
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

    std::vector<double> costs;
    std::vector<double> times;
    
    for (int i = 0; i < 10; i++) {
        Solucao solucao;
        for (int j = 1; j <= n; j++) {
            solucao.route.push_back(j);
        }
        solucao.route.push_back(1);

        auto start = std::chrono::high_resolution_clock::now();
        Solucao* finalSolution = ILS(&solucao, &data, 50, n > 150 ? n / 2 : n, alpha);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        double timeTaken = duration.count();
        
        costs.push_back(finalSolution->costSolution);
        times.push_back(timeTaken);

        std::cout << "Execução " << (i + 1) << " - Custo: " << finalSolution->costSolution 
                  << " - Tempo: " << timeTaken << " segundos" << std::endl;
    }

    // Cálculo das estatísticas
    double meanCost = calculateMean(costs);
    double meanTime = calculateMean(times);

    // nome do arquivo: benchmarks/benchmark_<instancia>.txt
    std::string filename = "./benchmarks/benchmark_" + std::string(argv[1]) + ".txt";
    std::ofstream outFile(filename);
    outFile << "Instância;Alpha;Custo Médio;Tempo Médio" << std::endl;
    outFile << argv[1] << ";" << alpha << ";" << meanCost << ";" << meanTime << std::endl;
    outFile.close();


    return 0;
}
