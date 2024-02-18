/*
 ============================================================================
 Name        : Gerador_Vertice.c
 Author      : Alexandre M. M.Filho
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//Includes
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>
#include<GL/glut.h>

//#include "../Lib/lib.h"

//---------------------------Estruturas--------------------------------------------------Estruturas

struct pontoGrade{
	int i, j;
	float valor;
	struct pontoGrade* prox;
}pontoGrade;

struct ponto{
	float x;
	float y;
	struct ponto *prox;
}ponto;

struct inicio{
	float **grid;
	struct pontoGrade* gridstart;
	struct ponto* start;
	struct ponto* media;
	int totalp,totalg,nl,nc;
}inicio;

struct RGB{
	float R;
	float G;
	float B;
}RGB;

//-------------------------Protótipos-----------------------------------------------------Protótipos

void cria_arq(FILE *,char *);
void num_vertices(int* n,int* ncol);
int config(int,float*,float,int,int);
void polariza(int,int,float,float **,char*);
void preencher(FILE *,char *, float);
float interpola(float,float,float);
void interpolador(int, float **,float,int, int,FILE*);
float asymptotic_decider(float*,float,int,int);
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void colorir(float);
void colorir2(float);
void imprimep(struct ponto);
void imprimeG(struct pontoGrade);
void imprimeM(float **,int,int);
void freeGeral(struct inicio*);
void criaPonto(struct inicio*,float,float);
void criaPontoG(struct inicio*,float,float,float);
void plotarp(struct ponto, float*, float*);
void plotarG(struct pontoGrade, float*, float*);
//void plotarG2(struct pontoGrade, float*, float*);
void ajustesTela(int,int);
float transladar(float,float);
float escalar(float,float);
void pontilhado(void);
//void gradeado(struct ponto, float*, float*);
void gradeado(void);
void copiamat(float**,float**);
int configm(int,float* ,float,int ,int );
float marching_square(float*,float,int,int);
void criaPontoM(struct inicio*,float,float);
void polarizam(int,int,float,float **,char*);
void interpoladorm(int , float ** ,float ,int , int ,FILE*);
//---------------------------Declarações e Inicializações de Variáveis Globais-------------//Declarações e Inicializações de Variáveis Globais
float cordx=0.0, cordy=0.0,telax,telay,size_x=1.0,size_y=1.0,taxa=0.0,alfa=0.0;
int dl=0, dc=0, i, cont=0;
int tecla = 0, taxaescala;
struct RGB cores;
struct inicio ini;
struct ponto* iso;
struct ponto* wiki;
struct pontoGrade* grid;
char arqvert[80];


//---------------------------Implementações-----------------------------------------------Implementações

//--------------------------------------------Pontilhado
void pontilhado(void){
	int lin, col;
	lin = dl+1;
	col = dc+1;
	glPointSize(10.0);
	glBegin(GL_POINTS);
	plotarG(*ini.gridstart,&cordx,&cordy);
	glEnd();
}
//--------------------------------------------Gradeado
void gradeado(void){
	int lin, col,aux;
	lin = ini.nl;
	col = ini.nc;
	//glPointSize(5);
	glLineWidth(5.0);
	glBegin(GL_LINES);

	for(int x = 0;x<=col;x++){
		for(int y= 0;y<=lin-1;y++){
			cores.R = 0.000;
			cores.G = 0.000;
			cores.B = 0.000;
			glColor3f(cores.R,cores.G,cores.B);
			//colorir2(ini.grid[x][y]);
			//glVertex2f(transladar(j,-1),transladar(i,-1));
			glVertex2f(transladar(escalar(y,taxa),-1),transladar(escalar(x,taxa),-1));

			//colorir2(ini.grid[x][y+1]);
			//glVertex2f(transladar(j+1,-1),transladar(i,-1));
			glVertex2f(transladar(escalar(y+1,taxa),-1),transladar(escalar(x,taxa),-1));

		}
	}
	for(int x = 0;x<=col;x++){
		for(int y= 0;y<=lin-1;y++){
			//colorir2(ini.grid[y][x]);
			//glVertex2f(transladar(j,-1),transladar(i,-1));
			glVertex2f(transladar(escalar(x,taxa),-1),transladar(escalar(y,taxa),-1));
			//colorir2(ini.grid[y+1][x]);
			//glVertex2f(transladar(j+1,-1),transladar(i,-1));
			glVertex2f(transladar(escalar(x,taxa),-1),transladar(escalar(y+1,taxa),-1));

		}
	}


	/*
		  for(int i = 0;i<=lin;i++){
		 			  for(int j= 0;j<=col;j++){
		 		 				  colorir2(ini.grid[i][j]);
		 		 				  //glVertex2f(transladar(i,-1),transladar(j,-1));
		 		 				  	  glVertex2f(transladar(escalar(i,taxa),-1),transladar(escalar(j,taxa),-1));
		 		 				  colorir2(ini.grid[i+1][j]);
		 		 				  //glVertex2f(transladar(i,-1),transladar(j+1,-1));
		 		 				  	  glVertex2f(transladar(escalar(i+1,taxa),-1),transladar(escalar(j,taxa),-1));
		 		 			  }
		 		  }*/

	glScalef(0,taxaescala,0);
	glLineWidth(1.0);
	glEnd();
}/*
void gradeado(struct ponto first, float* px, float *py){
	int lin, col;
	lin = dl+1;
	col = dc+1;
	glPointSize(10);
	glBegin(GL_LINES);
	plotarG2(*ini.gridstart,&cordx,&cordy);
	glEnd();
}*/
//--------------------------------------------Transladar
float transladar(float x,float tx){
	return x = x + tx;
}
//--------------------------------------------Escalar
float escalar(float x,float tx){
	return x = x*tx;
}
//--------------------------------------------AjustesTela
void ajustesTela(int nl,int nc){
	size_x = 2/(nl+0.1);
	size_y = 2/(nc+0.1);
	if(size_x < size_y){
		taxa = size_x;
	}else{
		taxa = size_y;
	}
}
//--------------------------------------------Plotar
void plotarp(struct ponto first, float* px, float *py){
	struct ponto *p;
	p = &first;
	float aux;
	int ix;
	//glPointSize(3);
	glLineWidth(20.0);
	glBegin(GL_LINES);
	//glBegin(GL_POINTS);

	while(!(p == NULL)){
	//while(!(p->prox == NULL)){
		aux = p->x;
		ix = aux;
		aux = aux - ix;
		aux = aux*10;
		ix = aux;
		if(ix == 0){
			colorir(p->y);
		}else{
			colorir(p->x);
		}
		//glColor3f(cores.R,cores.G,cores.B);
		glColor3f(1.0,0.3,0.0);

		glVertex2f(transladar(escalar(p->x,taxa),-1),transladar(escalar(p->y,taxa),-1));
		//glVertex2f(transladar(escalar(p->prox->x,taxa),-1),transladar(escalar(p->prox->y,taxa),-1));
		//printf("%.6f %.6f\n",p->x,p->y);
		p = p->prox;
	}
	glLineWidth(1.0);
	glEnd();
}
//--------------------------------------------PlotarG
void plotarG(struct pontoGrade first, float* px, float *py){
	struct pontoGrade *p;
	p = &first;
	glPointSize(3);
	glBegin(GL_POINTS);

	while(!(p == NULL)){
		//colorir(p->valor);
		colorir2(p->valor);
		glColor3f(cores.R,cores.G,cores.B);
		//glColor3f(1.0,0.0,0.0);
		//glVertex2f(transladar(p->i,-1),transladar(p->j,-1));
		glVertex2f(transladar(escalar(p->j,taxa),-1),transladar(escalar(p->i,taxa),-1));
		//printf("i:%d j:%d val:%f\n",p->i,p->j,p->valor);
		p = p->prox;
	}
	glEnd();
}
/*void plotarG2(struct pontoGrade first, float* px, float *py){
	struct pontoGrade *p;
	p = &first;
	glPointSize(3);
	glBegin(GL_LINES);

	while(!(p == NULL)){
		//colorir(p->valor);
		colorir2(p->valor);
		glColor3f(cores.R,cores.G,cores.B);
		//glColor3f(1.0,0.0,0.0);
		//glVertex2f(transladar(p->i,-1),transladar(p->j,-1));
		glVertex2f(transladar(escalar(p->j,taxa),-1),transladar(escalar(p->i,taxa),-1));
		//printf("i:%d j:%d val:%f\n",p->i,p->j,p->valor);
		p = p->prox;
	}
	glEnd();
}*/
//--------------------------------------------imprimep
void imprimep(struct ponto first){
	struct ponto *p;
	p = &first;
	printf("plotando iso:\n");
	while(!(p == NULL)){
		printf("%.6f %.6f\n",p->x,p->y);
		p = p->prox;
	}
}
//--------------------------------------------imprimeG
void imprimeG(struct pontoGrade first){
	struct pontoGrade *p;
	p = &first;
	printf("plotando grade:\n");
	while(!(p == NULL)){
		printf("i:%d j:%d val:%f\n",p->i,p->j,p->valor);
		p = p->prox;
	}
}
//--------------------------------------------imprimeM
void imprimeM(float **mat,int col,int lin){
	printf("plotando matriz:\n");
	for(int i = 0;i<lin+1;i++){
		for(int j = 0;j<col+1;j++){
			printf("%f ",mat[i][j]);
		}
		printf("\n");
	}
}
//--------------------------------------------FreeGeral
void freeGeral(struct inicio *apagar){
	struct ponto *p , *pprox;
	pprox = apagar->start;
	while(!(pprox == NULL)){
		p = pprox;
		pprox = pprox->prox;
		free(p);
	}
	free(pprox);
	apagar->start = NULL;
	apagar->totalp = 0;
}
//--------------------------------------------FreeGeralG
void freeGeralG(struct inicio *apagar){
	struct pontoGrade *p , *pprox;
	pprox = apagar->gridstart;
	while(!(pprox == NULL)){
		p = pprox;
		pprox = pprox->prox;
		free(p);
	}
	free(pprox);
	apagar->gridstart = NULL;
	apagar->totalg = 0;
}
//--------------------------------------------CriaPonto
void criaPonto(struct inicio* No,float cx, float cy){
	struct ponto *novo;
	novo = (struct ponto*) malloc(sizeof(struct ponto));
	novo->x = cx;
	novo->y = cy;
	novo->prox = No->start;
	No->start = novo;
	No->totalp = No->totalp +1;
	printf("\nPonto criado com sucesso x:%f y:%f \n",novo->x,novo->y);
}
//--------------------------------------------CriaPontoG
void criaPontoG(struct inicio* No,float cx, float cy,float val){
	struct pontoGrade *novo;
	novo = (struct pontoGrade*) malloc(sizeof(struct pontoGrade));
	novo->i = cx;
	novo->j = cy;
	novo->valor = val;
	novo->prox = No->gridstart;
	No->gridstart = novo;
	No->totalg = No->totalg +1;
	//printf("\nPontoG criado com sucesso x:%f y:%f val:%f\n%f %f %f\n",novo->i,novo->j,novo->valor,cx,cy,val);
}
//--------------------------------------------CriaPonto
void criaPontoM(struct inicio* No,float cx, float cy){
	struct ponto *novo;
	novo = (struct ponto*) malloc(sizeof(struct ponto));
	novo->x = cx;
	novo->y = cy;
	novo->prox = No->media;
	No->media = novo;
	No->totalp = No->totalp +1;
	printf("\nPonto criado com sucesso x:%f y:%f \n",novo->x,novo->y);
}
//--------------------------------------------Colorir
void colorir(float cx){
	int inte = cx;
	float frac = cx;
	//printf("1-intex:%i  fracx:%f  cx:%f\n",inte,frac,cx);
	frac = frac - inte;
	frac = frac *10;
	inte = frac;
	//printf("2-intex:%i  fracx:%f\n",inte,frac);
	switch(inte){
	case 0:
		cores.R = 0.007;
		cores.G = 0.070;
		cores.B = 0.650;
		break;
	case 1:
		cores.R = 0.007;
		cores.G = 0.384;
		cores.B = 0.713;
		break;
	case 2:
		cores.R = 0.000;
		cores.G = 0.639;
		cores.B = 0.760;
		break;
	case 3:
		cores.R = 0.070;
		cores.G = 0.670;
		cores.B = 0.019;
		break;
	case 4:
		cores.R = 0.552;
		cores.G = 0.780;
		cores.B = 0.011;
		break;
	case 5:
		cores.R = 1.000;
		cores.G = 1.000;
		cores.B = 0.003;
		break;
	case 6:
		cores.R = 1.000;
		cores.G = 0.772;
		cores.B = 0.007;
		break;
	case 7:
		cores.R = 0.996;
		cores.G = 0.580;
		cores.B = 0.000;
		break;;
	case 8:
		cores.R = 1.000;
		cores.G = 0.396;
		cores.B = 0.003;
		break;
	case 9:
		cores.R = 1.000;
		cores.G = 0.011;
		cores.B = 0.003;
		break;
	}
	glColor3f(cores.R,cores.G,cores.B);
}

