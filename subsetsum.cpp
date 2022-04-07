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

/* funções força bruta -----------------*/

void reset(int * array, int len) {
	for (int i = 0; i < len; i++) array[i] = 0;
}

int sum(const int * array, const int * subset, int len) {
	int sum = 0;
	for (int i = 0; i < len; i++)
		if (subset[i])
			sum += array[i];
	return sum;
}

int finished(const int * subset, int len) {
	for (int i = 0; i < len; i++) if(!subset[i]) return 0;
	return 1;
}

void next(int * subset, int len) {
	for (int i = 0; i < len; i++) {
		if (subset[i]) {
			subset[i] = 0;
		} else {
			subset[i] = 1;
			break;
		}
	}
}

int subsetSumBF(int * array, int len, int value, int * subset, long & count) {
	int tmp = 0;
	reset(subset, len);
	while(!((tmp = sum(array, subset, len)) == value) && !finished(subset, len)) {
		next(subset, len);
		count++;
	}
	return tmp == value;
}

/* ---------------- PRÁTICA 11 ----------------------------- */

/* funções Backtracking V1 ----------------- */

int __subseqSumBT(int * array, int len, int value, int * subset, int pos, int sum, long & count) {
	// Sucesso!
	if (sum == value) return 1;

	if (pos >= len) return 0;
	count++;

	// Adicionando número na posição atual ao conjunto
	subset[pos] = 1;
	if (__subseqSumBT(array, len, value, subset, pos + 1, sum + array[pos], count)) return 1;

	// Desfazendo (Backtracking) porque não deu certo e tentando de novo
	subset[pos] = 0;
	if (__subseqSumBT(array, len, value, subset, pos + 1, sum, count)) return 1;

	return 0;
}

int subsetSumBT(int * array, int len, int value, int * subset, long & count) {
	return __subseqSumBT(array, len, value, subset, 0, 0, count);
}

/* funções Backtracking V2 ----------------- */

// TODO Prática 11

int __subsetSumBTv2(int * array, int len, int value, int * subset, int pos, int sum, long & count) {
	// Sucesso!

	if (sum == value) return 1;

	if (pos >= len) return 0;

	count++;

    //Ordenação decrescente do array utilizando INSERTION SORT
    /*
    for(int i = 1; i < len; i ++){
        int tmp = array[i];
        int j = i - 1;

        while(array[j] < tmp && j >= 0){
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = tmp;
	}*/

    //Prunning
    if(sum > value) return 0;

	// Adicionando número na posição atual ao conjunto
	subset[pos] = 1;
	if (__subsetSumBTv2(array, len, value, subset, pos + 1, sum + array[pos], count)) return 1;

	// Desfazendo (Backtracking) porque não deu certo e tentando de novo
	subset[pos] = 0;
	if (__subsetSumBTv2(array, len, value, subset, pos + 1, sum, count)) return 1;

	return 0;
}

int subsetSumBTv2(int * array, int len, int value, int * subset, long & count) {

	// TODO Prática 11

	//Tratando um "pior caso"
	int totSum = 0; //variavel de Soma Total inicializada em 0
	for(int i = 0; i < len; i++) totSum += array[i]; //iteração que faz totSum somar todos itens do array
    if (value > totSum) return 0;

	return __subsetSumBTv2(array, len, value, subset, 0, 0, count);
}

/* ---------------- PRÁTICA 12 ----------------------------- */

int subsetSumGreedy(int * array, int len, int value, int * subset, long & count) {
	// TODO Prática 12

	int tmp = 0; //variavel temporaria que receberá o numero a ser diminuído em 'value'
	int j; //variavel que vai guardar a posicao do numero escolhido no subarray

	//tratando um loop infinito (pior caso)
	int totSum = 0; //variavel de Soma Total inicializada em 0
	for(int i = 0; i < len; i++) totSum += array[i]; //iteração que faz totSum somar todos itens do array
    if (value > totSum) return 0;

	do{

        for(int i = 0; i < len; i++){
            count += 1; //incrementar variavel 'count'

            if(array[i] <= value && array[i] > tmp && subset[i] == 0){
                tmp = array[i];
                j = i;
            }
            if(tmp == value) break; //se ja chegou no resultado, nao precisa ir mais além
        }

        subset[j] = 1;
        value -= tmp;
        tmp = 0;

        if(value == 0) return 1;

	} while(value > 0);

	return 0;
}


/* -------------------------------------- */

void runSubsetSum(const char * name,	int func(int *, int, int, int *, long &), int * array, int len, int value) {
	long count = 0;
	int * subset = new int[len];
	reset(subset, len);

	auto start = std::chrono::high_resolution_clock::now();

	int found = func(array, len, value, subset, count);

	auto finish = std::chrono::high_resolution_clock::now();
	long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() / 1000;

	cout << name << ": " << (found?"YES ":" NO ");
	if (found) {
		cout << "[ ";
		for (int i = 0; i < len; i++) {
			if (subset[i])
				cout << array[i] << " ";
		}
		cout << "] ";
	}
	cout << "[time: "<< elapsed << "us] ";
	cout << "[count: "<< count << "]" << endl;


	delete [] subset;
}

int main_SUBSETSUM() {
//int main() {

	int size = 20;

	int * array = new int[size];

	srand(333);

	for (int i = 0; i < size; i++) {
		cout << (array[i] = rand() % (size * 2)) << " ";
	}
	cout << endl;

	int value = 0;
	for (int i = 0; i < 20; i++) {
		 value += (rand() % (size * 2));
		cout << i << ": Trying " << value << " ..." << endl;

		runSubsetSum("Bruteforce     ", subsetSumBF, array, size, value);
		runSubsetSum("Backtracking   ", subsetSumBT, array, size, value);
		runSubsetSum("Backtracking V2", subsetSumBTv2, array, size, value);
		runSubsetSum("Greedy         ", subsetSumGreedy, array, size, value);
	}

	delete [] array;

	return 0;
}
