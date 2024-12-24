#include "Knapsack.h"
#include "minknap.c"

Knapsack::Knapsack(Data& data){
  
  IloExpr exp(env); // sum of weights
  
  numItems = data.getNItems();

  model = IloModel(env);
  obj = IloMaximize(env);
  cplex = IloCplex(model);
  x = IloBoolVarArray(env, numItems);
  constraints = IloRangeArray(env);
  constraints2 = IloRangeArray(env); // constraints para as variáveis x

  for(int i = 0; i < numItems; ++i){
    char var_name[100];
    sprintf(var_name, "x_%d", i+1);
    
    x[i].setName(var_name);
    exp += data.getItemWeight(i) * x[i];

  }

  constraints.add(exp <= data.getBinCapacity());
  obj.setExpr(exp);

  model.add(obj);
  model.add(constraints);
  model.add(constraints2);

}

Knapsack::~Knapsack(){
  model.end();
  cplex.end();
  env.end();
}

double Knapsack::solve(){
  cplex.setOut(env.getNullStream());
  cplex.solve();
  return cplex.getObjValue();
}

std::pair<double, std::vector<bool>*> Knapsack::solveMinKnap(IloNumArray *pi, Data &data){
  long int *p = new long int[numItems]; // profit

  for(int i = 0; i < numItems; ++i){
    p[i] = (*pi)[i] * M;
  }

  int *w = new int[numItems]; // weight

  for(int i = 0; i < numItems; ++i){
    w[i] = data.getItemWeight(i);
  }

  int *x = new int[numItems]; // solution

  double z = minknap(numItems, p, w, x, data.getBinCapacity()) / M;

  std::vector<bool> *pattern = new std::vector<bool>(numItems);

  for(int i = 0; i < numItems; ++i){
    (*pattern)[i] = x[i];
  }

  delete[] p;
  delete[] w;
  delete[] x;

  return std::make_pair(z, pattern);
}

void Knapsack::changeObjective(IloNumArray *pi){
  IloExpr exp(env);

  for(int i = 0; i < numItems; ++i){
    exp += (*pi)[i] * x[i]; // muda o ponto extremo (?)
  }

  obj.setExpr(exp);
  
}

void Knapsack::changeConstraints(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S){
  
  model.remove(constraints2);
  constraints2.end(); 

  constraints2 = IloRangeArray(env);
  IloExpr exp(env);


}
