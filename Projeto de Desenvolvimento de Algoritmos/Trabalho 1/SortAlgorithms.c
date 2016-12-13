#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define TAM 100000
#define IT 10

void Cronometro(void (*SortFunction)(int*, int, int), int *vet, int ini, int fim) {
	double media, tempoTotal = 0.0, desvioPadrao = 0.0, *tempo;
	int *vetOrd;
	clock_t start, end;

	vetOrd = (int*) malloc(sizeof(int) * (fim + 1));
	tempo = (double*) malloc(sizeof(double) * IT);

	for (int i = 0; i < IT; i++) {
		memcpy(vetOrd, vet, sizeof(int) * (fim + 1));
		start = clock();
		SortFunction(vetOrd, ini, fim);
		end = clock();

		tempo[i] = (double) (end - start) / CLOCKS_PER_SEC;
		printf("Amostra no %d: %.4f\n", i + 1, tempo[i]);
		tempoTotal += tempo[i];
	}

	media = tempoTotal / IT;

	for (int i = 0; i < IT; i++)
		desvioPadrao += pow(tempo[i] - media, 2);
	desvioPadrao = sqrt(desvioPadrao / (IT - 1));


	printf("\nMedia tempo decorrido: %.4f", media);
	printf("\nDesvio Padrao: %.4f\n\n", desvioPadrao);
	free(vetOrd);
	free(tempo);
}

void InsertionSort(int *orig, int ini, int fim) {
	int x, aux, per;

	for(x = ini + 1; x <= fim; x++) {
		aux = orig[x];

		per = x - 1;

		while((aux < orig[per]) && (per >= 0)) {
			orig[per + 1] = orig[per];
			per--;
		}

		orig[per + 1] = aux;
	}
}

void SelectionSort(int *orig, int ini, int fim) {
	int i, j, min, aux;

	for(i = ini; i < fim; i++) {
		min = i;
		for(j = (i + 1); j <= fim; j++) {
			if(orig[j] < orig[min])
				min = j;
		}

		if(i != min) {
			aux = orig[i];
			orig[i] = orig[min];
			orig[min] = aux;
		}
	}
}

void QuickSort (int *orig, int ini, int fim) {
	int i, j, m, aux;
	i = ini;
	j = fim;
	m = orig[(i + j) / 2];
	do {
		while (orig[i] < m) i++;
		while (orig[j] > m) j--;

		if (i <= j) {
			aux = orig[i];
			orig[i] = orig[j];
			orig[j] = aux;
			i++; j--;
		}
	} while (i <= j);

	if (ini < j)
		QuickSort(orig, ini, j);

	if (fim > i)
		QuickSort(orig, i, fim);
}

void BuildHeap(int *orig, int ini, int fim) {
	int aux = orig[ini], j = ini * 2 + 1;

	while(j <= fim) {
		if(j < fim) {
			if(orig[j] < orig[j + 1])
				j++;
		}

		if(aux < orig[j]) {
			orig[ini] = orig[j];
			ini = j;
			j = 2 * ini + 1;
		} else
			j = fim + 1;
	}

	orig[ini] = aux;
}

void HeapSort(int *orig, int ini, int fim) {
	int i, j, aux;

	for(i = (fim / 2); i >= 0; i--) {
		BuildHeap(orig, i, fim);
	}

	for(j = fim; j >= 1; j--) {
		aux = orig[0];
		orig[0] = orig[j];
		orig[j] = aux;
		BuildHeap(orig, 0, j - 1);
	}
}

void MergeSort(int *orig, int ini, int fim) {
	int i, j, k, halfSize, *vTemp;

	if(ini == fim) return;

	//Ordena recursivamente 2 metades
	halfSize = (ini + fim) / 2;
	MergeSort(orig, ini, halfSize);
	MergeSort(orig, halfSize + 1, fim);

	//Intercalação no Vetor Temporário
	i = ini;
	j = halfSize + 1;
	k = 0;
	vTemp = (int *) malloc(sizeof(int) * (fim - ini + 1));

	while((i < halfSize + 1) || (j < fim + 1)) {
		if(i == halfSize + 1) { //i passou da primeira metade
			vTemp[k] = orig[j];
			j++; k++;
		} else {
			if(j == fim + 1) { //j passou da segunda metade
				vTemp[k] = orig[i];
				i++; k++;
			} else {
				if(orig[i] < orig[j]) {
					vTemp[k] = orig[i];
					i++; k++;
				} else {
					vTemp[k] = orig[j];
					j++; k++;
				}
			}
		}
	}

	//Copia vetor temporário p/ vetor original
	for(i = ini; i <= fim; i++)
		orig[i] = vTemp[i - ini];

	free(vTemp);
}

