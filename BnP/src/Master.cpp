#include "Master.h"

Master::Master(Data& data){
  
  IloExpr exp(env);

  numItems = data.getNItems();
  
  model = IloModel(env);
  obj = IloMinimize(env);
  cplex = IloCplex(model);
  lambdas = IloNumVarArray(env, numItems, 0.0, IloInfinity);
  constraints = IloRangeArray(env);
  
  columns = std::vector<std::vector<int>>(numItems, std::vector<int>(numItems, 0));

  for(int i = 0; i < numItems; ++i){
    char var_name[100];
    sprintf(var_name, "lambda_%d", i);
    
    lambdas[i].setName(var_name);
    exp += lambdas[i];
    
    constraints.add(lambdas[i] == 1);
    columns[i][i] = 1;

  }

  obj.setExpr(exp);
  model.add(obj);
  model.add(constraints);
}

Master::~Master(){
  model.end();
  cplex.end();
  env.end();
}

double Master::solve(){
  cplex.setOut(env.getNullStream());
  cplex.solve();
  return cplex.getObjValue();
}

void Master::addColumn(std::vector<int> &column){
  IloNumColumn col = obj(1);

  for(int i = 0; i < numItems; ++i){
    col += constraints[i](column[i]);
  }

  columns.push_back(column);
  
  char var_name[100];
  sprintf(var_name, "lambda_%d", (int)columns.size());
  
  lambdas.add(IloNumVar(col, 0.0, IloInfinity));
  lambdas[lambdas.getSize() - 1].setName(var_name);
}

IloNumArray* Master::getDuals(){
  IloNumArray* duals = new IloNumArray(env, numItems);
  cplex.getDuals(*duals, constraints);
  return duals;
}

std::vector<double> Master::getLambdas(){
  IloNumArray values(env);
  cplex.getValues(values, lambdas);
  std::vector<double> lambdas_(values.getSize());

  for(int i = 0; i < values.getSize(); ++i){
    lambdas_[i] = values[i];
  }

  return lambdas_;
}


