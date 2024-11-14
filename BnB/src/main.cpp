#include <iostream>
using namespace std;

#include "data.h"
#include "hungarian.h"
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <numeric>
#include <chrono>

struct Node {
    std::vector<std::pair<int, int>> forbidden_arcs; // lista de arcos proibidos desse nó
    std::vector<std::vector<int>> subtour; // conjunto de subtour 
    double lower_bound; // limite inferior
    int chosen; // indice do menor subtour
    bool feasible; // se é viável
};

double readInstanceCost(std::string instance_name){
	std::string costs_file = "costs.tsp";
	std::ifstream file(costs_file);

	std::string line;
	while (std::getline(file, line)){
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ';')){
			tokens.push_back(token);
		}
		if (tokens[0] == instance_name){
			return std::stod(tokens[2]);
		}
	}

	exit(1);
}

vector<vector<int>> findSubtours(hungarian_problem_t *p){
	int n = p->num_rows;
	vector<vector<int>> subtours;
	vector<int> visited(n, 0);

	for(int start = 0; start < n; ++start){
		if(visited[start]) continue;

		vector<int> subtour;
		int current = start;

		// seguir o ciclo até retornar ao nó inicial
		do {
			subtour.push_back(current);
			visited[current] = 1;

			// encontrar o próximo nó na matriz
			int next = -1;
			for(int j = 0; j < n; ++j){
				if(p->assignment[current][j] == 1){
					next = j;
					break;
				}
			}

			current = next;
		} while(current != start && current != -1);

		// adiciona o subtour se for um ciclo fechado 
		if(current == start){
			subtour.push_back(start);
			subtours.push_back(subtour);
		}
	}

	std::sort(subtours.begin(), subtours.end(), [](const vector<int> &a, const vector<int> &b){
		return a.size() < b.size();
	});


	return subtours;
}

void printSubtours(vector<vector<int>> &subtours){
	std::cout << "{";
	for (int i = 0; i < subtours.size(); i++){
		std::cout << "{";
		for (int j = 0; j < subtours[i].size(); j++){
			std::cout << subtours[i][j];
			if (j != subtours[i].size()-1) std::cout << ",";
		}
		std::cout << "}";
		if (i != subtours.size()-1) std::cout << ",";
	}
	std::cout << "}" << std::endl;
}

void createNode(Node &node, hungarian_problem_t *p, double **cost){
	double **cost_copy = new double*[p->num_rows];

	for (int i = 0; i < p->num_rows; i++){
		cost_copy[i] = new double[p->num_cols];
		for (int j = 0; j < p->num_cols; j++){
			cost_copy[i][j] = cost[i][j];
		}
	}

	for (int i = 0; i < node.forbidden_arcs.size(); i++){
		cost_copy[node.forbidden_arcs[i].first][node.forbidden_arcs[i].second] = 1e9;
	}

	hungarian_init(p, cost_copy, p->num_rows, p->num_cols, HUNGARIAN_MODE_MINIMIZE_COST);
	node.lower_bound = hungarian_solve(p);
	node.subtour = findSubtours(p);
	node.feasible = node.subtour.size() == 1;
	node.chosen = 0;

	for (int i = 0; i < p->num_rows; i++) delete [] cost_copy[i];
	delete [] cost_copy;
	hungarian_free(p);
}

Node BranchAndBound(Data *data, double **cost, int mode){ // mode -> {1: DFS, 2: BFS}
	hungarian_problem_t p;
	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), HUNGARIAN_MODE_MINIMIZE_COST);

	std::list<Node> tree;

	double upper_bound = 1e9;

	Node root;
	createNode(root, &p, cost); // resolve AP_TSP para a instância inicial

	tree.push_back(root);

	Node best_node = root;
	
	while(!tree.empty()){
		Node current_node;
		if(mode == 1){
			current_node = tree.back(); // DFS
			tree.pop_back();
		} else {
			current_node = tree.front(); // BFS
			tree.pop_front();
		}

		if (current_node.lower_bound >= upper_bound) continue; // por limite superior

		if (current_node.feasible){
			if (current_node.lower_bound < upper_bound){
				upper_bound = current_node.lower_bound;
				best_node = current_node;
			}
		} else {
			// adiciona os filhos do nó atual
			for(int i = 0; i < current_node.subtour[current_node.chosen].size() - 1; ++i){
				Node child;
				child.forbidden_arcs = current_node.forbidden_arcs;

				child.forbidden_arcs.push_back(
					std::make_pair(
						current_node.subtour[current_node.chosen][i], 
						current_node.subtour[current_node.chosen][i+1]
					)
				);
				
				createNode(child, &p, cost);

				if(child.lower_bound < upper_bound){
					tree.push_back(child);
				}		
			}

		}

	}

	hungarian_free(&p);
	return best_node;
	
}

int main(int argc, char** argv) {

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}
	
	int mode = atoi(argv[2]) ? atoi(argv[2]) : 1;
	auto start = std::chrono::high_resolution_clock::now();
	Node optimal = BranchAndBound(data, cost, mode);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = end - start;

	std::cout << data->getInstanceName();
    printf(";%.d;%.2lf\n", (int)optimal.lower_bound, duration.count());

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}