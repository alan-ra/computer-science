from collections import deque

def BFS(G, v):
    """busca em largura no grafo G, retornando todos os vértices visitados a
    partir de v e a distância a cada um deles"""

    distance = [0] * len(G) #lista da distância do do vértice v a todos os outros de G
    visited = [False] * len(G) #lista de vértices visitados

    queue = deque([v]) #fila do BFS
    visited[v] = True

    #enquanto tiver vértice na fila a ser visitado
    while queue:
        u = queue.popleft() #cabeça da fila (vértice de partida)

        #visita vizinhos do vértice
        for w in G[u]:
            if not visited[w]: #se não foi visitado ainda
                visited[w] = True #marca como visitado
                distance[w] = distance[u] + 1 #adiciona 1 na distância acumulada
                queue.append(w) #coloca vértice na fila a visitar

    return distance

#lista de adjacência do grafo
G = [
[1, 2],
[0, 3],
[0, 3],
[1, 2, 4],
[3]
]

#imprimindo resultado da busca em largura iniciando pelo vértice 0
print(BFS(G, 0))
