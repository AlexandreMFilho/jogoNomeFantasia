/*
 ============================================================================
 Name        : TopGear5000.c
 Author      : Alexandre M. M.Filho
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
//Criação 09/10/2022
 */

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

//Prototipos


void keyboard(unsigned char, int, int);
void Desenha();
void Timer(int);
void relatorio();
void inicializa_nave();
void atualiza_nave(float,float);
void Desenha_nave();

struct ponto{
	float x;
	float y;
	float z;
}ponto;


//Nave
//ela é feita de 3 pontos, cujas coordenadas serão atualizadas
//e será feita a movimentação da mesma
struct nave{
	struct ponto pe,ph,pd,pc;//pontoe_squerda/ponto_head/ponto_direita/ponto_centro
	float a,x,b,y,c,m;
}nave;

struct nave jet;
float cordx=0.0,cordy=0.0,borda_esquerda=-100.0,borda_direita=100.0,topo=100.0,chao=-100.0;
float layer_jogo = 1.0,background =0.0;

int main(int argc, char** argv) {

	inicializa_nave();
	atualiza_nave(0.0,0.0);

	//Inicializa a Biblioteca
	glutInit(&argc, argv);

	//Indica o modo da janela e modelo de cor
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	//Tamanho da tela
	glutInitWindowSize (400, 400);
	//Posicao da tela
	glutInitWindowPosition (300, 100);
	//Cria a janela e define seu titulo
	glutCreateWindow ("Asteroids *By Alexandre Filho");

	//Define a funcao display() como funcao que desenha na janela
	//Toda vez que GLUT determinar que a tela via ser redesenhada a
	//a função display deve ser chamada.
	glutDisplayFunc(Desenha);

	glutTimerFunc(33,Timer, 1);
	//Indica que toda vez que uma tecla do teclado for pressionada
	//GLUT deverá chamar a funcao keyboard() para tratar os eventos.

	//Informa o range de amostragem da janela
	glOrtho (borda_esquerda-10, borda_direita+10, chao-10, topo+10, -110 ,110);
	//Cor de preenchimento da janela
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glFlush();

	glutKeyboardFunc(keyboard);


	//inicia o Loop de desenhos com GLUT
	glutMainLoop();

	return EXIT_SUCCESS;
}

void Timer(int value){

	//------------------------------------------ ao enconstar nas bordas
	//O carro vai andar apenas para um lado e para o outro, o cenário que irá
	//se movimentar, assim o movimento do carro é para a esquerda e a direita
	//apenas
	/*if(jet.pc.x >= borda_direita){
		reseta(borda_direita,jet.pc.y);
	}
	else if(jet.pc.x <= borda_esquerda){
		reseta(borda_esquerda,jet.pc.y);
	}*/
	atualiza_nave(cordx,cordy);
	glutPostRedisplay();
	glutTimerFunc(33,Timer, 1);
	relatorio();
	//printf('\033[H');
	//system('\033c');
}

void relatorio(){
	printf("Informações importantes\n");
	printf("cordx:%f\ncordy:%f\n",cordx,cordy);
	printf("centro_nave_:(%2f,%2f,%2f)\n",jet.pc.x,jet.pc.y,jet.pc.z);
	printf("esquerda_nave:(%2f,%2f,%2f)\n",jet.pe.x,jet.pe.y,jet.pe.z);
	printf("direita_nave:(%2f,%2f,%2f)\n",jet.pd.x,jet.pd.y,jet.pd.z);
	printf("\n\n\n");
}

void Desenha(){
	//Seta cada pixel a cor da tela
	glClear(GL_COLOR_BUFFER_BIT);

	//Campo
	glBegin(GL_QUADS);
	glColor3f(0.5,0.5,0.5);
	glVertex3f(borda_esquerda,chao,background);
	glVertex3f(borda_esquerda,topo,background);
	glVertex3f(borda_direita,topo,background);
	glVertex3f(borda_direita,chao,background);
	glFlush();
	glEnd();

	//Orientação
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(borda_esquerda,0.0,0.0);
	glVertex3f(borda_direita,0.0,0.0);

	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,topo,0.0);
	glVertex3f(0.0,chao,0.0);


	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,topo);
	glVertex3f(0.0,0.0,chao);
	glFlush();
	glEnd();

	desenha_nave();

	glutSwapBuffers();
	//glFlush();

}
void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'w' :
		cordy-=-0.1;
		break;
	case 's' :
		cordy-=0.1;
		break;
	case 'a':
		cordx-=0.1;
		break;
	case 'd':
		cordx-=-0.1;
		break;
	}
	Desenha();
}

void inicializa_nave(){
	jet.pc.z=layer_jogo;
	jet.pe.z=background;
	jet.pd.z=background;
	jet.ph.z=background;
	jet.pe.x+=nave.pc.x -3;
	jet.pe.y+=nave.pc.y -3;
	jet.ph.x+=nave.pc.x;
	jet.ph.y+=nave.pc.y +6;
	jet.pd.x+=nave.pc.x +3;
	jet.pd.y+=nave.pc.y -3;
	jet.m = (jet.pc.y - jet.ph.y)/(jet.pc.x - jet.ph.x);/*
	jet.a = jet.m;
	jet.b = 1;
	jet.c = (jet.m*jet.ph.x) + (-1*jet.ph.y);*/

}
void atualiza_nave(float x,float y){
	jet.pc.x+=x;
	jet.pc.y+=y;
	jet.pe.x+=x;
	jet.pe.y+=y;
	jet.ph.x+=x;
	jet.ph.y+=y;
	jet.pd.x+=x;
	jet.pd.y+=y;
	jet.pc.z=layer_jogo;
	jet.pe.z=layer_jogo;
	jet.pd.z=layer_jogo;
	jet.ph.z=layer_jogo;

}
void desenha_nave(){
		glPushMatrix();
		//glRotatef(translado,rx,ry,rz);//glRotatef(translado,0.0,0.0,jet.pe.x);
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_TRIANGLES);
		//glColor3f(0.5,0.5,0.5);
		/*
		glVertex2f(transladar(jet.pe.x,translado),transladar(jet.pe.y,translado));
		glVertex2f(transladar(jet.pc.x,translado),transladar(jet.pc.y,translado));
		glVertex2f(transladar(jet.pd.x,translado),transladar(jet.pd.y,translado));
		 */
		glVertex3f(jet.pe.x,jet.pe.y,jet.pe.z);
		glVertex3f(jet.ph.x,jet.ph.y,jet.ph.z);
		glVertex3f(jet.pd.x,jet.pd.y,jet.pd.z);
		glPopMatrix();
		glFlush();
		glEnd();

		glPointSize(3.0);
		glBegin(GL_POINTS);
		glColor3f(0.0,0.0,0.0);
		glVertex3f(jet.pc.x,jet.pc.y,jet.pc.z);
		glFlush();
		glEnd();
}
