#!/bin/bash

make

# Definir as instâncias que você quer rodar com a extensão .tsp
instances=("dantzig42.tsp" "swiss42.tsp" "att48.tsp" "gr48.tsp" "hk48.tsp" "eil51.tsp" "berlin52.tsp" "brazil58.tsp" 
           "st70.tsp" "eil76.tsp" "pr76.tsp" "gr96.tsp" "rat99.tsp" "kroA100.tsp" "kroB100.tsp" 
           "kroC100.tsp" "kroD100.tsp" "kroE100.tsp" "rd100.tsp" "eil101.tsp" "lin105.tsp" "pr107.tsp")

# Contagem de instâncias
instances_count=${#instances[@]}
echo "instance_name;mean_cost;mean_time" $'\n' > ./results.txt

# Inicializa o contador de instâncias
i=1

# Definir cor verde para o terminal
GREEN="\033[0;32m"
NC="\033[0m" # Resetar a cor

# Loop para rodar apenas as instâncias selecionadas
for instance in "${instances[@]}"; do
    # Exibe a instância em verde e o resto da linha com a cor padrão
    echo -e "${GREEN}running instance $instance${NC}"
    echo "instance $i of $instances_count"

    # Executa o programa para a instância
    ./mlp "../instances/$instance" >> ./results.txt

    # Incrementa o contador
    i=$((i + 1))
done

echo "done"
