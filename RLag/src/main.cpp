#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>

#include "Data.h"
#include "BnB.h"
#include "Rlag.h"

using namespace std;

double runTSP(const string& instancePath){
    string command = "cd ../TSP && make && ./tsp ../instances/" + instancePath + ".tsp";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "Erro ao executar o comando: " << command << endl;
        exit(1);
    }

    char buffer[128];
    string output;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    pclose(pipe);

    // Processar a saída para extrair `meanCost`
    istringstream iss(output);
    string line;
    double meanCost = INFINITY;

    while (getline(iss, line)) {
        // Encontrar a linha contendo o resultado final
        if (line.find(";") != string::npos) {
            size_t lastSemicolon = line.rfind(';');
            if (lastSemicolon != string::npos) {
                meanCost = stod(line.substr(lastSemicolon + 1));
            }
        }
    }

    if (meanCost == INFINITY) {
        cerr << "Erro: não foi possível extrair meanCost da saída do TSP" << endl;
        exit(1);
    }

    return meanCost;
}

int main(int argc, char** argv){
    chrono::time_point<std::chrono::system_clock> start, end;
    Node best_node;

    Data * data = new Data(argc, argv[1]);
    data->readData();

	const int dimension = data->getDimension();

    double tsp_cost = runTSP(data->getInstanceName());
    std::cout << "Custo do TSP: " << tsp_cost << std::endl;
    const double upperBound = tsp_cost * 1.5;
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