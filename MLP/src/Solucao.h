#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include "Data.h"

typedef struct {
    public:
        std::vector<int> sequence;
        double latency = 0;
} Solucao;

typedef struct Subsequence{
    double T, C;
    int W;
    int first, last; // primeiro e ultimo nó da subsequencia
    inline static Subsequence Concatenate(Subsequence &sigma_1, Subsequence &sigma_2, Data *data){
        Subsequence sigma;
        double temp = data->getDistance(sigma_1.last, sigma_2.first);
        sigma.W = sigma_1.W + sigma_2.W;
        sigma.T = sigma_1.T + sigma_2.T + temp;
        sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
        sigma.first = sigma_1.first;
        sigma.last = sigma_2.last;

        return sigma;
    }
} Subsequence;

void UpdateAllSubsequences(Solucao *s, vector<vector<Subsequence>> & subseq_matrix, Data *data);
void UpdateNeighborSubsequences(Solucao *s, vector<vector<Subsequence>> & subseq_matrix, Data *data, int begin, int end);
Solucao DoubleBridge(Solucao &solucao, Data *data);

void printSequence(const Solucao *solucao);

#endif