void colorir2(float cx){
	if(cx >= alfa){
		cores.R = 1.000;
		cores.G = 0.300;
		cores.B = 0.000;
	}else{
		cores.R = 0.000;
		cores.G = 0.000;
		cores.B = 1.000;
	}

	glColor3f(cores.R,cores.G,cores.B);
}
//--------------------------------------------copiamat
void copiamat(float **original, float **copia){
	int linha, coluna;
	for(linha = 0; linha<ini.nl+1; linha++){
		for(coluna = 0; coluna<ini.nc+1; coluna++){
			copia[linha][coluna] = original[linha][coluna];
		}
	}
}
//--------------------------------------------init
void init(void){
	//glOrtho (-1.1, (telax+0.1)-1,-1.1,(telay+0.1)-1, -1 ,1);
	glOrtho (-1.2,1.2,1.2,-1.2, -1 ,1);
	glLineWidth(1.0);
	//glOrtho (-2.2,2.2,2.2,-2.2, -1 ,1);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glFlush();
}
//--------------------------------------------display
void display(void){

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glFlush();
	//Apenas Pontos
	if(tecla ==1){
		pontilhado();
	}
	//Apenas Grade
	if(tecla == 2){
		pontilhado();
		gradeado();
		//gradeado(*ini.start, &cordy,&cordx);
	}
	//Fundo pontilhado e pontos do arquivo
	if(tecla == 3){
		pontilhado();
		plotarp(*ini.start, &cordy,&cordx);
	}
	//Fundo Gradeado e pontos do arquivo
	if(tecla == 4){
		gradeado();
		plotarp(*ini.start, &cordx,&cordy);
	}
	//Fundo pontilhado e linhas do arquivo
	if(tecla == 5){
		pontilhado();
		plotarp(*ini.start, &cordx,&cordy);
	}
	//fundo gradeado e linhas do arquivo
	if(tecla == 6){
		gradeado();
		pontilhado();
		plotarp(*ini.start, &cordx,&cordy);
	}
	if(tecla == 7){
		gradeado();
		pontilhado();
		plotarp(*ini.media, &cordx,&cordy);
	}
	if(tecla == 8){
		pontilhado();
		plotarp(*ini.media, &cordx,&cordy);
	}

	//Preenchimento em L
	if(tecla == 10){
		int lin, col;
		lin = 7;
		col = 5;
		glBegin(GL_LINES);
		//Linha
		for(int i=0;i<lin;i++)
		{
			//Coluna
			for(int j=0;j<col;j++)
			{
				glColor3f(1,0,0);
				glVertex2f(j,i); //p0 horizontal
				glVertex2f(j+1,i);
				glVertex2f(j,i); //p0 vertical
				glVertex2f(j,i+1);
			}
		}
		//Coloca linhas finalizadoras (complexidade [N*N])

		glVertex2f(col,lin);
		glVertex2f(0,lin);
		glVertex2f(col,lin);
		glVertex2f(col,0);

		glEnd();
	}

	glFlush();
}
//--------------------------------------------keyboard
void keyboard( unsigned char key, int x, int y ){
	switch( key ) {

	case 'q' : case 'Q' :
		tecla = 1;
		break;

	case 'w' : case 'W' :
		tecla = 2;
		break;

	case 'a' : case 'A' :
		tecla = 3;
		break;

	case 's' : case 'S' :
		tecla = 4;
		break;

	case 'z' : case 'Z' :
		tecla = 5;
		break;

	case 'x' : case 'X' :
		tecla = 6;
		break;

	case 'c' : case 'C' :
		tecla = 7;
		break;
	case 'v' : case 'V':
		tecla = 8;
	}
	display();
}
//--------------------------------------------Cria Arq
void cria_arq(FILE * fp, char * a){
	fp = fopen(a,"w");
	fflush(fp);
	fclose(fp);
}
//--------------------------------------------Preencher
void preencher(FILE * fp,char * nome,float alfa){

	int line,colu,i,j,z;
	char linha[1024], *pch = NULL;
	//rewind(fp);
	printf("\nEntrei no preencher..\nAquisição dados matriz..\n");
	//Pega dimensão da matriz
	fgets(linha,1024,fp);
	pch = strtok(linha," x\n");
	colu = atoi(pch);
	pch = strtok(NULL," x\n");
	line = atoi(pch);
	pch = strtok(NULL," x\n");
	z = atoi(pch);

	fgets(linha,1024,fp);
	pch = strtok(linha," x\n");
	alfa = atof(pch);

	printf("dimensão %d %d %d\n",line,colu,z) ;
	printf("Valor alpha : %f \n",alfa);

	ini.nl = line;
	ini.nc = colu;
	ini.totalg = (line+1)*(colu+1);


	dl = line;
	dc = colu;

	float arr[line][colu];

	//ini.grid = min;

	float **ar = malloc((line+1) * sizeof(*arr));
	for (int lin = 0; lin < colu+1; lin++) {
		ar[lin] = malloc((colu+1) * sizeof(*arr[lin]));
	}

	float **am = malloc((line+1) * sizeof(*arr));
	for (int lin = 0; lin < colu+1; lin++) {
		am[lin] = malloc((colu+1) * sizeof(*arr[lin]));
	}

	ini.grid = malloc((line+1) * sizeof(*arr));
	for (int lin = 0; lin < colu+1; lin++) {
		ini.grid[lin] = malloc((colu+1) * sizeof(*arr[lin]));
	}

	//Pega dados e preenche na matriz temporária
	for(i = 0,fgets(linha,1024,fp);i<line,!feof(fp);fgets(linha,1024,fp),i++) {
		for(j = 0,pch = strtok(linha," \n#");j<colu,pch!=NULL;pch = strtok(NULL," \n#"),j++){
			ar[i][j] = atof(pch);
			am[i][j] = atof(pch);
			ini.grid[i][j] = ar[i][j];
			printf("dbg %f %d %d\n",ar[i][j],i,j);
			criaPontoG(&ini,i,j,ini.grid[i][j]);
			//criaPontoG(&ini,i,j,am[i][j]);
		}
		//printf("\n");
	}

	imprimeG(*ini.gridstart);
	imprimeM(ini.grid,ini.nl,ini.nc);

	//printf("teste saber se a grade foi feita.");
	//imprimeG(*ini.gridstart);

	// for(int i =0; i<=line;i++){
	//   for(int j = 0; j<=colu;j++){
	//     printf("%f ",ar[i][j]);
	//   }
	//   printf("\n");
	// }

	polariza(line,colu,alfa,ar,nome);
	polarizam(line,colu,alfa,am,nome);
	//free(ar);
	// free(am);
}
//--------------------------------------------num_vertices
void num_vertices(int* n, int* ncol){
	*n=rand()%*ncol+1;
}
//--------------------------------------------polariza
void polariza(int nline,int ncol, float alfa,float **matriz,char* name){
	float aux[4];
	FILE * arq = NULL;


	//Tratamento nome do arquivo de saída, será o "nome do arquivivo de entrada . ms"
	char nomearq[80],*pname;
	pname = strtok(name,".");
	strcpy(nomearq,pname);
	strcat(nomearq,".ms");

	strcpy(arqvert,nomearq);
	//printf("O novo arquivo terá o nome %s\nA variável global copiou %s\n",nomearq,arqvert);

	cria_arq(arq,nomearq);
	arq = fopen(nomearq,"a");

	fprintf(arq,"%dx%d\n",nline,ncol);

	for(int i=0;i<nline;i++){
		//printf("quadrado da linha:%d\n",i);
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
			//printf("\n%f\t%f\t%f\t%f\n",aux[0],aux[1],aux[2],aux[3]);
			//printf("config:\t%d\n",bin_dec);
			interpolador(config(bin_dec,*matriz,alfa,i,j),matriz,alfa,i,j,arq);
		}
		// fprintf(arq,"\n");
	}

}
//--------------------------------------------polarizam
void polarizam(int nline,int ncol, float alfa,float **matriz,char* name){
	float aux[4];
	FILE * arq = NULL;


	//Tratamento nome do arquivo de saída, será o "nome do arquivivo de entrada . ms"
	char nomearq[80],*pname;
	pname = strtok(name,".");
	strcpy(nomearq,pname);
	strcat(nomearq,"_M.ms");

	strcpy(arqvert,nomearq);
	//printf("O novo arquivo terá o nome %s\nA variável global copiou %s\n",nomearq,arqvert);

	cria_arq(arq,nomearq);
	arq = fopen(nomearq,"a");

	fprintf(arq,"%dx%d\n",nline,ncol);

	for(int i=0;i<nline;i++){;
		//printf("quadrado da linha:%d\n",i);
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
			//printf("\n%f\t%f\t%f\t%f\n",aux[0],aux[1],aux[2],aux[3]);
			//printf("config:\t%d\n",bin_dec);
			interpoladorm(configm(bin_dec,*matriz,alfa,i,j),matriz,alfa,i,j,arq);
		}
		// fprintf(arq,"\n");
	}

}
//--------------------------------------------config
int config(int conf,float* matriz,float alfa,int nline,int ncol){
	//int* pont;
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
//--------------------------------------------configm
int configm(int conf,float* matriz,float alfa,int nline,int ncol){
	//int* pont;
	if(conf == 5 || conf == 10){
		float aux = marching_square(matriz,alfa,nline,ncol);
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
//--------------------------------------------interpolador
void interpolador(int config, float ** fonte,float alfa,int linha, int coluna,FILE* final){
	float x = 0.0, y=0.0;

	//Aqui percorre o vetor configuração
	int tabela[][4]={{-1,-1,-1,-1},{3,0,-1,-1},{0,1,-1,-1},{3,1,-1,-1},{1,2,-1,-1},{0,1,2,3},{0,2,-1,-1},{3,2,-1,-1},{2,3,-1,-1},{0,2,-1,-1},{0,3,1,2},{2,1,-1,-1},{1,3,-1,-1},{1,0,-1,-1},{0,3,-1,-1},{5,5,5,5}};
	// printf("Configuração da tabela:\t%d\t%d\t%d\t%d\n",tabela[config][0],tabela[config][1],tabela[config][2],tabela[config][3]);
	//printf("\nChamou interpolador\nlinha:%d,coluna:%d\n",linha,coluna );
	float v0 = fonte[linha][coluna];
	float v1 = fonte[linha+1][coluna];
	float v2 = fonte[linha+1][coluna+1];
	float v3 = fonte[linha][coluna+1];

	// printf("\nv0 fonte[%d][%d]=%f\n",linha,coluna,v0);
	// printf("\nv1 fonte[%d][%d]=%f\n",linha+1,coluna,v1);
	// printf("\nv2 fonte[%d][%d]=%f\n",linha+1,coluna+1,v2);
	// printf("\nv3 fonte[%d][%d]=%f\n",linha,coluna+1,v3);

	for(int i = 0;i<4;i++){
		//printf("\ntabela :%d\n",tabela[config][i]);
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
			criaPonto(&ini,x,y);


		}
	}
	// printf("\n");
	//fprintf(final,"\n");
}

//--------------------------------------------interpolador
void interpoladorm(int config, float ** fonte,float alfa,int linha, int coluna,FILE* final){
	float x = 0.0, y=0.0;

	//Aqui percorre o vetor configuração
	int tabela[][4]={{-1,-1,-1,-1},{3,0,-1,-1},{0,1,-1,-1},{3,1,-1,-1},{1,2,-1,-1},{0,1,2,3},{0,2,-1,-1},{3,2,-1,-1},{2,3,-1,-1},{0,2,-1,-1},{0,3,1,2},{2,1,-1,-1},{1,3,-1,-1},{1,0,-1,-1},{0,3,-1,-1},{5,5,5,5}};
	// printf("Configuração da tabela:\t%d\t%d\t%d\t%d\n",tabela[config][0],tabela[config][1],tabela[config][2],tabela[config][3]);
	//printf("\nChamou interpolador\nlinha:%d,coluna:%d\n",linha,coluna );
	float v0 = fonte[linha][coluna];
	float v1 = fonte[linha+1][coluna];
	float v2 = fonte[linha+1][coluna+1];
	float v3 = fonte[linha][coluna+1];

	// printf("\nv0 fonte[%d][%d]=%f\n",linha,coluna,v0);
	// printf("\nv1 fonte[%d][%d]=%f\n",linha+1,coluna,v1);
	// printf("\nv2 fonte[%d][%d]=%f\n",linha+1,coluna+1,v2);
	// printf("\nv3 fonte[%d][%d]=%f\n",linha,coluna+1,v3);

	for(int i = 0;i<4;i++){
		//printf("\ntabela :%d\n",tabela[config][i]);
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

			criaPontoM(&ini,x,y);

		}
	}
	// printf("\n");
	//fprintf(final,"\n");
}
//--------------------------------------------interpola
float interpola(float v1,float v2,float alfa){
	return (alfa-v1)/(v2-v1);
}
//--------------------------------------------asymptotic_decider
float asymptotic_decider(float* v,float a,int linha,int coluna){
	float q1,q2,r,alfa;
	alfa = (a-v[linha*1+coluna])/(v[(linha+1)*1 + coluna]-v[linha*1+coluna]);
	q1 = (v[linha*1+coluna]+(alfa*(v[(linha+1)*1+coluna]-v[linha*1+coluna])));
	q2 = (v[(linha*1)+(coluna+1)]+(alfa*(v[(linha+1)*1+(coluna+1)]-v[(linha*1)+(coluna+1)])));
	r = (q1+(alfa*(q2-q1)));
	return r;
}
//--------------------------------------------marching_square
float marching_square(float* v,float a,int linha,int coluna){
	return ((v[(linha+1)*1 + coluna]+v[linha*1+coluna]+v[(linha+1)*1+coluna]+v[linha*1+coluna])/4);
}
//--------------------------------------------Main
int main(int argc, char** argv){
	cores.R = 0.0;
	cores.G = 0.0;
	cores.B = 0.0;
	/*
		telax = 1;
		telay = 1;
	 */
	iso = NULL;
	wiki = NULL;
	grid = NULL;
	ini.start = iso;
	ini.media = wiki;
	ini.gridstart = grid;
	ini.totalp = 0;
	ini.totalg = 0;
	ini.nl = 0;
	ini.nc = 0;


	FILE *fp,*fpp;
	char nome[80];
	char linha[1024], *pch = NULL;
	float alfa = 0.0;

	printf("Digite o nome da matriz fonte a ser lida:\n");
	scanf("%s",nome);
	fp = fopen(nome,"r");
	if(fp == NULL){
		printf("Arquivo inexistente.\n");
		return 1;
	}
	printf("\nArquivo encontrado e aberto..");
	//printf("Digite o Valor alfa desejado :\n");
	//scanf("%f",&alfa);
	preencher(fp,nome,alfa);
	printf("fechando arquivo.txt %d\n",fclose(fp));
	//fp = NULL;
	//printf("\nTerminei o preencher..");

	printf("\nAjustando detalhes tela\ndl:%d dc:%d\n..\n",dl,dc);
	ajustesTela(dl,dc);
	telax = dl;
	telay = dc;
	taxaescala = 0;

	imprimep(*ini.start);

	//imprimep(*ini.media);

	imprimeG(*ini.gridstart);

	imprimeM(ini.grid,ini.nl,ini.nc);

	//Inicializa a biblioteca GLUT e negocia uma seção com o gerenciador de janelas.
	//É possível passar argumentos para a função glutInit provenientes da linha de execução, tais como informações sobre a geometria da tela
	glutInit(&argc, argv);

	//Informa à biblioteca GLUT o modo do display a ser utilizado quando a janela gráfica for criada.
	// O flag GLUT_SINGLE força o uso de uma janela com buffer simples, significando que todos os desenhos serão feitos diretamente nesta janela.
	// O flag GLUT_RGB determina que o modelo de cor utilizado será o modelo RGB.
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	//Define o tamanho inicial da janela, 256x256 pixels, e a posição inicial do seu canto superior esquerdo na tela, (x, y)=(100, 100).
	glutInitWindowSize (800,800);
	glutInitWindowPosition (283, 84);

	// Cria uma janela e define seu título
	glutCreateWindow ("Marching Squares");

	//Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
	init();

	// Define display() como a função de desenho (display callback) para a janela corrente.
	// Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
	glutDisplayFunc(display);

	// Indica que sempre que uma tecla for pressionada no teclado, GLUT deverá chama a função keyboard() para tratar eventos de teclado (keyboard callback).
	// A função de teclado deve possuir o seguinte protótipo:
	glutKeyboardFunc(keyboard);

	//Inicia o loop de processamento de desenhos com GLUT.
	// Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
	glutMainLoop();

	//freeGeral(&ini);
	//freeGeralG(&ini);
	//fclose(fp);
	return EXIT_SUCCESS;

}
