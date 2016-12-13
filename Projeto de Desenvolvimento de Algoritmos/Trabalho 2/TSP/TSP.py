from itertools import combinations
import math

def dynamic_tsp(G):
    """Resolve TSP no grafo G e retorna uma tupla com distância e vértices
    do percurso."""

    C = {} # Sub-problemas
    sequence = [0] # Ordem do percurso
    size = len(G[0]) # Tamanho do grafo
    C[(frozenset([0]), 0)] = 0 # Distância para C({0},0)

    for s in range(2, size + 1):
        min_index = 0 # Vértice visitado
        min_global = math.inf # Determina vértice visitado
        # Itera na combinação simples de todos os subconjuntos de tamanho s
        for S in combinations(range(size), s):
            if 0 in S: # vértice de partida pertence ao subconjunto S
                C[(frozenset(S), 0)] = math.inf # Distância para C(S,0)
                for j in S:
                    if j != 0:
                        # C(S,j) = distância mínima
                        min_value = min([C[(frozenset(set(S)-set([j])),i)] + G[i][j]
                                        for i in S if i != j])
                        C[(frozenset(S), j)] = min_value
                        # Determina próximo vértice visitado
                        if min_value < min_global:
                            min_global = min_value
                            min_index = j
        sequence.append(min_index) # Adiciona vértice visitado na sequência
    sequence.append(0) # Fecha sequência com vértice de partida
    # Obtém a distância no último registro de C
    j = min([([C[(frozenset(list(range(size))),i)], i]) for i in range(size)])
    # Retorna ordem e distância completa, com retorno ao primeiro vértice
    return tuple([j[0] + G[j[1]][0],sequence])

# Grafos de teste --------------------------------------------------------------

G = [
    [0,5,8],
    [5,0,2],
    [8,2,0]
]

G1 = [
    [0,10,15,20],
    [10,0,35,25],
    [15,35,0,30],
    [20,25,30,0]
]

# Chamada à função -------------------------------------------------------------

print(dynamic_tsp(G1))
