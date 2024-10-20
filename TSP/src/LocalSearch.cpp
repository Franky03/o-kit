#include "Solucao.h"
#include "LocalSearch.h"
#include <utility>
#include <algorithm>

bool bestImprovementSwap(Solucao *solucao, Data *data){
    double bestDelta = 0;
    int best_i = -1, best_j = -1;

    for(int i = 1; i < solucao->route.size() -1; ++i){
        int a = solucao->route[i];
        int a_prev = solucao->route[i-1];
        int a_next = solucao->route[i+1];
        for(int j = i+1; j < solucao->route.size() - 1; ++j){
            int b = solucao->route[j];
            int b_prev = solucao->route[j-1];
            int b_next = solucao->route[j+1];

            double delta;
            if ( j != i+1 ){
                // remove a distancia entre a e a_prev e a e a_next
                delta = -data->getDistance(a, a_prev) - data->getDistance(a, a_next);
                // remove a distancia entre b e b_prev e b e b_next
                delta -= data->getDistance(b, b_prev) - data->getDistance(b, b_next);
                // adiciona a distancia entre b e a_prev e b e a_next
                delta += data->getDistance(b, a_prev) + data->getDistance(b, a_next);
                // adiciona a distancia entre a e b_prev e a e b_next
                delta += data->getDistance(a, b_prev) + data->getDistance(a, b_next);
            }
            else {
                // remove a distancia entre a e a_prev
                delta = -data->getDistance(a, a_prev);
                // remove a distancia entre b e b_next
                delta -= data->getDistance(b, b_next);

                // adiciona a distancia entre b e a_prev
                delta += data->getDistance(b, a_prev);
                // adiciona a distancia entre a e b_next
                delta += data->getDistance(a, b_next);
            }

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        } 
    }

    if (bestDelta < 0){
        std::swap(solucao->route[best_i], solucao->route[best_j]);
        solucao->costSolution += bestDelta;
        return true;
    }
    return false;
}

bool bestImprovementReinsert(Solucao *solucao, Data *data){
    return bestImprovementOrOpt(solucao, data, 1);
}

bool bestImprovement2Opt(Solucao *solucao, Data *data){
    double bestDelta = 0;
    int best_i = -1, best_j = -1;
    int n = solucao->route.size();

    for(int i =0; i < n - 1; ++i){
        int a = solucao->route[i];
        int a_next = solucao->route[i+1];

        for(int j = i+2; j < n - 1; ++j){
            int b = solucao->route[j];
            int b_next = solucao->route[j+1];

            double delta;
            // remove a distancia entre a e a_next e b e b_next
            delta = -data->getDistance(a, a_next) - data->getDistance(b, b_next);
            // adiciona a distancia entre a e b e a_next e b_next
            delta += data->getDistance(a, b) + data->getDistance(a_next, b_next);
           
            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i + 1;
                best_j = j + 1;
            }
        }
    }   

    if (bestDelta < 0){
        std::reverse(solucao->route.begin() + best_i, solucao->route.begin() + best_j);
        solucao->costSolution += bestDelta;
        return true;
    }
    return false;
}

bool bestImprovementOrOpt(Solucao *solucao, Data *data, int k){
    double bestDelta = 0;
    int best_i = -1, best_j = -1;
    int n = solucao->route.size();

    for(int i = 1; i < n - k; ++i){
        int a_prev = solucao->route[i-1];
        int a_start = solucao->route[i];
        int a_end = solucao->route[i+k-1];
        int a_next = solucao->route[i+k];

        for(int j = 1; j < n; j++){
            int b_prev = solucao->route[j-1];
            int b = solucao->route[j];

            if((j+k-1) >= i && i>=(j-k)) continue;

            double delta;
            // remove a distancia entre a_prev e a_start e a_end e a_next
            delta = -data->getDistance(a_prev, a_start) - data->getDistance(a_end, a_next);
            // adiciona a distancia entre a_prev e a_next
            delta += data->getDistance(a_prev, a_next);
            // remove a distancia entre b_prev e b
            delta -= data->getDistance(b_prev, b);
            // adiciona a distancia entre b_prev e a_start e a_end e b
            delta += data->getDistance(b_prev, a_start) + data->getDistance(a_end, b);

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }

        }
    }

    if (bestDelta < 0){
        Solucao newSol = *solucao;
        solucao->route.insert(solucao->route.begin() + best_j, newSol.route.begin() + best_i, newSol.route.begin() + best_i + k);
        if(best_i < best_j){
            solucao->route.erase(solucao->route.begin() + best_i, solucao->route.begin() + best_i + k);
        }else{
            solucao->route.erase(solucao->route.begin() + best_i + k, solucao->route.begin() + best_i+(2*k));
        }

        solucao->costSolution += bestDelta;
        return true;
    }
    return false;
}

void LocalSearch(Solucao *solucao, Data *data){
    std::vector<int> methods = {0, 1, 2, 3, 4};
    bool improved = false;
    
    while(!methods.empty()){
        int n = rand() % methods.size();
        switch(methods[n]){
            case 0:
                improved = bestImprovementSwap(solucao, data);
                break;
            case 1:
                improved = bestImprovementReinsert(solucao, data);
                break;
            case 2:
                improved = bestImprovement2Opt(solucao, data);
                break;
            case 3:
                improved = bestImprovementOrOpt(solucao, data, 2);
                break;
            case 4:
                improved = bestImprovementOrOpt(solucao, data, 3);
                break;
        }

        if(improved){
            methods = {0, 1, 2, 3, 4};
        }else{
            methods.erase(methods.begin() + n);
        }
    }
}