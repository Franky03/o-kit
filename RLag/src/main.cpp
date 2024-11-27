#include <iostream>
#include <vector>
#include <chrono>

#include "Data.h"
#include "BnB.h"
#include "Rlag.h"
#include "../TSP/src/ILS.h"

using namespace std;

int main(int argc, char** argv){
    chrono::time_point<std::chrono::system_clock> start, end;
    Node best_node;

    Data * data = new Data(argc, argv[1]);
    data->readData();

	const int dimension = data->getDimension();

    Solucao solucao;
    for (int j = 1; j <= dimension; j++) {
        solucao.route.push_back(j);
    }
    solucao.route.push_back(1);

    Solucao *finalSolution = ILS(&solucao, data, 50, dimension > 150 ? dimension / 2 : dimension, 0.5);

    const double upperBound = finalSolution->costSolution;
    std::vector<vector<double>> cost;
    for(int i = 0; i < dimension; i++){
        vector<double> row;
        for(int j = 0; j < dimension; j++){
            row.push_back(data->getDistance(i, j));
        }
        cost.push_back(row);
    }

    for(int i = 0; i < dimension; i++){
        cost[i][i] = INFINITY;
    }

    start = chrono::high_resolution_clock::now();
    best_node = BranchAndBound(data, cost, upperBound);
    end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed_seconds = end - start;

    cout << "Tempo de execução: " << elapsed_seconds.count() << "s" << endl;
    cout << "Custo da solução: " << best_node.rlag.cost << endl;

    return 0;
}