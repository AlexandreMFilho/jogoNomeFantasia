/*
 ============================================================================
 Name        : Asteroids.c
 Author      : Alexandre M. M.Filho
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================

 //Nave anda e ao chegar nas bordas aparecer na outra. 03/09/2022
 */

#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nvqm.h"

//Prototipos


void keyboard(unsigned char, int, int);
void inicializa_nave();
void atualiza_nave(float,float);
void Desenha();
void Timer(int);
void reseta(float,float);//float,float,float,float);
float transladar(float,float);
void desenha_reta();
void relatorio();

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

//void atira(struct nave);

//TODO Atualizar pensamento, apenas desenhar a nave, e seus pontos em uma matrix, ao andar, mexer na matriz, principalmente a rotação.
// mat4
//                                                    | a e i m |
// [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p] = | b f j n |
//                                                    | c g k o |
//                                                    | d h l p |

//float mat4_det           (mat4 *a);



// mat3
//                               | a d g |
// [a, b, c, d, e, f, g, h, i] = | b e h |
//                               | c f i |
//float mat3_det        (mat3 *a);


struct nave tri;
struct ponto bola;


int aux = 0,atira=0;
float cordx=0.0,cordy=0.0,borda_esquerda=-100.0,borda_direita=100.0,topo=100.0,chao=-100.0,translado = 0.0,rx=0.0,ry=0.0,rz=0.0;
float layer_jogo = 1.0,background =0.0;
mat3 mat_reta;
int main(int argc, char** argv) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	inicializa_nave();
	atualiza_nave(0.0,0.0);
	//mat3 mat_ret = {tri.pc.x,tri.ph.x,0,tri.pc.y,tri.ph.y,0,tri.pc.z,tri.ph.z,0};
	//mat_reta = mat_ret;
	//mat4 mat_nave = {tri.pc.x,tri.pc.y,tri.pc.z,0,tri.pe.x,tri.pe.y,tri.pe.z,0,tri.ph.x,tri.ph.y,tri.ph.z,0,tri.pd.x,tri.pd.y,tri.pd.z,0};
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

	glutTimerFunc(100,Timer, 1);
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
	if(tri.pc.x >= borda_direita){
		reseta(borda_esquerda+1,tri.pc.y);
	}
	else if(tri.pc.x <= borda_esquerda){
		reseta(borda_direita-1,tri.pc.y);
	}
	if(tri.pc.y >= topo){
		reseta(tri.pc.x,chao+1);
	}
	else if(tri.pc.y < chao){
		reseta(tri.pc.x,topo-1);
	}

	atualiza_nave(cordx,cordy);
	//atualiza_ponto(cordx,cordy);
	glutPostRedisplay();
	glutTimerFunc(33,Timer, 1);
	relatorio();//printf('\033[H');
	//system('\033c');
}
void relatorio(){
	printf("cordx:%f\ncordy:%f\n",cordx,cordy);
	printf("centro_nave_:(%2f,%2f,%2f)\n",tri.pc.x,tri.pc.y,tri.pc.z);
	printf("esquerda_nave:(%2f,%2f,%2f)\n",tri.pe.x,tri.pe.y,tri.pe.z);
	printf("direita_nave:(%2f,%2f,%2f)\n",tri.pd.x,tri.pd.y,tri.pd.z);
	printf("rx:%f\nry:%f\nrz:%f\ntranslado:%f\n",rx,ry,rz,translado);
	printf("m:%f\ta:%f\tb:%f\tc:%f\n",tri.m,tri.a,tri.b,tri.c);
	printf("\n\n\n");
}

