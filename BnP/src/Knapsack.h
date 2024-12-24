#ifndef KNAP_H
#define KNAP_H

#define M 1e6

#include <vector>
#include <ilcplex/ilocplex.h>
#include "Data.h"

class Knapsack {
  IloEnv env;
  IloModel model;
  IloCplex cplex;
  IloObjective obj;
  IloRangeArray constraints;

  IloRangeArray constraints2;
  IloBoolVarArray x;

  int numItems;

  public:
    Knapsack(Data& data);
    ~Knapsack();
    double solve();
    std::pair<double, std::vector<bool>*> solveMinKnap(IloNumArray *pi, Data &data);
    std::vector<bool>* getPattern();
    void changeObjective(IloNumArray *pi);
    void changeConstraints(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S);
};

#endif
