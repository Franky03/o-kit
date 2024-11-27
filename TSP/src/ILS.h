#ifndef ILS_H
#define ILS_H

#include "Data.h"
#include "Solucao.h"
#include "Construcao.h"
#include "LocalSearch.h"

Solucao *ILS(Solucao *solucao, Data *data, int maxIter, int maxIterILS, double alpha);

#endif