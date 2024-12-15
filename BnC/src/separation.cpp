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

    std::cout << "Subconjuntos: " << std::endl;
    for (vector<int> S : result) {
        std::cout << "{ ";
        for (int i : S) {
            std::cout << i << " ";
        }
        std::cout << "}" << std::endl;
        std::cout << std::endl;
    }

    return result;
}

MaxBackResult MaxBackMod(double** x, int n, vector<int> &S0){

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

    for(int i: S0){
        for(int j: complement){
            if(j > i)
                cut_val += x[i][j];
            else
                cut_val += x[j][i];
        }
    }
    
    mincut_val = cut_val;
    int s = -1, t = -1; // vértices s e t que formam a aresta de corte
    
    // main loop
    for (int k = 0; k < n - 1; ++k) {
      // Encontra o vértice com maior maxback_val
      int selected_vertex = -1;
      double max_val = -1.0;
      for (int i : complement) {
           if (maxback_val[i] > max_val) {
               max_val = maxback_val[i];
               selected_vertex = i;
           }
      }

      if (selected_vertex == -1) break; // Se não houver vértices em complemento, encerra o loop

      // Atualiza S e complement
      S0.push_back(selected_vertex);
      complement.erase(remove(complement.begin(), complement.end(), selected_vertex), complement.end());

      // Atualiza cut_val
      cut_val = cut_val + 2 - 2 * maxback_val[selected_vertex];

      // Atualiza maxback_val
      for (int j : complement) {
           if (j > selected_vertex) {
               maxback_val[j] += x[selected_vertex][j];
          } else {
              maxback_val[j] += x[j][selected_vertex];
           }
       }

       // Atualiza mincut_val
       if (cut_val < mincut_val) {
           mincut_val = cut_val;
           best_S = S0;
           // Atualiza {s, t} com os últimos dois vértices
           // t vai ser o penuultimo vértice adicionado
           t = best_S[best_S.size() - 2];
           s = selected_vertex;
      }
    } 
    // Retorna os resultados finais
    MaxBackResult result;
    result.cut_val = mincut_val;
    result.S = best_S;
    result.s = s;
    result.t = t;
    
    return result;
}

void Shrink(double **x, int& n, int s, int t) {
    if (s > t) std::swap(s, t); 

    for (int i = 0; i < n; i++) {
        if (i != s && i != t) {
            x[s][i] += x[t][i];
            x[i][s] = x[s][i]; 
        }
    }

    for (int i = t; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            x[i][j] = x[i + 1][j];
        }
    }
    for (int j = t; j < n - 1; j++) {
        for (int i = 0; i < n; i++) {
            x[i][j] = x[i][j + 1];
        }
    }
    n--; // Ajustar o tamanho da matriz
}

void addToExcluded(vector<int>& excluded, vector<int>& S){
    for(int i: S){
        if(find(excluded.begin(), excluded.end(), i) == excluded.end()){
            excluded.push_back(i);
        }
    }
}

void printV(vector<int> V){
    for(int i: V){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

vector<vector<int>> MinCut(double** x, int n) {
    vector<vector<int>> minCutSet;
    double minCutValue = numeric_limits<double>::infinity();
    vector<int> excluded;
    vector<int> V;
    int original_n = n;

    double** original_x = new double*[n];
    for (int i = 0; i < n; i++) {
        original_x[i] = new double[n];
        for (int j = 0; j < n; j++) {
            original_x[i][j] = x[i][j];
        }
    }

    while (excluded.size() < original_n) {
        vector<int> S = findInitialSubset(n, excluded);
        V = S;
    
        x = new double*[original_n];
        for (int i = 0; i < original_n; i++) {
            x[i] = new double[original_n];
            for (int j = 0; j < original_n; j++) {
                x[i][j] = original_x[i][j];
            }
        }
        n = original_n;

        while (n > 1) {
            MaxBackResult result = MaxBackMod(x, n, S);
            if (result.s == -1 || result.t == -1) {
                break;
            }
            if (result.cut_val < 2 - EPSILON) {
                minCutValue = result.cut_val;
                V = result.S;
                printV(V);
                addToExcluded(excluded, V);
            }
            Shrink(x, n, result.s, result.t);
        }

        if (!V.empty() && V.size() < n) {
            minCutSet.push_back(V);
        } else {
            minCutSet = {};
        }
    }

    // Libera a memória original_x
    for (int i = 0; i < original_n; i++) {
        delete[] original_x[i];
    }
    delete[] original_x;

    return minCutSet;
}
