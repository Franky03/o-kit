#include "ColumnGeneration.h"

std::vector<double> ColumnGenerationMinKnap(Data& data, Master* master, Knapsack* knap){
  IloNumArray *pi;
  std::pair<double, std::vector<bool>*> result;
  std::vector<double> solution;

  master->solve();

  while(true){
    pi = master->getDuals();
    result = knap->solveMinKnap(pi, data);

    delete pi;
    
    if(1 - result.first >= 0 - 1e-6) {
      delete result.second;
      break;
    }

    master->addColumn(*result.second);

    delete result.second;

    master->solve();
    
  }
  
  solution = master->getLambdas();
 
  return solution;

}

void ColumnGeneration(Data& data, Master* master, Knapsack* knap, ColumnNode *root){
  
  // forçar os lambdas para o branch and bound (força os items
  // separados e os juntos a ficarem juntos na relaxação linear)
  master->forceLambda(&root->T, &root->S);
  knap->changeConstraints(&root->T, &root->S);

  IloNumArray *pi;
  double knap_result;
  double master_result;
  master_result = master->solve();

  while(true){
    pi = master->getDuals();
    
    knap->changeObjective(pi);
    knap_result = knap->solve();

    delete pi;
    
    if(1 - knap_result >= 0 - 1e-6){
      break;
    }
    
    std::vector<bool> *column = knap->getPattern();
    master->addColumn(*column);
    delete column;

    master_result = master->solve();

  }

  root->value = master_result;
  root->solution = master->getLambdas();
}
