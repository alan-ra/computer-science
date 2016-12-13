// Alan Rodrigues de Almeida - 00080215
// Claudemir de Souza Freitas Junior - 00082601
// João Francisco Lopes Soares - 00071879
// Luis Gustavo Madoglio - 000

//Funções auxiliares------------------------------------------------------------

// Diferença entre dois conjuntos
function setDifference(A, B) {
  return A.filter(function (element) {
    return B.indexOf(element) == -1;
  });
}

// Checa se w é sentença
function isSentence(w, N) {
  return !N.some(function (element) {
    return w.includes(element);
  });
}

// Retorna primeiro símbolo não terminal da cadeia
function findNonTerminal(w, N) {
  var n = N.find(function (element) {
    return w.includes(element);
  });

  return n === undefined? false: n;
}

// escolhe uma regra em P que contenha o símbolo não terminal n de forma
// aleatória
function randomRule(n, P) {
  var newP = P.filter(function (element) {
    return element["a"].includes(n);
  });

  var randomIndex = Math.floor(Math.random() * newP.length);
  return newP[randomIndex];
}

// Exercício 1------------------------------------------------------------------
function trivialGrammar(V, T, P, S) {
  var w = S;
  var N = setDifference(V, T);
  while (!isSentence(w, N)) {
    var n = findNonTerminal(w, N);
    var r = randomRule(n, P);
    w = w.replace(r["a"], r["b"]);
  }

  return w;
}

// Exercício 2------------------------------------------------------------------
function derivation(i, G, w) {
  if (i == 0)
    return w;

  var wLength = w.length;
  var oldW = w.slice(0, wLength);
  w.splice(0, wLength);
  var N = setDifference(G["V"], G["T"]);

  for (var j = 0; j < wLength; j++)
    for (var k = 0; k < G["P"].length; k++) {
      var newW = oldW[j].replace(G["P"][k]["a"], G["P"][k]["b"]);
      if (newW !== oldW[j] || isSentence(newW,N) && w.indexOf(newW) == -1)
        w.push(newW);
    }

  return derivation(i-1, G, w);
}

// Exercício 3------------------------------------------------------------------
function shortenGrammar(G) {

  var N = setDifference(G["V"], G["T"]);
  var gLength = G["P"].length;
  var counter = 1;

  // Itera em todas as regras
  for (var x =0; x < gLength; x++) {
    var r = G["P"][x];

    // Considerando gramática definida corretamente, |b| > 2 é não-unitário
    if (r["b"].length > 2) {

      // Encontra o único não-terminal de b na regra
      var n = findNonTerminal(r["b"], N);
      // Salva sua posição
      var nPos = r["b"].indexOf(n);
      var rule = r["b"].substring(1, r["b"].length - 1).split('');

      // Se posição do não-terminal no início: Gramática Linear à Esquerda
      if (nPos === 0) {
        rule = rule.reverse();
        r["b"] = '(' + n + counter + ')' + r["b"][r["b"].length - 1];

        // Cria as outras regras em P
        for (var i in rule) {
          var ruleNo;
          if (i == rule.length - 1)
            ruleNo = n;
          else
            ruleNo = '(' + n + (counter + 1) + ')';
          G["P"].push({a: '(' +  n + counter++ + ')', b: ruleNo + rule[i]});
        }

      // Senão: Gramática Linear à Direita
      } else {
        r["b"] = r["b"][0] + '(' + n + counter + ')';
        for (var i in rule) {
          var ruleNo;
          if (i == rule.length - 1)
            ruleNo = n;
          else
            ruleNo = '(' + n + (counter + 1) + ')';
          var newRule = {a: '(' + n + counter++ + ')', b: rule[i] + ruleNo};
          G["P"].push(newRule);
        }
      }
    }
  }
}

