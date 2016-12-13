def remove_inaccessible_useless(G):
    """Remove símbolos inacessíveis e inúteis e atualiza as regras de produção
       em G."""

    # Bloco do algoritmo de símbolos inúteis -----------------------------------
    N0 = set([0])
    N1 = set() # Novo conjunto de não-terminais
    while N0 != N1:
        N0 = N1
        # Gera novo conjunto de símbolos não-terminais úteis
        N1 = N0 | {Y[0] for Y in G['P'] if set(Y[1]) <= N0 | G['T']}
    else:
        # Atualiza os não-terminais e as regras de produção em G
        G['N'] = N1
        G['P'] = [P for P in G['P'] if P[0] in G['N'] and set(P[1]) <= G['N'] | G['T']]

    # Bloco do algoritmo de símbolos inacessíveis ------------------------------
    V0 = set()
    V1 = set([G['S']])
    while V0 != V1:
        V0 = V1
        # Gera novo vocabulário com símbolos acessíveis
        V1 = V0 | {Xi for X in G['P'] if X[0] in V0 for Xi in X[1]}
    else:
        # Atualiza os não-terminais, terminais e regras de produção em G
        G['N'] &= V1
        G['T'] &= V1
        G['P'] = [P for P in G['P'] if P[0] in G['N'] and set(P[1]) <= G['N'] | G['T']]

def remove_null_production(G):
    """Remove produções em vazio e atualiza atualiza as regras de produção."""

    E0 = set();
    # Não-terminais que possuem transição em vazio
    E1 = {A[0] for A in G['P'] if A[1] == ''}
    while E0 != E1:
        E0 = E1
        # Não-terminais que geram transição em vazio indiretamente
        E1 = E0 | {A[0] for A in G['P'] if set(A[1]) <= E0}
    else:
        GP0 = set()
        # Conjunto de regras de produção que não transitam em vazio
        GP1 = {P for P in G['P'] if P[1] != ''}
        while GP0 != GP1:
            GP0 = GP1
            # Gera regras de produção a partir da combinação simples de todos os
            # símbolos de E1 em todas as regras de GP1
            GP1 = GP0 | {tuple([A[0],A[1][0:i] + A[1][i+1:]]) for A in GP0
                         for i in range(len(A[1])) if A[1][i] in E1
                         and A[1][0:i] + A[1][i+1:] != ''}
        # Atualiza regras de produção
        GP = list(GP1)
        GP.sort()
        G['P'] = GP
        # Adiciona transição em vazio na raiz, caso a gramática gere cadeia vazia
        if G['S'] in E1:
            G['P'].append(tuple(['S','']))

def remove_unit_production(G):
    """Remove produções unitárias do tipo A -> B em G."""

    NA = {}
    # Gera NA de todos os não-terminais
    for N in G['N']:
        N0 = set()
        N1 = set([N])
        while N0 != N1:
            N0 = N1
            # Conjunto com lado direito das regras A -> B
            N1 = N0 | {P[1] for P in G['P'] if P[0] in N0 and P[1] in G['N']}
        else:
            NA[N] = N1
    # Regras de produção não unitárias
    GP = [P for P in G['P'] if not P[1] in G['N'] or P[1] == '']

    # Gera novas regras de produção não-unitárias
    for A, B in NA.items():
        # Regras não-unitárias dos símbolos que tinham produções unitárias
        newP = [P for P in G['P'] if P[0] in B and P[1] not in G['N']]
        # Adiciona as regras diretamente para A
        # ex: onde A -> B e B -> a, adiciona A -> a
        for P in newP:
            P1 = tuple([A,P[1]])
            if P1 not in GP:
                GP.append(P1)
    G['P'] = GP # Atualiza regras de produção em G

# Funções auxiliares -----------------------------------------------------------

def print_grammar(G):
    """Imprime a gramática de maneira legível."""

    print('T = %s,\nN = %s,\nP = {' % (G['T'], G['N']))
    i = 0
    for P in G['P']:
        i += 1
        if i == len(G['P']):
            print('   %s -> %s\n},' % (P[0],P[1]))
        else:
            print('   %s -> %s,' % (P[0],P[1]))
    print(G['S'])

# Gramáticas de teste ----------------------------------------------------------

G1 = {
    'T': {'b','c','d','e'},
    'N': {'S','B','C','D','E'},
    'P': [
        ('S','Bb'),
        ('S','Cc'),
        ('S','Ee'),
        ('B','b'),
        ('B','Bb'),
        ('C','Cc'),
        ('C','c'),
        ('D','Bd'),
        ('D','Cd'),
        ('D','d'),
        ('E','Ee'),
    ],
    'S': 'S'
}

G2 = {
    'T': {'a','b'},
    'N': {'S','A','B','C'},
    'P': [
        ('S','ABC'),
        ('A','BB'),
        ('A',''),
        ('B','CC'),
        ('B','a'),
        ('C','AA'),
        ('C','b')
    ],
    'S': 'S'
}

G3 = {
    'T': {'a','b','c'},
    'N': {'S','A','B','C'},
    'P': [
        ('S','A'),
        ('S','B'),
        ('S','C'),
        ('A','aaAa'),
        ('A','B'),
        ('A',''),
        ('B','bBb'),
        ('B','b'),
        ('B','C'),
        ('C','cC'),
        ('C','')
    ],
    'S': 'S'
}

G4 = {
    'T': {'a','b'},
    'N': {'S','X'},
    'P': [
        ('S','aXa'),
        ('S','bXb'),
        ('X','a'),
        ('X','b'),
        ('X','S'),
        ('X','')
    ],
    'S': 'S'
}

G5 = {
    'T': {'a','b'},
    'N': {'S','X','Y','Z','M','N'},
    'P': [
        ('S','XY'),
        ('X','a'),
        ('Y','Z'),
        ('Y','b'),
        ('Z','M'),
        ('M','N'),
        ('N','a')
    ],
    'S': 'S'
}

G6 = {
    'T': {'a','b','c','d'},
    'N': {'S','B','C'},
    'P': [
        ('S','aBC'),
        ('B','bB'),
        ('B',''),
        ('C','cCc'),
        ('C','d'),
        ('C',''),
    ],
    'S': 'S'
}

# Chamadas das funções e impressão dos resultados ------------------------------

#remove_inaccessible_useless(G1)
remove_null_production(G6)
# remove_unit_production(G5)
# remove_inaccessible_useless(G5)
print_grammar(G6)
