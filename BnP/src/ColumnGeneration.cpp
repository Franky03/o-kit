#include "ColumnGeneration.h"

std::vector<double> ColumnGenerationMinKnap(Data& data, Master& master, Knapsack& knap){
  IloNumArray *pi;
  std::pair<double, std::vector<bool>*> result;
  std::vector<double> solution;

  master.solve();
  int count = 0;
  while(true){
    pi = master.getDuals();
    result = knap.solveMinKnap(pi, data);
    
    if(1 - result.first >= 0 - 1e-6) {
      delete pi;
      delete result.second;
      break;
    }

    master.addColumn(*result.second);
    delete result.second;
    delete pi;

    master.solve();
    count++;
  }
  std::cout << "Number of columns: " << count << std::endl;

  solution = master.getLambdas();
  std::cout << "Solution: ";
  for(int i = 0; i < solution.size(); ++i){
    std::cout << solution[i] << " ";
  }
  std::cout << std::endl;
  return solution;

}

void ColumnGeneration(Data& data, Master& master, Knapsack& knap, ColumnNode *root){
  
  // forçar os lambdas para o branch and bound (força os items
  // separados e os juntos a ficarem juntos na relaxação linear)
  master.forceLambda(root->T, root->S);
  knap.changeConstraints(root->T, root->S);
  std::cout << "Forced and changed" << std::endl;
  IloNumArray *pi;
  double knap_result;
  double master_result;
  
  std::cout << "Solving master" << std::endl;
  master_result = master.solve();

  while(true){
    pi = master.getDuals();

    std::cout << "Got duals" << std::endl;
    
    knap.changeObjective(pi);
    std::cout << "Changed objective" << std::endl;
    knap_result = knap.solve();

    delete pi;
    
    if(1 - knap_result >= 0 - 1e-6){
      break;
    }
    
    std::cout << "Adding column" << std::endl;
    std::vector<bool> *column = knap.getPattern();
    master.addColumn(*column);
    delete column;

    master_result = master.solve();

  }

  root->value = master_result;
  root->solution = master.getLambdas();
}