// Exercício 4------------------------------------------------------------------
function isRegularGrammar(G) {
  var N = setDifference(G["V"], G["T"]);
  P = G["P"];
  var nPos;

  // Verifica todas as regras da gramática
  for (var i = 0; i < P.length; i++) {

    // Se a não pertence a N, não é regular
    if (P[i]["a"].length > 1 || findNonTerminal(P[i]["a"], N) == false)
      return false;

    // Encontra o único não-terminal possível de b na regra
    var n = findNonTerminal(P[i]["b"], N);

    if (n !== false) {
      // Salva sua posição se primeira iteração (define linear esquerda ou direita)
      if (!nPos) {
        if (P[i]["b"].length > 1) {
          nPos = P[i]["b"].indexOf(n);
          // Símbolo não-terminal no meio da cadeia
          if (nPos !== 0 && nPos !== P[i]["b"].length - 1)
            return false;
        }
      // Próximas iterações, checa se continua no início ou fim
      } else {
        if (nPos === 0 && findNonTerminal(P[i]["b"][0], N) === false)
          return false;
        else if (nPos !== 0 && findNonTerminal(P[i]["b"][P[i]["b"].length - 1], N) === false)
          return false;
      }

      // b tem mais de 1 símbolo, tem não-terminal no início e o restante terminal
      if (P[i]["b"].length > 1 && nPos === 0 && !isSentence(P[i]["b"].substring(1, P[i]["b"].length), N))
        return false;
    }

  }

  return true;
}

// Chamada das funções----------------------------------------------------------

// Gramáticas de teste
var g1 = {
  V: ['0','1','2','3','S','A'],
  T: ['0','1','2','3'],
  P: [
    {a: 'S', b: 'S2'},
    {a: 'S', b: 'S3'},
    {a: 'S', b: 'A'},
    {a: 'A', b: '1'},
    {a: 'A', b: '0'}
  ],
  S: 'S'
};

var g2 = {
  V: ['a','b','c','d','e','f','g','P','Q','S'],
  T: ['a','b','c','d','e','f','g'],
  P: [
    {a: 'S', b: 'abcdP'},
    {a: 'P', b: 'efP'},
    {a: 'P', b: 'Q'},
    {a: 'Q', b: 'g'}
  ],
  S: 'S'
};

var g3 = {
  V: ['a','b','c','d','e','f','g','P','Q','S'],
  T: ['a','b','c','d','e','f','g'],
  P: [
    {a: 'S', b: 'Pabcd'},
    {a: 'P', b: 'Pef'},
    {a: 'P', b: 'Q'},
    {a: 'Q', b: 'g'}
  ],
  S: 'S'
};

var g4 = {
  V: ['a','b','c','d','e','f','g','P','Q','S'],
  T: ['a','b','c','d','e','f','g'],
  P: [
    {a: 'S', b: 'Pabcd'},
    {a: 'P', b: 'Pef'},
    {a: 'P', b: 'Q'},
    {a: 'Q', b: 'g'}
  ],
  S: 'S'
};

// Teste exercício 1 -----------------------------------------------------------
console.log("Exercício 1 ----------------------------------------------------");
console.log("w = " + trivialGrammar(g1.V, g1.T, g1.P, g1.S));

// Teste exercício 2 -----------------------------------------------------------
var i = 4;
var w = [g1["S"]];
w = derivation (i, g1, w);

// Verificação de sentenças
var N = setDifference(g1["V"], g1["T"]);
w = w.filter(function (element) {
  return isSentence(element, N);
});

console.log("\nExercício 2 ----------------------------------------------------");
console.log("S =>^" + i + " w\nW = {" + w.toString() + "}");

// Teste exercício 3 -----------------------------------------------------------
shortenGrammar(g2);

console.log("\nExercício 3 ----------------------------------------------------");
console.log("\nGramática Unitária Linear à Direita\n");
for (var i in g2["P"]) {
  console.log(g2["P"][i]["a"] + ' -> ' + g2["P"][i]["b"]);
}

shortenGrammar(g3);
console.log("\nGramática Unitária Linear à Esquerda\n");
for (var i in g3["P"]) {
  console.log(g3["P"][i]["a"] + ' -> ' + g3["P"][i]["b"]);
}

// Teste exercício 4 -----------------------------------------------------------
console.log("\nExercício 4 ----------------------------------------------------");
console.log("G4 = ");
console.log(g4);
isRegularGrammar(g4)? console.log("G4 é regular"): console.log("G4 não é regular");
console.log("\n");