void inicializa_nave(){
	tri.pc.z=layer_jogo;
	tri.pe.z=layer_jogo;
	tri.pd.z=layer_jogo;
	tri.ph.z=layer_jogo;
	tri.pe.x+=nave.pc.x -3;
	tri.pe.y+=nave.pc.y -3;
	tri.ph.x+=nave.pc.x;
	tri.ph.y+=nave.pc.y +6;
	tri.pd.x+=nave.pc.x +3;
	tri.pd.y+=nave.pc.y -3;
	tri.m = (tri.pc.y - tri.ph.y)/(tri.pc.x - tri.ph.x);
	tri.a = tri.m;
	tri.b = 1;
	tri.c = (tri.m*tri.ph.x) + (-1*tri.ph.y);
	/*
	tri.pc.z=layer_jogo;
	tri.pe.z=layer_jogo;
	tri.pd.z=layer_jogo;
	tri.ph.z=layer_jogo;
	tri.pe.x+=nave.pc.x -3;
	tri.pe.y+=nave.pc.y -3;
	tri.ph.x+=nave.pc.x;
	tri.ph.y+=nave.pc.y +6;
	tri.pd.x+=nave.pc.x +3;
	tri.pd.y+=nave.pc.y -3;
	tri.m = (tri.pc.y - tri.ph.y)/(tri.pc.x - tri.ph.x);
	tri.a = tri.m;
	tri.b = 1;
	tri.c = (tri.m*tri.ph.x) + (-1*tri.ph.y);
*/

}
/*
void inicializa_bola(){
	bola.x = 10.0;
	bola.y = 10.0;
}
 */
void reseta(float x0, float y0){//float x1, float y1,float x2, float y2){
	tri.pc.x=x0;
	tri.pc.y=y0;
	tri.pe.x=x0;
	tri.pe.y=y0;
	tri.ph.x=x0;
	tri.ph.y=y0;
	tri.pd.x=x0;
	tri.pd.y=y0;
	tri.pc.z=layer_jogo;
	tri.pe.z=layer_jogo;
	tri.pd.z=layer_jogo;
	tri.ph.z=layer_jogo;
	/*tri.ph.x=x1;
	tri.ph.y=x1;
	tri.pd.x=y2;
	tri.pd.y=y2;*/
	inicializa_nave();
}

void atualiza_nave(float x,float y){
	tri.pc.x+=x;
	tri.pc.y+=y;
	tri.pe.x+=x;
	tri.pe.y+=y;
	tri.ph.x+=x;
	tri.ph.y+=y;
	tri.pd.x+=x;
	tri.pd.y+=y;
	tri.pc.z=layer_jogo;
	tri.pe.z=layer_jogo;
	tri.pd.z=layer_jogo;
	tri.ph.z=layer_jogo;
	tri.m = (tri.pc.y - tri.ph.y)/(tri.pc.x - tri.ph.x);
	tri.a = tri.m;
	tri.b = 1;
	tri.c = (tri.m*tri.ph.x) + (-1*tri.ph.y);

}
/*
void atualiza_ponto(float x, float y){
	bola.x+=x;
	bola.y+=y;
}
 *//*
void atira(struct nave gatilho){


	glBegin(GL_POINTS);
	glColor3f(1.0,0.0,0.0);
	for(float i = gatilho.ph.x;i<120;i++){
		glVertex2f((gatilho.a*i)+(gatilho.b*i)+gatilho.c,(gatilho.a*i)+(gatilho.b*i)+gatilho.c);
		glEnd();
		glutSwapBuffers();
	}*/
void Desenha_nave(){
		glPushMatrix();
		glRotatef(translado,rx,ry,rz);//glRotatef(translado,0.0,0.0,tri.pe.x);
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_TRIANGLES);
		//glColor3f(0.5,0.5,0.5);

		glVertex2f(transladar(tri.pe.x,0),transladar(tri.pe.y,0));
		glVertex2f(transladar(tri.pc.x,0),transladar(tri.pc.y,0));
		glVertex2f(transladar(tri.pd.x,0),transladar(tri.pd.y,0));

		//glVertex3f(tri.pe.x,tri.pe.y,tri.pe.z);
		//glVertex3f(tri.ph.x,tri.ph.y,tri.ph.z);
		//glVertex3f(tri.pd.x,tri.pd.y,tri.pd.z);
		glPopMatrix();
		glFlush();
		glEnd();

		glPointSize(3.0);
		glBegin(GL_POINTS);
		glColor3f(0.0,0.0,0.0);
		glVertex3f(tri.pc.x,tri.pc.y,tri.pc.z);
		glFlush();
		glEnd();
}

