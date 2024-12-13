#include "separation.h"
#include <vector>
#include <limits>
#include <set>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>

struct MaxBackResult {
    double cut_val;
    vector<int> S;
    int s;
    int t;
};

vector<int> findInitialSubset(int n, vector<int>& excluded) {
    vector<int> S0;
    int random_vertex;

    do {
        random_vertex = rand() % n; // Gera um número aleatório entre 0 e n-1
    } while (find(excluded.begin(), excluded.end(), random_vertex) != excluded.end()); // Garante que o vértice não foi usado antes

    S0.push_back(random_vertex);
    return S0;
}

vector <vector<int> > MaxBack(double** x, int n){
    vector<vector<int>> result; // Armazena todos os subconjuntos best_S encontrados
    vector<int> excluded;      // Vetor de vértices já escolhidos inicialmente

    while (excluded.size() < n) {
        vector<int> S0 = findInitialSubset(n, excluded);

        vector<int> complement;
        for(int i = 0; i < n; ++i){
            if (find(S0.begin(), S0.end(), i) == S0.end()) { // i não está em S
                complement.push_back(i);                 // adiciona i ao complemento
            }
        }

        vector<double> maxback_val(n, 0.0);

        for(int i: complement){
            for(int j: S0){
                if(j > i)
                    maxback_val[i] += x[i][j];
                else
                    maxback_val[i] += x[j][i];
            }
        }

        double cut_val = 0.0;
        double mincut_val;
        vector<int> best_S = S0;

        // calcula valores iniciais 
        for(int i: S0){
            for(int j: complement){
                if(j > i)
                    cut_val += x[i][j];
                else
                    cut_val += x[j][i];
            }
        }
        mincut_val = cut_val;

        // main loop
        for(int k = 0; k < n - 1; ++k){
            // encontra o vértice com maior maxback_val
            int selected_vertex = -1;
            double max_val = -1.0;  
            for(int i: complement){
                if(maxback_val[i] > max_val){
                    max_val = maxback_val[i];
                    selected_vertex = i;
                }
            }

            if(selected_vertex == -1){
                break;
            }

            // atualiza S e complement
            S0.push_back(selected_vertex);
            complement.erase(remove(complement.begin(), complement.end(), selected_vertex), complement.end());

            // atualiza cut_val
            cut_val = cut_val + 2 - 2 * maxback_val[selected_vertex];

            // atualiza maxback_val
            for (int j : complement) { // complement representa V \ S_k
                if (j > selected_vertex) {
                    maxback_val[j] += x[selected_vertex][j];
                } else {
                    maxback_val[j] += x[j][selected_vertex];
                }
            }

            // atualiza mincut_val
            if(cut_val < mincut_val){
                mincut_val = cut_val;
                best_S = S0;
            } 
        }

        if (!best_S.empty() && best_S.size() < n) {
            // Adiciona o melhor subconjunto encontrado ao resultado apenas se ele não for vazio ou completo
            result.push_back(best_S);
        }

        excluded.insert(excluded.end(), best_S.begin(), best_S.end());

    }
    
    return result;
}

MaxBackResult MaxBackMod(double** x, int n, vector<int>& excluded){
    vector<vector<int>> result; // Armazena todos os subconjuntos best_S encontrados
    vector<int> S0 = findInitialSubset(n, excluded);

    vector<int> complement;
    for(int i = 0; i < n; ++i){
        if (find(S0.begin(), S0.end(), i) == S0.end()) { // i não está em S
            complement.push_back(i);                 // adiciona i ao complemento
        }
    }

    vector<double> maxback_val(n, 0.0);

    for(int i: complement){
        for(int j: S0){
            if(j > i)
                maxback_val[i] += x[i][j];
            else
                maxback_val[i] += x[j][i];
        }
    }

    double cut_val = 0.0;
    double mincut_val;
    vector<int> best_S = S0;
    int s = -1, t = -1;

    // calcula valores iniciais 
    for(int i: S0){
        for(int j: complement){
            if(j > i)
                cut_val += x[i][j];
            else
                cut_val += x[j][i];
        }
    }
    mincut_val = cut_val;

    // main loop
    for(int k = 0; k < n - 1; ++k){
        // encontra o vértice com maior maxback_val
        int selected_vertex = -1;
        double max_val = -1.0;  
        for(int i: complement){
            if(maxback_val[i] > max_val){
                max_val = maxback_val[i];
                selected_vertex = i;
            }
        }

        if(selected_vertex == -1){
            break;
        }

        // atualiza S e complement
        S0.push_back(selected_vertex);
        complement.erase(remove(complement.begin(), complement.end(), selected_vertex), complement.end());

        // atualiza cut_val
        cut_val = cut_val + 2 - 2 * maxback_val[selected_vertex];

        // atualiza maxback_val
        for (int j : complement) { // complement representa V \ S_k
            if (j > selected_vertex) {
                maxback_val[j] += x[selected_vertex][j];
            } else {
                maxback_val[j] += x[j][selected_vertex];
            }
        }

        // atualiza mincut_val
        if(cut_val < mincut_val){
            mincut_val = cut_val;
            best_S = S0;
            s = S0[S0.size() - 2]; // penúltimo elemento
            t = selected_vertex;   // último elemento
        } 
    }

    if (!best_S.empty() && best_S.size() < n) {
        // coloca best_S como vazio
        std::cout << "Best_S VAZIO" << std::endl;
        best_S.clear();
    }
    
    // printa o best_S
    std::cout << "Best_S: ";
    for(int i: best_S){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return {mincut_val, best_S, s, t};
}

void Shrink(double **x, int n, int s, int t){
    for(int i=0; i < n; ++i){
        if (i != s && i != t) {
            x[s][i] += x[t][i];
            x[i][s] = x[s][i];
        }
    }

    for(int i = t; i < n - 1; ++i){
        for(int j = 0; j < n; ++j){
            x[i][j] = x[i + 1][j];
            x[j][i] = x[j][i + 1];
        }
    }
}


vector<vector<int> > MinCut(double** x, int n){

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(x[i][j] <= EPSILON){
                x[i][j] = 0.0;
            }
        }
    }

    vector<vector<int>> minCutSet;
    double minCutValue = numeric_limits<double>::infinity();

    for(int k=0; k < n-1; k++){
        vector<int> excluded = {};
        MaxBackResult maxBackResult = MaxBackMod(x, n, excluded);

        std::cout << "CutValue: " << maxBackResult.cut_val << std::endl;

        if (maxBackResult.cut_val < minCutValue) {
            minCutValue = maxBackResult.cut_val;
            minCutSet.push_back(maxBackResult.S);
        }

        Shrink(x, n, maxBackResult.s, maxBackResult.t);
        n--;
    }
    std::cout << "MinCutValue: " << minCutValue << std::endl;
    return minCutSet;
}


