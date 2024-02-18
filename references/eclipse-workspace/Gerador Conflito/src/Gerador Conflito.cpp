//============================================================================
// Name        : Gerador_Conflitos2.cpp
// Author      : Alexandre M. M.Filho
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>
#include<GL/glut.h>

 int cont =0;

void num_vertices(int* n){
	*n=rand()%4;
}

float asymptotic_decider(float* v,float a){
  float q1,q2,r,alfa;
  alfa = (a-v[0])/(v[1]-v[0]);
  q1 = (v[0]+(alfa*(v[1]-v[0])));
  q2 = (v[3]+(alfa*(v[2]-v[3])));
  r = (q1+(alfa*(q2-q1)));
  return r;
}
float marching_square(float* v){
  return ((v[0]+v[1]+v[2]+v[3])/4);
}

void analisa(float * vertices,float alfa,FILE * fp){
  //Parte que testa os vertices no marching square e no asymptotic decider:
     float ad, ms;
     int rad, rms;
     ad = asymptotic_decider(vertices,alfa);
     ms = marching_square(vertices);
     if (ad >= alfa){
       rad = 1;
     }else{
       rad = 0;
     }
     if (ms >= alfa){
       rms = 1;
     }else{
       rms = 0;
     }
     if(rad!=rms){

       //Se houver divergencia de resultados plota no arquivo
       for(int k=0;k<4;k++){
         fprintf(fp,"%f\t",vertices[k]);
         printf("%f\t",vertices[k]);
        }
        //coloca os valores dados por ambos algoritmos
       fprintf(fp,"/AD:%f/%d\tMS:%f/%d\n",ad,rad,ms,rms);
       printf("/AD:%f/%d\tMS:%f/%d\n",ad,rad,ms,rms);
       cont++;
      }
}

void preencher(FILE * fp,char * nome){
	char analise[1024];//nome arquivo .an
	strcpy(analise,strcat(strtok(nome,"."),".an"));
	printf("\nNovo_arquivo: %s\n",analise);

	FILE* novo_arq = fopen(analise,"w");// Ponteiro file pra escrever no arquivo .an

	int line,colu,i,j,alfa;
	char linha[1024], *pch = NULL;
	//rewind(fp);
	printf("\nEntrei no preencher..\nAquisição dados matriz..\n");
	//Pega dimensão da matriz
	fgets(linha,1024,fp);
	pch = strtok(linha," x\n");
	colu = atoi(pch);
	pch = strtok(NULL," x\n");
	line = atoi(pch);

	fgets(linha,1024,fp);
	alfa = atof(linha);

	printf("dimensão %d %d\n",line,colu);
	printf("Valor alpha : %f .\n",alfa);


	float arr[line][colu];

	//ini.grid = min;

	float **ar = (float**)malloc((line+1) * sizeof(*arr));
	for (int lin = 0; lin < colu+1; lin++) {
		ar[lin] = (float*)malloc((colu+1) * sizeof(*arr[lin]));
	}

	//Pega dados e preenche na matriz temporária
	for(i = 0,fgets(linha,1024,fp);i<line,!feof(fp);fgets(linha,1024,fp),i++) {
		for(j = 0,pch = strtok(linha," \n#");j<colu,pch!=NULL;pch = strtok(NULL," \n#"),j++){
			ar[j][i] = atof(pch);

			printf("%f ",ar[j][i]);

			//criaPontoG(&ini,i,j,am[i][j]);
		}
		printf("\n");
	}

	float vertices[4] = {0.0,0.0,0.0,0.0};
	for(i = 0; i<line;i++){
		for(j =0; j<colu;j++){
			vertices[0]= ar[i][j];
			vertices[1]= ar[i][j+1];
			vertices[2]= ar[i+1][j+1];
			vertices[3]= ar[i+1][j];
			if((vertices[0] > alfa and vertices[2] > alfa and vertices[1] < alfa and vertices[3] < alfa)
			or (vertices[1] > alfa and vertices[3] > alfa and vertices[0] < alfa and vertices[2] < alfa)){
			analisa(vertices,alfa,novo_arq);
			}
		}

	}
	fprintf(novo_arq,"%d conflitos.\n",cont);
	printf("%d conflitos.\n",cont);

	fclose(novo_arq);
	//free(ar);
	//free(am);
}


int main(int argc, char** argv){


  FILE *fp;
  char nome[80];
  char linha[1024], *pch = NULL;
  float alfa = 0.0;

	  strcpy(nome,"/home/alexandre/input/Conclusao/");
  if(argc == 2){
	  strcat(nome, argv[1]);
  }
  else{
  printf("Digite o nome da matriz fonte a ser lida:\n");
  scanf("%s",nome);
  }
  fp = fopen(nome,"r");
  if(fp == NULL){
	printf("Arquivo inexistente.\n");
    return 1;
  }
  printf("\nArquivo encontrado e aberto..");
  preencher(fp,nome);
  printf("fechando arquivo.txt %d\n",fclose(fp));

  //printf("\nTerminei o preencher..");

	//fclose(fp);
	return EXIT_SUCCESS;

}
