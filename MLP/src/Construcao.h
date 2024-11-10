#ifndef CONSTRUCAO_H
#define CONSTRUCAO_H

#include <vector>
#include "Data.h"
#include "Solucao.h"

typedef struct {
    int k_inserted;
    int inserted_position;
    int removed_position;
    double cost; // delta ao inserir k na aresta {i,j}
} Insercao; 

Solucao construction(Solucao* solucao, Data* data, double alpha);

#endif