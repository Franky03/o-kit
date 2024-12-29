#include "Master.h"

Master::Master(Data& data){
  
  IloExpr exp(env);

  numItems = data.getNItems();
  
  model = IloModel(env);

  cplex = IloCplex(model);
  lambdas = IloNumVarArray(env, numItems, 0, IloInfinity);
  constraints = IloRangeArray(env);
  
  columns = std::vector<std::vector<bool>>(numItems, std::vector<bool>(numItems, false));

  for(int i = 0; i < numItems; ++i){
    char var_name[100];
    sprintf(var_name, "lambda_%d", i);
    
    lambdas[i].setName(var_name);
    exp += lambdas[i];
    
    constraints.add(lambdas[i] == 1);
    columns[i][i] = true;

  }

  obj = IloMinimize(env, exp);
  model.add(obj);
  model.add(constraints);

  cplex.exportModel("master.lp");
}

Master::~Master(){
  env.end();
}

double Master::solve(){
  cplex.setOut(env.getNullStream());
  cplex.solve();
  return cplex.getObjValue();
}

void Master::addColumn(std::vector<bool> &column){
  IloNumColumn col = obj(1);

  for(int i = 0; i < numItems; ++i){
    col += constraints[i](column[i]);
  }
  columns.push_back(column);
  
  char var_name[100];
  sprintf(var_name, "lambda_%d", (int)columns.size());
  
  lambdas.add(IloNumVar(col, 0, IloInfinity));
  lambdas[lambdas.getSize() - 1].setName(var_name);
}

IloNumArray* Master::getDuals(){
  IloNumArray* duals = new IloNumArray(env, numItems);
  cplex.getDuals(*duals, constraints);

//  std::cout << "Duals: ";
//  for(int i = 0; i < numItems; ++i){
//    std::cout << (*duals)[i] << " ";
//  }
//  std::cout << std::endl;

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

std::pair<int, int> Master::getMostFractional(){
  std::pair<int, int> mostFractional = std::make_pair(-1, -1);

  double mf_val = 1e9;

  // search in a matrix (i, j) where the lambda is most fractional 
  // (closest to 0.5)
  // for example:
  // if lambda_5 = 0.275 and the column that is linked to 
  // lambda_5 is 0 1 1 1 0, so, sum lambda value in the element 
  // 2x3 2x4 3x4 (assuming the first element of matrix is 1x1)
  // then i see the element that is closest to 0.5 
  // and return the pair of items that are linked to this column
  
  for(int i=0; i < numItems; ++i){
    for(int j=i+1; j < numItems; ++j){
      double fval = 0;
      bool frac = false;

      for(int k=0; k < columns.size(); ++k){
        if(columns[k][i] && columns[k][j]){
          // se a coluna k tem os itens i e j juntos 
          fval += cplex.getValue(lambdas[k]);
          frac = true;
        } 
      }
      
      if(fabs(fval - 0.5) < mf_val && frac){
        mf_val = fabs(fval - 0.5);
        mostFractional = std::make_pair(i, j);
      }

    }
  }

  return mostFractional;
}

std::vector<int> *Master::getBannedLambdas(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S){
  
  std::vector<int> *bannedLambdas = new std::vector<int>();

  if(T != NULL){
    for(int i = 0; i < T->size(); ++i){
      for(int j = 0; j < columns.size(); ++j){
        if(columns[j][(*T)[i].first] != columns[j][(*T)[i].second]){
          bannedLambdas->push_back(j); // ban if the items are separated
        }
      }
    }
  }

  if(S != NULL){
    for(int i = 0; i < S->size(); ++i){
      for(int j = 0; j < columns.size(); ++j){
        if(columns[j][(*S)[i].first] && columns[j][(*S)[i].second]){
          bannedLambdas->push_back(j); // ban if the items are together
        }
      }
    }
  }

  std::sort(bannedLambdas->begin(), bannedLambdas->end());
  bannedLambdas->erase(std::unique(bannedLambdas->begin(), bannedLambdas->end()), bannedLambdas->end());

  return bannedLambdas;
}

void Master::forceLambda(std::vector<std::pair<int, int>> *T, std::vector<std::pair<int, int>> *S){
  
  std::vector<int> *bannedLambdas = getBannedLambdas(T, S);
  
  for(int i = 0; i < lambdas.getSize(); ++i){
    if(std::find(bannedLambdas->begin(), bannedLambdas->end(), i) != bannedLambdas->end()){
      lambdas[i].setUB(0);
    }
    else {
      lambdas[i].setUB(IloInfinity);
    }
  }

  delete bannedLambdas;
  
}

