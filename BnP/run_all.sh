#!/bin/bash

make

# Definir a cor verde para o terminal
GREEN='\033[0;32m'
NC='\033[0m' # Sem cor

# Contar o número total de instâncias
total_instances=$(find instances -type f | wc -l)
echo "Total de instâncias: ${total_instances}"

# Zerar o arquivo benchmark.txt e adicionar cabeçalho
echo "instance,size,bins,time" > ./benchmark.txt

i=1

for instance in instances/*; do
    echo "Processando ${GREEN}${instance}${NC}"
    echo "Instance $i of $total_instances"
    ./bpp "${instance}"
    
    i=$(($i + 1))
done

echo "-" >> ./benchmark.txt

