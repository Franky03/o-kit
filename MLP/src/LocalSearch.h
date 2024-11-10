#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Solucao.h"
#include "Data.h"

void LocalSearch(Solucao *solucao, Data *data, vector<vector<Subsequence>> subseq_matrix);
bool bestImprovementSwap(Solucao *solucao, Data *data, vector<vector<Subsequence>> subseq_matrix);
bool bestImprovement2Opt(Solucao *solucao, Data *data, vector<vector<Subsequence>> subseq_matrix);
bool bestImprovementOrOpt(Solucao *solucao, Data *data, vector<vector<Subsequence>> subseq_matrix, int k);

#endif
