#include "separation.h"
#include <vector>
#include <limits>
#include <set>
#include <cmath>
#include <cstdlib>
#include <ctime>

vector<int> findInitialSubset(int n, set<int>& excluded){
    vector<int> S0;
    int random_vertex;
    do {
        random_vertex = rand() % n; // Gera um número aleatório entre 0 e n-1
    } while (excluded.find(random_vertex) != excluded.end()); // Garante que o vértice não foi usado antes
    S0.push_back(random_vertex);
    return S0;
}

vector <vector<int> > MaxBack(double** x, int n){
    vector<vector<int>> result; // Armazena todos os subconjuntos best_S encontrados
    set<int> excluded;          // Conjunto de vértices já escolhidos inicialmente

    while (excluded.size() < n) {
        vector<int> S0 = findInitialSubset(n, excluded);
        set<int> S(S0.begin(), S0.end());

        set<int> complement;
        for(int i = 0; i < n; ++i){
            if(S.find(i) == S.end()){ // i não está em S
                complement.insert(i); // adiciona i ao complemento
            }
        }

        vector<double> maxback_val(n, 0.0);

        for(int i: complement){
            for(int j: S){
                if(j > i)
                    maxback_val[i] += x[i][j];
                else
                    maxback_val[i] += x[j][i];
            }
        }

        double cut_val = 0.0;
        double mincut_val;
        set<int> best_S = S;

        // calcula valores iniciais 
        for(int i: S){
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
            S.insert(selected_vertex);
            complement.erase(selected_vertex);

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
                best_S = S;
            } 
        }

        if (!best_S.empty() && best_S.size() < n) {
            // Adiciona o melhor subconjunto encontrado ao resultado apenas se ele não for vazio ou completo
            result.push_back(vector<int>(best_S.begin(), best_S.end()));
        }

        excluded.insert(best_S.begin(), best_S.end());

    }
    
    return result;
}
