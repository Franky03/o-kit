#include "Knapsack.h"
#include "combo.c"

Knapsack::Knapsack(Data& data){
  
  IloExpr exp(env); // sum of weights
  
  numItems = data.getNItems();

  model = IloModel(env);
  
  cplex = IloCplex(model);
  x = IloBoolVarArray(env, numItems);
  constraints = IloRangeArray(env);
  var_constraints = IloRangeArray(env); // constraints para as variáveis x

  for(int i = 0; i < numItems; ++i){
    char var_name[100];
    sprintf(var_name, "x_%d", i+1);
    
    x[i].setName(var_name);
    exp += data.getItemWeight(i) * x[i];

  }

  constraints.add(exp <= data.getBinCapacity());
  
  obj = IloMaximize(env);
  model.add(obj);
  model.add(constraints);
  model.add(var_constraints);

}

Knapsack::~Knapsack(){
  model.end();
  cplex.end();
  env.end();
}

double Knapsack::solve(){
  cplex.setOut(std::cout); // Ativa o log
  cplex.exportModel("knapsack.lp"); // Exporta o modelo para análise

  cplex.setOut(env.getNullStream());
  cplex.solve();
  std::cout << "Objective value: " << cplex.getObjValue() << std::endl;
  return cplex.getObjValue();
}

std::vector<bool>* getBoolPattern(item *items, int numItems){
  std::vector<bool> *pattern = new std::vector<bool>(numItems);

  for(int i = 0; i < numItems; ++i){
    if(items[i].x){
      (*pattern)[items[i].index] = true;
    } else {
      (*pattern)[items[i].index] = false;
    }
  }

  return pattern;
}

std::pair<double, std::vector<bool>*> Knapsack::solveMinKnap(IloNumArray *pi, Data& data){
  int *w = new int[numItems]; // weight 
  item items[numItems];

  for(int i = 0; i < numItems; ++i){
    w[i] = data.getItemWeight(i);
    items[i].p = (itype) (*pi)[i] * M;
    items[i].w = (itype) w[i];
    items[i].x = false;
    items[i].index = i;
  }

  stype lb = 0;
  stype ub = numItems * M * M;

  stype z = combo(items, items + numItems-1, (stype)data.getBinCapacity(), lb, ub, true, false) / M;
  
  std::vector<bool> *pattern = getBoolPattern(items, numItems);
  
  delete[] w;

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
  
  model.remove(var_constraints);
  var_constraints.end(); 

  var_constraints = IloRangeArray(env);
  IloExpr exp(env);
  
  if(S != NULL) {
    for(int i = 0; i < S->size(); ++i){
      var_constraints.add(
        x[(*S)[i].first] + x[(*S)[i].second] <= 1
      ); // this is forcing the variables to be different (x1 + x2 <= 1)
    }
  }

  if(T != NULL) {
    for(int i = 0; i < T->size(); ++i){
      var_constraints.add(
        x[(*T)[i].first] - x[(*T)[i].second] == 0
      ); 
      // this is forcing the variables to be equal (x1 - x2 == 0)
    }
  }

  model.add(var_constraints);

}


std::vector<bool>* Knapsack::getPattern(){
  std::vector<bool> *solution = new std::vector<bool>(numItems);
  IloNumArray values(env, numItems);

  cplex.getValues(values, x);

  for(int i = 0; i < numItems; ++i){
    (*solution)[i] = values[i];
  }

  return solution;
}