int main() {
	int i, vAle[TAM], vOrd[TAM], vRev[TAM], cont = TAM-1;
	int vOrigAle[TAM], vOrigOrd[TAM], vOrigRev[TAM];

	srand(time(NULL));
	for(i = 0; i < TAM; i++, cont--) {
		//Aleatório
		vOrigAle[i] = rand() % TAM;

		//Ordenado
		vOrigOrd[i] = i;

		//Reverso
		vOrigRev[i] = cont;
	}

	//EXECUÇÃO INSERTION SORT

	//copia vetor original ao vetor a ordenar
	memcpy(vAle, vOrigAle, sizeof(int) * TAM);
	memcpy(vOrd, vOrigOrd, sizeof(int) * TAM);
	memcpy(vRev, vOrigRev, sizeof(int) * TAM);

	//Aleatório
	printf("INSERTION SORT:\n\nVetor Aleatorio\n");
	Cronometro(InsertionSort, vAle, 0, TAM - 1);

	//Ordenado
	printf("Vetor Ordenado\n");
	Cronometro(InsertionSort, vOrd, 0, TAM - 1);

	//Reverso
	printf("Vetor Reverso\n");
	Cronometro(InsertionSort, vRev, 0, TAM - 1);

	//EXECUÇÃO SELECTION SORT

	//reatribui o vetor que ja passou pelo algoritmo de ordenacao a cima
	memcpy(vAle, vOrigAle, sizeof(int) * TAM);
	memcpy(vOrd, vOrigOrd, sizeof(int) * TAM);
	memcpy(vRev, vOrigRev, sizeof(int) * TAM);

	//Aleatório
	printf("---------------\nSELECTION SORT:\n\nVetor Aleatorio\n");
	Cronometro(SelectionSort, vAle, 0, TAM - 1);

	//Ordenado
	printf("Vetor Ordenado\n");
	Cronometro(SelectionSort, vOrd, 0, TAM - 1);

	//Reverso
	printf("Vetor Reverso\n");
	Cronometro(SelectionSort, vRev, 0, TAM - 1);

	//EXECUÇÃO QUICK SORT

	memcpy(vAle, vOrigAle, sizeof(int) * TAM);
	memcpy(vOrd, vOrigOrd, sizeof(int) * TAM);
	memcpy(vRev, vOrigRev, sizeof(int) * TAM);

	//Aleatório
	printf("---------------\nQUICK SORT:\n\nVetor Aleatorio\n");
	Cronometro(QuickSort, vAle, 0, TAM-1);

	//Ordenado
	printf("Vetor Ordenado\n");
	Cronometro(QuickSort, vOrd, 0, TAM-1);

	//Reverso
	printf("Vetor Reverso\n");
	Cronometro(QuickSort, vRev, 0, TAM-1);

	//EXECUÇÃO HEAP SORT

	memcpy(vAle, vOrigAle, sizeof(int) * TAM);
	memcpy(vOrd, vOrigOrd, sizeof(int) * TAM);
	memcpy(vRev, vOrigRev, sizeof(int) * TAM);

	//Aleatório
	printf("---------------\nHEAP SORT:\n\nVetor Aleatorio\n");
	Cronometro(HeapSort, vAle, 0, TAM - 1);

	//Ordenado
	printf("Vetor Ordenado\n");
	Cronometro(HeapSort, vOrd, 0, TAM - 1);

	//Reverso
	printf("Vetor Reverso\n");
	Cronometro(HeapSort, vRev, 0, TAM - 1);

	//EXECUÇÃO MERGE SORT

	memcpy(vAle, vOrigAle, sizeof(int) * TAM);
	memcpy(vOrd, vOrigOrd, sizeof(int) * TAM);
	memcpy(vRev, vOrigRev, sizeof(int) * TAM);

	//Aleatório
	printf("---------------\nMERGE SORT:\n\nVetor Aleatorio\n");
	Cronometro(MergeSort, vAle, 0, TAM-1);

	//Ordenado
	printf("Vetor Ordenado\n");
	Cronometro(MergeSort, vOrd, 0, TAM-1);

	//Reverso
	printf("Vetor Reverso\n");
	Cronometro(MergeSort, vRev, 0, TAM-1);

	/*for(i = 0; i < TAM; i++)
	{
		if(i == 0)
			printf("[%d, ", vAle[i]);
		else if(i == (TAM - 1))
			printf("%d]\n\n", vAle[i]);
		else
			printf("%d, ", vAle[i]);
	}

	for(i = 0; i < TAM; i++)
	{
		if(i == 0)
			printf("[%d, ", vOrd[i]);
		else if(i == (TAM - 1))
			printf("%d]\n\n", vOrd[i]);
		else
			printf("%d, ", vOrd[i]);
	}

	for(i = 0; i < TAM; i++)
	{
		if(i == 0)
			printf("[%d, ", vRev[i]);
		else if(i == (TAM - 1))
			printf("%d]\n\n", vRev[i]);
		else
			printf("%d, ", vRev[i]);
	}*/

	return(0);
}
