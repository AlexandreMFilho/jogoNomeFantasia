//============================================================================
// Name        : Gerador_Volume_txt.cpp
// Author      : Alexandre M. M.Filho
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

int main() {
	char nome[40],aux[20];
	int size_x,size_y,size_z;
	float isovalue;
	FILE *arq = nullptr;
	printf("Nome do arquivo:\n");
	scanf("%s",&aux);
	strcpy(nome,"/home/alexandre/input/Conclusao/");
	strcat(nome,aux);
	//puts(nome);
	printf("%s",nome);
	arq = fopen(nome,"w");
	printf("\nas 3 dimensoes:\n");
	scanf("%d %d %d",&size_x,&size_y,&size_z);
	printf("o isovalor de interesse:\n");
	scanf("%f",&isovalue);
	fprintf(arq,"%d %d %d\n%f\n",size_x,size_y,size_z,isovalue);
	//fprintf(arq,"%d %d %d\n",size_x,size_y,size_z);
	float mat[size_x*size_y*size_z];
	int indice = 0;
	for(int i=0;i<size_z;i++){
		for(int j=0;j<size_y;j++){
			for(int k=0;k<size_x;k++){
				//srand(time(NULL));
				mat[indice] = (rand() % 9999 + 1000)/10000.0;
				/*if(i>4 and j>4 and k>4){// and i<5 and j<5 and k<5){
					//srand(time(NULL));
					mat[indice] = (rand() % 9999 + 1000)/10000.0;
				}else{
					mat[indice] = 0.0;
				}*/
				fprintf(arq,"%f ",mat[indice]);
				printf("%f ",mat[indice]);
			}
		}
	}
	fprintf(arq,"\n#");
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
