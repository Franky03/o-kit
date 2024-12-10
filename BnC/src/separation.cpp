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
    std::cout << "MaxBack" << std::endl;
    vector<vector<int>> result; // Armazena todos os subconjuntos best_S encontrados
    set<int> excluded;          // Conjunto de vértices já escolhidos inicialmente

    srand(time(nullptr)); // Inicializa o gerador de números aleatórios
    std::cout << "DEBUG 1"<< std::endl;

    while (excluded.size() < n) {

        std::cout << "DEBUG 2"<< std::endl;
        vector<int> S0 = findInitialSubset(n, excluded);
        set<int> S(S0.begin(), S0.end());

        std::cout << "DEBUG 3"<< std::endl;

        set<int> complement;
        for(int i = 0; i < n; ++i){
            if(S.find(i) == S.end()){ // i não está em S
                complement.insert(i); // adiciona i ao complemento
            }
        }

        std::cout << "DEBUG 4"<< std::endl;

        vector<double> maxback_val(n, 0.0);

        for(int i: complement){
            for(int j: S){
                maxback_val[i] += x[i][j];
            }
        }

        double cut_val = 0.0;
        double mincut_val;
        set<int> best_S = S;

        std::cout << "DEBUG 5"<< std::endl;

        // calcula valores iniciais 
        for(int i: S){
            for(int j: complement){
                cut_val += x[i][j];
            }
        }
        mincut_val = cut_val;

        std::cout << "DEBUG 6"<< std::endl;

        // main loop
        for(int k = 0; k < n - S0.size(); ++k){
            std::cout << "DEBUG 7"<< std::endl;
            // encontra o vértice com maior maxback_val
            int selected_vertex = -1;
            double max_val = -1.0;  
            std::cout << excluded.size() << std::endl;
            for(int i: complement){
                if(maxback_val[i] > max_val){
                    max_val = maxback_val[i];
                    selected_vertex = i;
                }
            }

            if(selected_vertex == -1){
                break;
            }
            
            std::cout << "Selected vertex: " << selected_vertex << std::endl;

            // atualiza S e complement
            S.insert(selected_vertex);
            complement.erase(selected_vertex);

            // atualiza cut_val
            cut_val = cut_val + 2 - 2 * maxback_val[selected_vertex];

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
        std::cout << "DEBUG 8"<< std::endl;
        if (!best_S.empty() && best_S.size() < n) {
            // Adiciona o melhor subconjunto encontrado ao resultado apenas se ele não for vazio ou completo
            result.push_back(vector<int>(best_S.begin(), best_S.end()));
        }

        excluded.insert(best_S.begin(), best_S.end());
        std::cout << "DEBUG 9"<< std::endl;
    }
    
    return result;
}
