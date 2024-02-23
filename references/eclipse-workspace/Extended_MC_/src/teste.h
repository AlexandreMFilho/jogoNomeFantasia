/*
 * teste.h
 *
 *  Created on: 19 de abr de 2022
 *      Author: alexandre
 */

#ifndef TESTE_H
#define TESTE_H

#include<stdio.h>
//#include"Octree_.h"

FILE* arquivo = nullptr;
char nome[80];
int size_x,size_y,size_z;
float dado_vol,isovalue;


FILE* abre_arquivo();
void fecha_arquivo(FILE*);
int pot_2(int);


int pot_2(int val){
	int aux = 1;
	if(val == 2){
		aux = 1;
	}else{
		aux = aux + pot_2(val/2);
	}
	return aux;
}


FILE* abre_arquivo(){
	FILE* fp;
	printf("Digite o nome da matriz fonte a ser lida:\n");
	scanf("%s",nome);
	fp = fopen(nome,"r");
	if(fp == NULL){
		printf("Arquivo inexistente.\n");
		return nullptr;
	}
	printf("Arquivo encontrado e aberto..\n");
	return fp;
}

void fecha_arquivo(FILE* file){
	fclose(file);
}


#endif // TESTE_H
