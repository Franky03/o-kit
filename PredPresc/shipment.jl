using Pkg
if !haskey(Pkg.dependencies(), "JuMP")
    Pkg.add("JuMP")
end

if !haskey(Pkg.dependencies(), "GLPK")
    Pkg.add("GLPK")
end

using JuMP
using GLPK
using DelimitedFiles

function read_instance(filename)
    open(filename, "r") do file
        dz = parse(Int, readline(file))
        dy = parse(Int, readline(file))
        
        p1 = parse(Int, readline(file))
        p2 = parse(Int, readline(file))
        
        # Pular linhas em branco antes da matriz de custos
        linha = readline(file) |> strip
        while isempty(linha)
            linha = readline(file) |> strip
        end

        # Lê a matriz c como uma matriz de inteiros (Matrix{Int})
        c = Array{Int,2}(undef, dz, dy)
        c[1, :] = parse.(Int, split(linha))
        for i in 2:dz
            linha = readline(file) |> strip
            c[i, :] = parse.(Int, split(linha))
        end

        # Pular linhas em branco antes do vetor de demanda
        linha = readline(file) |> strip
        while isempty(linha)
            linha = readline(file) |> strip
        end
        
        # Lê o vetor de demanda y
        y = parse.(Int, split(linha))
        
        return dz, dy, p1, p2, c, y
    end
end

dz, dy, p1, p2, c, y = read_instance("kstc_dmnd.ship")

# Criação do modelo de otimização
model = Model(GLPK.Optimizer)

# Variáveis
@variable(model, s[1:dz, 1:dy] >= 0) # quantidade de produto transportada do armazém i para o local j
@variable(model, t[1:dz] >= 0)       # produção de última hora em cada armazém i
@variable(model, z[1:dz] >= 0)       # quantidade produzida e armazenada em cada armazém i

# Função objetivo
@objective(model, Min, 
    p1 * sum(z[i] for i in 1:dz) +
    p2 * sum(t[i] for i in 1:dz) +
    sum(c[i, j] * s[i, j] for i in 1:dz, j in 1:dy)
)

# Restrições

# Satisfazer a demanda em cada local j
for j in 1:dy
    @constraint(model, sum(s[i, j] for i in 1:dz) >= y[j])
end

# Limite de envio para cada armazém i (não pode exceder o estoque + produção de última hora)
for i in 1:dz
    @constraint(model, sum(s[i, j] for j in 1:dy) <= z[i] + t[i] )
end

# Resolver o modelo
optimize!(model)

# Imprimir os resultados
if termination_status(model) == MOI.OPTIMAL
    println("Custo total: ", objective_value(model))
    for i in 1:dz
        println("Armazém ", i, ":")
        println("  Produção de última hora: ", value(t[i]))
        println("  Produção antecipada: ", value(z[i]))
        for j in 1:dy
            println("  Transporte para o local ", j, ": ", value(s[i, j]))
        end
    end
else
    println("Nenhuma solução ótima encontrada")
end