void desenha_reta(){
	for(float i=tri.pc.x;i<borda_direita;i+=0.5){
		glPointSize(5.0);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(tri.pc.x,tri.pc.y,tri.pc.z);
		//glVertex3f(tri.pc.x+i,(((tri.a*borda_esquerda)+tri.c)/tri.m)+i,tri.pc.z);
		glVertex3f(tri.pc.x+cordx,topo+cordy,tri.pc.z);

		//glVertex3f(mat_reta[0],mat_reta[3],mat_reta[6]);
		//glVertex3f(mat_reta[1],mat_reta[4],mat_reta[7]);
	}
		glFlush();
		glEnd();
}
void testa_rotacao(){
	//mat3x2_rotate(mat_reta,cordx);
	printf("");

}
float transladar(float x,float tx){
	return x = x + tx;
}
/*
desenhaCircle(tri.pc.x, tri.pc.y, 0.5, 10);
void desenhaCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < num_segments; i++)
    {
        float theta = 2.0 * 3.1415926f * (float)i / (float)num_segments;//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}*/
void Desenha(){
	//Seta cada pixel a cor da tela
	glClear(GL_COLOR_BUFFER_BIT);
/*
	glBegin(GL_QUADS);
	glColor3f(0.9,0.9,0.9);
	glVertex3f(borda_esquerda,chao,background);
	glVertex3f(borda_esquerda,topo,background);
	glVertex3f(borda_direita,topo,background);
	glVertex3f(borda_direita,chao,background);
	glFlush();
	glEnd();

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
	//glVertex3f(tri.pc.x,0.0,0.0);
	//glVertex3f(tri.pc.x,0.0,0.0);
	glVertex3f(borda_direita,0.0,0.0);

	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,topo,0.0);
	//glVertex3f(0.0,tri.pc.x,0.0);
	//glVertex3f(0.0,tri.pc.x,0.0);
	glVertex3f(0.0,chao,0.0);


	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,topo);
	//glVertex3f(0.0,0.0,tri.pc.x);
	//glVertex3f(0.0,0.0,tri.pc.x);
	glVertex3f(0.0,0.0,chao);
	glFlush();
	glEnd();

*/
	//if(aux == 1){
	/*glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2f(bola.x,bola.y);
	glColor3f(10.0,0.0,0.0);
	 *//*
	//if(atira == 1){
		glBegin(GL_POINTS);
		glPointSize(3);
		glColor3f(1.0,0.0,0.0);
		//for(float i = tri.ph.x;i<120;i+=cordx){
			glVertex2f(tri.ph.x+cordx,(tri.m*tri.ph.x+cordx)+tri.ph.y);
			glutSwapBuffers();
			glEnd();
		//}
	//}*/
	/*
	glBegin(GL_TRIANGLES);
	glLoadIdentity();
	glColor3f(0.0,0.0,1.0);
	glVertex2f(tri.pe.x,tri.pe.y);
	glVertex2f(tri.pd.x,tri.pd.y);
	glVertex2f(tri.ph.x,tri.ph.y);
	glFlush();
	glEnd();*/
	//glTranslatef(0.0,0.0,0.0);
	//glRotatef(translado,0.0,0.0,tri.pc.x);
	//glRotatef(translado,0.0,0.0,tri.pd.x);
	//glMatrixMode(GL_MODELVIEW);

	//}
	/*
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(0.0,0.0);
		glFlush();
		glEnd();
	 */


	Desenha_nave();
	desenha_reta();

	testa_rotacao();
	glutSwapBuffers();
	//glFlush();


}
void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'w' :
		cordy-=-0.01;
		break;
	case 's' :
		cordy-=0.01;
		break;
	case 'a':
		cordx-=0.01;
		break;
	case 'd':
		cordx-=-0.01;
		break;
	case 'r':
		reseta(0.0,0.0);//,0.0,0.0,0.0,0.0);
		break;
	case 'j':
		rx-=0.1;
		break;
	case 'k':
		ry-=0.1;
		break;
	case 'l':
		rz-=0.1;
		break;
	case 'u' :
		rx-=-0.1;
		break;
	case 'i':
		ry-=-0.1;
		break;
	case 'o':
		rz-=-0.1;
		break;
	case 'y':
		translado-=-0.1;
		break;
	case 'h':
		translado-=0.1;
		break;

		/*default:
		cordx=0.0;
		cordy=0.0;
		atira =0;
		break;*/
	}
	atualiza_nave(cordx,cordy);
	Desenha();
}
