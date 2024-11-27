#include <iostream>
#include <vector>
#include <chrono>

#include "Data.h"
#include "BnB.h"
#include "Rlag.h"

using namespace std;

int main(int argc, char** argv){
    chrono::time_point<std::chrono::system_clock> start, end;
    Node best_node;

    Data * data = new Data(argc, argv[1]);
    data->readData();

	const int dimension = data->getDimension();

    const double upperBound = 10629.0;
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
    cout << "Custo da solução: " << best_node.lower_bound << endl;

    return 0;
}