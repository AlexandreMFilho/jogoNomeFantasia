/*
 ============================================================================
 Name        : Gerador_Vertice.c
 Author      : Alexandre M. M.Filho
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>
//#include "../Lib/lib.h"

void cria_arq(FILE *,char *);
void num_vertices(int* n,int* ncol);
int config(int,float*,float,int,int);
void polariza(int,int,float,float **,char*);
void preencher(FILE *,char *, float);
float interpola(float,float,float);
void interpolador(int, float **,float,int, int,FILE*);
float asymptotic_decider(float*,float,int,int);

int main(){

  FILE *fp;
  char nome[80];
  float alfa = 0.0;

  printf("Digite o nome da matriz fonte a ser lida:\n");
  scanf("%s",nome);
  fp = fopen(nome,"r");
  if(fp == NULL){
	  printf("Arquivo inexistente.\n");
    return 1;
  }
  printf("Valor alfa:");
  scanf("%f",&alfa);
  preencher(fp,nome,alfa);
}

//Funcao que cria o arquivo
void cria_arq(FILE * fp, char * a){
  fp = fopen(a,"w");
  fflush(fp);
  fclose(fp);
}
//Funcao que preenche o arquivo com a matriz
void preencher(FILE * fp,char * nome,float alfa){

  int line,colu,i,j;
  char linha[1024], *pch = NULL;

  //Pega dimensão da matriz
  fgets(linha,1024,fp);
  pch = strtok(linha," x\n");
  colu = atoi(pch);
  pch = strtok(NULL," x\n");
  line = atoi(pch);
  printf("dimensão %d %d\n",line,colu);

  float arr[line][colu];

   float **ar = malloc((line+1) * sizeof(*arr));
      for (int lin = 0; lin < colu+1; lin++) {
            ar[lin] = malloc((colu+1) * sizeof(*arr[lin]));
      }

      //Pega dados e preenche na matriz temporária
      for(i = 0,fgets(linha,1024,fp);i<line, !feof(fp);fgets(linha,1024,fp),i++) {
        for(j = 0,pch = strtok(linha," \n#");j<colu, pch!=NULL;pch = strtok(NULL," \n#"),j++){
          ar[i][j] = atof(pch);
          printf("%f ",ar[i][j]);
        }
        printf("\n");
      }

      // for(int i =0; i<=line;i++){
      //   for(int j = 0; j<=colu;j++){
      //     printf("%f ",ar[i][j]);
      //   }
      //   printf("\n");
      // }

   polariza(line,colu,alfa,ar,nome);
   free(ar);
}

void num_vertices(int* n, int* ncol){
  *n=rand()%*ncol+1;
}
void polariza(int nline,int ncol, float alfa,float **matriz,char* name){
      float aux[4];
      FILE * arq;


      //Tratamento nome do arquivo de saída, será o "nome do arquivivo de entrada . ms"
      char nomearq[80],*pname;
      pname = strtok(name,".");
      strcpy(nomearq,pname);
      strcat(nomearq,".ms");

      cria_arq(arq,nomearq);
      arq = fopen(nomearq,"a");

      fprintf(arq,"%dx%d\n",nline,ncol);

      for(int i=0;i<nline;i++){
                printf("quadrado da linha:%d\n",i);
      		for(int j=0;j<ncol;j++){

                    //teste para ver se a leitura de quadrado a quadrado está funcionando
		    //aux[0]=matriz[i][j];
                    //aux[1]=matriz[i+1][j];
                    //aux[2]=matriz[i+1][j+1];
                    //aux[3]=matriz[i][j+1];

                    int bin_dec=0;
                    if(matriz[i][j] < alfa){
			                  aux[0]=0;
		                }else{
                        aux[0]=1;
                        bin_dec= bin_dec + 1;
                    }
                    if(matriz[i+1][j] < alfa){
                        aux[1]=0;
                    }else{
                        aux[1]=1;
                        bin_dec = bin_dec + pow(2,1);
                    }
		                  if(matriz[i+1][j+1] < alfa){
                        aux[2]=0;
                    }else{
                        aux[2]=1;
                        bin_dec = bin_dec + pow(2,2);
                    }
		                if(matriz[i][j+1] < alfa){
                        aux[3]=0;
                    }else{
                        aux[3]=1;
                        bin_dec = bin_dec + pow(2,3);
                    }
		        printf("\n%f\t%f\t%f\t%f\n",aux[0],aux[1],aux[2],aux[3]);
		        printf("config:\t%d\n",bin_dec);
            interpolador(config(bin_dec,*matriz,alfa,i,j),matriz,alfa,i,j,arq);
		 }
     // fprintf(arq,"\n");
      }

}
int config(int conf,float* matriz,float alfa,int nline,int ncol){
  int* pont;
  if(conf == 5 || conf == 10){
    float aux = asymptotic_decider(matriz,alfa,nline,ncol);
    //se aux > alfa permanece a configuração(Não acontece nada)
    //se aux < alfa, troca de 5 para 10 e de 10 para 5
    if(aux < alfa){
      if(conf/5 == 1){
        conf = 10;
      }else if(conf/5 == 2){
        conf = 5;
      }
    }
  }
	//5 representa que todos as arestas são interceptadas uma vez que 5 é a soma dos números que representam cada aresta
      return conf;
}



void interpolador(int config, float ** fonte,float alfa,int linha, int coluna,FILE* final){
float x = 0.0, y=0.0;

        //Aqui percorre o vetor configuração
      int tabela[][4]={{-1,-1,-1,-1},{3,0,-1,-1},{0,1,-1,-1},{3,1,-1,-1},{1,2,-1,-1},{0,1,2,3},{0,2,-1,-1},{3,2,-1,-1},{2,3,-1,-1},{0,2,-1,-1},{0,3,1,2},{2,1,-1,-1},{1,3,-1,-1},{1,0,-1,-1},{0,3,-1,-1},{5,5,5,5}};
      // printf("Configuração da tabela:\t%d\t%d\t%d\t%d\n",tabela[config][0],tabela[config][1],tabela[config][2],tabela[config][3]);
      printf("\nChamou interpolador\nlinha:%d,coluna:%d\n",linha,coluna );
      float v0 = fonte[linha][coluna];
      float v1 = fonte[linha+1][coluna];
      float v2 = fonte[linha+1][coluna+1];
      float v3 = fonte[linha][coluna+1];

      // printf("\nv0 fonte[%d][%d]=%f\n",linha,coluna,v0);
      // printf("\nv1 fonte[%d][%d]=%f\n",linha+1,coluna,v1);
      // printf("\nv2 fonte[%d][%d]=%f\n",linha+1,coluna+1,v2);
      // printf("\nv3 fonte[%d][%d]=%f\n",linha,coluna+1,v3);

      for(int i = 0;i<4;i++){
              printf("\ntabela :%d\n",tabela[config][i]);
              switch(tabela[config][i]){

              case 0:
              x = 0.0;
              y = interpola(v1,v0,alfa);
              break;

              case 1:
              x = interpola(v1,v2,alfa);
              y = 1.0;
              break;

              case 2:
              x = 1.0;
              y = interpola(v2,v3,alfa);
              break;

              case 3:
              x = interpola(v0,v3,alfa);
              y = 0.0;
              break;

              default:
              x = -100;
              y = -100;
              break;

              }

              if(x > 0 || y > 0){
                x=x+coluna;
                y=y+linha;
                //printf("x=%f y=%f\t",x,y);
                fprintf(final,"%f %f\n",x,y);
              }
      }
      // printf("\n");
    //fprintf(final,"\n");
}

float interpola(float v1,float v2,float alfa){
  return (alfa-v1)/(v2-v1);
}
float asymptotic_decider(float* v,float a,int linha,int coluna){
  float q1,q2,r,alfa;
  alfa = (a-v[linha*1+coluna])/(v[(linha+1)*1 + coluna]-v[linha*1+coluna]);
  q1 = (v[linha*1+coluna]+(alfa*(v[(linha+1)*1+coluna]-v[linha*1+coluna])));
  q2 = (v[(linha*1)+(coluna+1)]+(alfa*(v[(linha+1)*1+(coluna+1)]-v[(linha*1)+(coluna+1)])));
  r = (q1+(alfa*(q2-q1)));
  return r;
}
