#ifndef COLGEN_H
#define COLGEN_H

#include <vector>
#include "Data.h"
#include "Knapsack.h"
#include "Master.h"

struct ColumnNode {
  std::vector<std::pair<int, int>> *T; // together items
  std::vector<std::pair<int, int>> *S; // separated items
  
  std::vector<double> solution;
  bool isFeasible;
  double value;

  ColumnNode(){}

  ColumnNode(ColumnNode *parent){
    T = parent->T;
    S = parent->S;
    solution = parent->solution;
    isFeasible = parent->isFeasible;
    value = parent->value;
  }

};

std::vector<double> ColumnGenerationMinKnap(Data& data, Master& master, Knapsack& knap);

void ColumnGeneration(Data& data, Master& master, Knapsack& knap, ColumnNode *root);

#endif
