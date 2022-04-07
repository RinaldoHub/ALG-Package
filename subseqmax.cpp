/*
 * subseqmax.cpp
 *
 *  Created on: 15 de dez de 2017
 *      Author: ramide
 */

/*
 * subsetsum.cpp
 *
 *  Created on: 14 de dez de 2017
 *      Author: ramide
 */

#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "util.h"

using namespace std;

/* ---------------- PRÁTICA 11 ----------------------------- */

/* funções força bruta -----------------*/

int subseqMaxBF(int * array, int len, int & ini, int & end, long & count) {
	int max = -9999, sum;
	ini = end = -1;
	for (int i = 0; i < len; i++) {
		sum = 0;
		for (int j = i; j < len; j++) {
			count++;

			sum += array[j];
			if (sum > max) {
				max = sum;
				ini = i;
				end = j;
			}
		}

	}
	return max;
}

/* funções Dividir e Conquistar ----------------- */

int subseqMaxMiddle(int * array, int start, int middle, int finish, int & ini, int & end, long & count) {
	int maxL = 0, maxR = 0;
	int sumL = 0, sumR = 0;

	//int iniPrev, endPrev;

	ini = end = middle;

	for (int i = middle - 1; i >= start; i--) {
		count ++;
		sumL += array[i];
		if (sumL > maxL) {
			maxL = sumL;
			ini = i;
		}
	}

	for (int i = middle + 1; i <= finish; i++) {
		count ++;
		sumR += array[i];
		if (sumR > maxR) {
			maxR = sumR;
			end = i;
		}
	}

	//if(endPrev - iniPrev >= end - ini) ini = iniPrev, end = endPrev;

	return maxL + maxR + array[middle];
}

// TODO Prática 11
// array:	o array sendo processado
// start:	posição a ser considerada início do array
// finish:	posição a ser considerada final do array
// ini:		(Resultado) deve conter ao final a posição de início da subsequência máxima
// end: 	(Resultado)	deve contar ao final a posição de final da subsequência máxima
// RETORNO:	soma da subsequência máxima
int __subseqMaxDC(int * array, int start, int finish, int & ini, int & end, long & count) {
	if (start > finish) return -9999; // caso de retorno

	// Calcule a posição central (middle)
	//	O meio está entre start e finish

	// Chame a função recursivamente para as duas metades:
	// 	Lado esquerdo (start até middle -1) e Lado direito (middle + 1 até finish)
	//  Para cada metade, devem ser retornados a soma da subseqmax, os os inicios (ini) e finais (end)
	//     Salve esses valores em variáveis temporárias adequadas para cada metade

	// Use subseqMaxMiddle() para achar a sequência com soma máxima que passa no meio do array (middle)
	// 	Lembre de salvar o retorno (soma máxima) e limites da sequência (ini e end) em variáveis próprias

	// Identifique qual dos três cenários (esquerda, direita, meio) gerou a maior soma:
	// 	Esse valor será o retornado pelo função e vai determinar
	// 	quais limites serão salvo nos parâmetros ini e end da chamada atual

	int middle = (start + finish) / 2; //variavel 'middle' que indica a metade do array
	int esq, dir, meio; //variaveis que receberao o retorno de cada funcao recursiva

	//chamada recursiva de cada parte do array e sua atribuicao a respectiva variavel
	esq = __subseqMaxDC(array, start, middle-1, ini, end, count);
	dir = __subseqMaxDC(array, middle+1, finish, ini, end, count);
	meio = subseqMaxMiddle(array, start, middle, finish, ini, end, count);

	//teste de condicao para retornar que parte do array possui a maior sequencia
	if(esq > dir && esq > meio) return esq;
	if(meio > esq && meio > dir) return meio;
	if(dir > esq && dir > meio) return dir;
}

int subseqMaxDC(int * array, int len, int & ini, int & end, long & count) {
	return __subseqMaxDC(array, 0, len - 1, ini, end, count);
}

/* ---------------- PRÁTICA 12 ----------------------------- */

/* funções versão Rercusiva ----------------- */

int seqMax(int * array, int pos, int & ini, long & count) {
	if (pos < 0) return 0;
	int sum, iniPrev;

	count ++;
	int x = array[pos];
	int sumPrev = x + seqMax(array, pos - 1, iniPrev, count);

	if ( x >= sumPrev) {		// começa nova sequencia
		sum = array[pos];
		ini = pos;
	} else {				// continua sequencia anterior
		sum = sumPrev;
		ini = iniPrev;
	}

	return sum;
}

int subseqMaxRec(int * array, int len, int & ini, int & end, long & count) {
	int sum = -9999;
	ini = end = -1;

	for (int i = 0; i < len; i++) {
		int _ini, _sum;
		_sum = seqMax(array, i, _ini, count);

		if (_sum > sum) {
			sum = _sum;
			ini = _ini;
			end = i;
		}
	}

	return sum;
}

/* funções Memoization (Top-Down) ----------------- */

int seqMaxMemo(int * array, int pos, int & ini, int * SUM, int * INI, long & count) {
	// TODO Prática 12

}

int subseqMaxMemo(int * array, int len, int & ini, int & end, long & count) {
	// TODO Prática 12

    return 0;
}

/* funções Programação Dinâmica (Bottom-Up)---------- */

int subseqMaxPD(int * array, int len, int & ini, int & end, long & count) {
	// TODO Prática 12

    int sum = 0, totSum = -1;
    ini = end = 0;
    int iniPrev = 0, endPrev = 0;

	for(int i = 0; i < len; i++)
	{
	    int flag = array[i];
		sum = sum + array[i];

		if(sum <= 0){
            iniPrev = i + 1;
			sum = 0;
        }
		if(sum > totSum && sum > 0){
            endPrev = i;
			totSum = sum;
		}

		if(endPrev - iniPrev >= end - ini) ini = iniPrev, end = endPrev;
	}

    return totSum;
}

/* -------------------------------------- */

void runSubseqMax(const char * name, int func(int *, int, int&, int&, long &), int * array, int len, int printSeq) {
	long count = 0;

	int ini = 0, end = 0;

	auto start = std::chrono::high_resolution_clock::now();

	int max = func(array, len, ini, end, count);

	auto finish = std::chrono::high_resolution_clock::now();
	long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() / 1000;

	cout << name << ": " << max << " ";

	if (printSeq) { cout << "[ "; print(array, ini, end); cout << "] "; }

	cout << "[time: "<< elapsed << "us] ";
	cout << "[count: "<< count << "]" << endl;

}

int main() {

	int printSeq = 1;
	int size = 20;

	srand(12345);

	int * array = new int[size];

	for (int i = 0; i < 10; i++) {

		for (int i = 0; i < size; i++) {
			array[i] = (rand() % (2*size)) - size;
		}
		cout << i << ": " << endl; print(array, size);

		runSubseqMax("Bruteforce     ", subseqMaxBF, array, size, printSeq);
		runSubseqMax("Divide&Conquer ", subseqMaxDC, array, size, printSeq);
		runSubseqMax("Recursive      ", subseqMaxRec, array, size, printSeq);
		runSubseqMax("Memoization    ", subseqMaxMemo, array, size, printSeq);
		runSubseqMax("Dynamic Prog.  ", subseqMaxPD, array, size, printSeq);
	}

	delete [] array;

	return 0;
}
