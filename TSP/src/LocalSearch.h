#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Solucao.h"
#include "Data.h"

void LocalSearch(Solucao *solucao, Data *data);
bool bestImprovementSwap(Solucao *solucao, Data *data);
bool bestImprovementReinsert(Solucao *solucao, Data *data);
bool bestImprovement2Opt(Solucao *solucao, Data *data);
bool bestImprovementOrOpt(Solucao *solucao, Data *data, int k);

#endif
