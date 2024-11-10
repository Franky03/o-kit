#include "Solucao.h"
#include "LocalSearch.h"
#include <utility>
#include <algorithm>

bool bestImprovementSwap(Solucao *solucao, Data *data,vector<vector<Subsequence>> &subseq_matrix){
    int best_i = -1, best_j = -1;
    int size = solucao->sequence.size();
    double bestLatency = solucao->latency;

    Subsequence s1, s2, s3, s4;
    for(int i = 1; i < size - 1; ++i){
        for(int j = i + 1; j < size - 1; ++j){

            if(i != j - 1){
                s1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][j], data);
                s2 = Subsequence::Concatenate(s1, subseq_matrix[i + 1][j - 1], data);
                s3 = Subsequence::Concatenate(s2, subseq_matrix[i][i], data);
                s4 = Subsequence::Concatenate(s3, subseq_matrix[j + 1][size - 1], data);   
            }

            else{
                s1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][j], data);
                s2 = Subsequence::Concatenate(s1, subseq_matrix[i][i], data);
                s4 = Subsequence::Concatenate(s2, subseq_matrix[j + 1][size - 1], data);
            }
            if (s4.C < bestLatency) {
                bestLatency = s4.C;
                best_i = i;
                best_j = j;
            }
        } 
    }

    if (bestLatency < solucao->latency){
        std::swap(solucao->sequence[best_i], solucao->sequence[best_j]);
        UpdateNeighborSubsequences(solucao, subseq_matrix, data, best_i, best_j);
        return true;
    }

    return false;
}

bool bestImprovement2Opt(Solucao *solucao, Data *data, vector<vector<Subsequence>> &subseq_matrix){
    double bestLatency = solucao->latency;
    int best_i = -1, best_j = -1;
    int n = solucao->sequence.size();

    Subsequence s1, s2;

    for(int i =1; i < n - 1; ++i){
        for(int j = i+2; j < n - 1; ++j){
            s1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j-1][i], data);
            s2 = Subsequence::Concatenate(s1, subseq_matrix[j][n-1], data);

            if(s2.C < bestLatency){
                bestLatency = s2.C;
                best_i = i;
                best_j = j;
            }
        }
    }   

    if (bestLatency < solucao->latency){
       int k_swaps = (best_j - best_i)/2;
       for(int k = 0; k < k_swaps; ++k){
           std::swap(solucao->sequence[best_j - 1 - k], solucao->sequence[best_i + k]);
       }
       UpdateNeighborSubsequences(solucao, subseq_matrix, data, best_i, best_j);
       return true;
    }
    return false;
}

bool bestImprovementOrOpt(Solucao *solucao, Data *data, vector<vector<Subsequence>> &subseq_matrix, int k){
    double bestLatency = solucao->latency;
    int best_i = -1, best_j = -1;
    int n = solucao->sequence.size();

    Subsequence s1, s2, s3;

    for(int i = 1; i < n - k - 1; ++i){
        for(int j = 0; j < n - 1; ++j){
            if(i - j <= 1 && i - j >= -(k-1)) continue;

            if (i>j){
                s1 = Subsequence::Concatenate(subseq_matrix[0][j], subseq_matrix[i + k - 1][i], data);
                s2 = Subsequence::Concatenate(s1, subseq_matrix[j + 1][i - 1], data);
                s3 = Subsequence::Concatenate(s2, subseq_matrix[i + k][n-1], data); 
            }

            else {
                s1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[i + k][j], data);
                s2 = Subsequence::Concatenate(s1, subseq_matrix[i+k-1][i], data);
                s3 = Subsequence::Concatenate(s2, subseq_matrix[j+1][n-1], data);
            }

            if(s3.C < bestLatency){
                bestLatency = s3.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestLatency < solucao->latency){
        if(best_i > best_j){
            std::reverse(solucao->sequence.begin() + best_i, solucao->sequence.begin() + best_i + k);
            std::rotate(solucao->sequence.begin() + best_j + 1, solucao->sequence.begin() + best_i, solucao->sequence.begin() + best_i + k);
            UpdateNeighborSubsequences(solucao, subseq_matrix, data, best_j, best_i + k - 1);
        } else {
            std::reverse(solucao->sequence.begin() + best_i, solucao->sequence.begin() + best_i + k);
            std::rotate(solucao->sequence.begin() + best_i, solucao->sequence.begin() + best_i + k, solucao->sequence.begin() + best_j + 1);
            UpdateNeighborSubsequences(solucao, subseq_matrix, data, best_i, best_j);
        }
       
        return true;
    }
    return false;
}

void LocalSearch(Solucao *solucao, Data *data, vector<vector<Subsequence>> &subseq_matrix){
    std::vector<int> methods = {0, 1, 2, 3, 4};
    bool improved = false;
    int count = 0;

    while(!methods.empty()){
        int n = rand() % methods.size();
        switch(methods[n]){
            case 0:
                improved = bestImprovementSwap(solucao, data, subseq_matrix);
                break;
            case 1:
                improved = bestImprovementOrOpt(solucao, data, subseq_matrix, 1);
                break;
            case 2:
                improved = bestImprovement2Opt(solucao, data, subseq_matrix);
                break;
            case 3:
                improved = bestImprovementOrOpt(solucao, data, subseq_matrix, 2);
                break;
            case 4:
                improved = bestImprovementOrOpt(solucao, data, subseq_matrix, 3);
                break;
        }

        if(improved){
            methods = {0, 1, 2, 3, 4};
            count++;    
        }else{
            methods.erase(methods.begin() + n);
        }
    }

}