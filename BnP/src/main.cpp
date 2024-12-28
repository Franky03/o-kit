#include "Data.h"
#include <stdio.h>
#include <iostream>
#include <list>
#include <chrono>
#include <numeric>

#include "Master.h"
#include "Knapsack.h"
#include "ColumnGeneration.h"

bool isFeasible(std::vector<double> &solution)
{
    for(int i = 0; i < solution.size(); ++i)
    {
        if(!(solution[i] - 1 >= 0 - 1e-6 || solution[i] + 1 <= 1 + 1e-6))
        {
            return false;
        }
    }

    return true;
}

double BranchAndPrice(Data &data){
  Master *master = new Master(data);
  Knapsack *knap = new Knapsack(data);

  std::list<ColumnNode*> tree;
  ColumnNode *root = new ColumnNode();

  double ub = 1e9;
  // resolve a geração de colunas para o min knapsack (subproblema)
  root->solution = ColumnGenerationMinKnap(data, master, knap);
  root->value = std::accumulate(root->solution.begin(), root->solution.end(), 0.0);
  root->isFeasible = isFeasible(root->solution);
    
  std::cout << "Root value: " << root->value << "\n";
  std::cout << "Root is feasible: " << root->isFeasible << "\n";

  if(root->isFeasible){
    ub = root->value;
    delete root;
    return ub;
  }

  
  ColumnNode *together = new ColumnNode(root); // first node
  ColumnNode *separated = new ColumnNode(root); // second node
  
  std::pair<int,int> fraction = master->getMostFractional();
   
  together->T.push_back(fraction);
  separated->S.push_back(fraction);

  tree.push_back(together);
  tree.push_back(separated);

  
  while(!tree.empty()){
    ColumnNode *node = tree.back();

    tree.pop_back();
      
    ColumnGeneration(data, master, knap, node);

    node->isFeasible = isFeasible(node->solution);

    if(node->isFeasible){
      if(node->value < ub){
        ub = node->value;
      }
    }

    else if(node->value < ub - 1) {
      std::cout << "Node value: " << node->value << "\n";

      std::pair<int,int> fraction = master->getMostFractional();
      
      together = new ColumnNode(node);
      separated = new ColumnNode(node);
      // ele herda os valores do pai
      together->T.push_back(fraction);
      separated->S.push_back(fraction);
      // adiciona novos valores para os filhos (os fractions)
      tree.push_back(together);
      tree.push_back(separated);
    }

  }
  
  delete master;
  delete knap;
  for(auto it = tree.begin(); it != tree.end(); ++it){
    delete *it;
  }

  return ub;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage:\n./bin instance\n");
        return 0;
    }

    Data data;
    data.readData(argv[1]);

    std::cout << "Instance: " << argv[1] << "\n";
    std::cout << "Number of items: " << data.getNItems() << "\n";

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    double result = BranchAndPrice(data);
    std::cout << "Result: " << result << "\n";

    end = std::chrono::system_clock::now();

    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time (s): " << elapsed_seconds / 1000.0 << "\n\n";

    return 0;
}
