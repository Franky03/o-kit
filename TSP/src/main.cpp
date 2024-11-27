#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"
#include <iostream>
#include <climits>
#include <chrono>
#include "ILS.h"

using namespace std;

Solucao* finalSolution;

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
            solucao.route.push_back(j);
        }
        solucao.route.push_back(1);

        auto start = std::chrono::high_resolution_clock::now();
        Solucao* finalSolution = ILS(&solucao, &data, 50, n > 150 ? n / 2 : n, alpha);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        double timeTaken = duration.count();
        
        sumCost += finalSolution->costSolution;
        sumTime += timeTaken;

    }

    // Cálculo das estatísticas
    double meanCost = sumCost / 10;
    double meanTime = sumTime / 10;

    std::cout << data.getInstanceName();
    printf(";%.2lf;%.2lf\n", meanTime, meanCost);

    return 0;
}
