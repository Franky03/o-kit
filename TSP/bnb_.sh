#!/bin/bash

make

# Definir as instâncias que você quer rodar com a extensão .tsp
instances=("bayg29.tsp" "bays29.tsp" "burma14.tsp" "fri26.tsp" "gr17.tsp" "gr21.tsp" "gr24.tsp" "ulysses16.tsp" "ulysses22.tsp")

# Contagem de instâncias
instances_count=${#instances[@]}
echo "instance_name;mean_cost;mean_time" > ../BnB/costs.tsp

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
    ./tsp "../instances/$instance" >> ../BnB/costs.tsp

    # Incrementa o contador
    i=$((i + 1))
done

echo "done"