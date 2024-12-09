#include "separation.h"
#include <vector>
#include <limits>
#include <set>
#include <cmath>

// Função auxiliar para calcular S0 baseado em um critério (exemplo: os primeiros k vértices)
vector<int> findInitialSubset(int n){
    vector<int> S0;
    for(int i = 0; i < n/2; ++i){ // metade dos vértices
        S0.push_back(i);
    }
    return S0;
}

vector <vector<int> > MaxBack(double** x, int n){
    vector<vector<int>> result;

    vector<int> S0 = findInitialSubset(n);
    set<int> S(S0.begin(), S0.end());
    set<int> complement;
    for(int i = 0; i < n; ++i){
        if(S.find(i) == S.end()){ // i não está em S
            complement.insert(i); // adiciona i ao complemento
        }
    }
    vector<double> maxback_val(n, 0.0);
    double cut_val = 0.0;
    double mincut_val = numeric_limits<double>::max();
    set<int> best_S = S;    

    // calcula valores iniciais 
    for(int i: S){
        for(int j: complement){
            cut_val += x[i][j];
        }
    }

    mincut_val = cut_val;

    // main loop
    for(int k = 0; k < n - S0.size(); ++k){
        // encontra o vértice com maior maxback_val
        int selected_vertex = -1;
        double max_val = -1.0;  
        for(int i: complement){
            if(maxback_val[i] > max_val){
                max_val = maxback_val[i];
                selected_vertex = i;
            }
        }

        // atualiza S e complement
        S.insert(selected_vertex);
        complement.erase(selected_vertex);

        // atualiza cut_val
        cut_val = cut_val + 2 - 2*maxback_val[selected_vertex];

        // atualiza maxback_val
        for (int j : complement) { // complement representa V \ S_k
            maxback_val[j] += x[selected_vertex][j]; // selected_vertex é o vértice recém-adicionado (i)
        }

        // atualiza mincut_val
        if(cut_val < mincut_val){
            mincut_val = cut_val;
            best_S = S;
        } 
    }

    
    
    
    return result;
}
