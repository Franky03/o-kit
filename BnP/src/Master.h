#ifndef MASTER_H
#define MASTER_H

#include <ilcplex/ilocplex.h>
#include "Data.h"
#include <vector>
#include <algorithm>

class Master {
  IloEnv env;
  IloModel model;
  IloObjective obj;
  IloCplex cplex;
  IloNumVarArray lambdas;
  IloRangeArray constraints;

  std::vector<std::vector<int>> columns;

  int numItems;
  
  public:
    Master(Data& data);
    ~Master();
    double solve();
    void addColumn(std::vector<int> &column);
    IloNumArray* getDuals();
    std::vector<double> getLambdas();
    // this is for branch and bound
    std::vector<int> *getBannedLambdas(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S);
    void forceLambda(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S);

};

#endif
