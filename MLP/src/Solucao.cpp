#include "Solucao.h"
#include "Data.h"
#include <vector>
#include <algorithm>


void printSequence(const Solucao *solucao){
    std::cout << "Sequencia: ";
    for (int i = 0; i < solucao->sequence.size(); i++){
        std::cout << solucao->sequence[i] << " ";
    }
    std::cout << std::endl;
}

void printSubseqMatrix(const std::vector<std::vector<Subsequence>> & subseq_matrix){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < subseq_matrix[i].size(); j++){
            std::cout << "Subsequencia [" << i << "][" << j << "]: " << subseq_matrix[i][j].T << " " << subseq_matrix[i][j].C << " " << subseq_matrix[i][j].W << std::endl;
        }
    }
}

void UpdateAllSubsequences(Solucao *s, vector<vector<Subsequence>> & subseq_matrix, Data *data){
    int n = s->sequence.size();

    for(int i = 0; i<n; ++i){
        int v = s->sequence[i];
        subseq_matrix[i][i].W = (i > 0);
        subseq_matrix[i][i].T = 0;
        subseq_matrix[i][i].C = 0;
        subseq_matrix[i][i].first = v;
        subseq_matrix[i][i].last = v;
    }

    for(int i = 0; i < n; ++i){
        for(int j = i + 1; j < n; ++j){
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], data);
        }
    }

    // necessário para atualização de subsequências invertidas (2-opt)
    for(int i = n-1; i >= 0; --i){
        for(int j = i - 1; j >= 0; --j){
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], data);
        }
    }

    s->latency = subseq_matrix[0][data->getDimension()].C;
}

void UpdateNeighborSubsequences(Solucao *s, vector<vector<Subsequence>> & subseq_matrix, Data *data, int begin, int end){
    int n = s->sequence.size();

    for(int i=begin; i <= end; ++i){
        subseq_matrix[i][i].first = s->sequence[i]; // atualiza o primeiro nó da subsequência, pois ele pode ter mudado de posição
        subseq_matrix[i][i].last = s->sequence[i]; 
    }

    for(int i = 0; i <= end; ++i){
        for(int j = begin; j < n; ++j){

            if (i >= j) continue;
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], data);
        }
    }

    for(int i = n - 1; i >= begin; --i){
        for(int j = end; j >= 0; --j){
            if (i <= j) continue;
            subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], data);
        }
    }

    s->latency = subseq_matrix[0][data->getDimension()].C;
}

Solucao *DoubleBridge(Solucao *solucao, Data *data){
    Solucao *newSolution = solucao;

    std::vector<int> first_block, second_block;
    int b1_size, b2_size;

    b1_size = (rand() % (newSolution->sequence.size()/10)) + 2;
    b2_size = (rand() % (newSolution->sequence.size()/10)) + 2;

    first_block.reserve(b1_size);
    second_block.reserve(b2_size);


    int pb1 = (rand() % (newSolution->sequence.size() - (b1_size+1))) + 1;
    int pb2 = (rand() % (newSolution->sequence.size() - (b2_size+1))) + 1;

    while(pb2 < pb1 + b1_size && pb1 < pb2 + b2_size){
        pb2 = (rand() % (newSolution->sequence.size() - (b2_size+1))) + 1;
    }

    for(int i = 0; i < b1_size; i++){
        first_block.push_back(newSolution->sequence[pb1 + i]);
    }

    for(int i = 0; i < b2_size; i++){
        second_block.push_back(newSolution->sequence[pb2 + i]);
    }
   
    if(pb1 > pb2){
        newSolution->sequence.erase(newSolution->sequence.begin() + pb1, newSolution->sequence.begin() + pb1 + b1_size);
        newSolution->sequence.insert(newSolution->sequence.begin() + pb1, second_block.begin(), second_block.end());
        newSolution->sequence.erase(newSolution->sequence.begin() + pb2, newSolution->sequence.begin() + pb2 + b2_size);
        newSolution->sequence.insert(newSolution->sequence.begin() + pb2, first_block.begin(), first_block.end());
        
    }else{
        newSolution->sequence.erase(newSolution->sequence.begin() + pb2, newSolution->sequence.begin() + pb2 + b2_size);
        newSolution->sequence.insert(newSolution->sequence.begin() + pb2, first_block.begin(), first_block.end());
        newSolution->sequence.erase(newSolution->sequence.begin() + pb1, newSolution->sequence.begin() + pb1 + b1_size);
        newSolution->sequence.insert(newSolution->sequence.begin() + pb1, second_block.begin(), second_block.end());
    }
    
    return newSolution;
}