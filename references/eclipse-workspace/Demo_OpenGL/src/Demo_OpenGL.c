#include <GL/glut.h>


void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

int polygon = 5;

int size_i, size_j;



void init(void)
{
  glOrtho (-1, size_i+1, -1, size_j+1, -1 ,1); //Fixo -1 a 1.
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display() {

  // Set every pixel in the frame buffer to the current clear color.
  glClear(GL_COLOR_BUFFER_BIT);

  // Drawing is done by specifying a sequence of vertices.  The way these
  // vertices are connected (or not connected) depends on the argument to
  // glBegin.  GL_POLYGON constructs a filled polygon.

 if(polygon == 3)
  {
  glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex3f(-0.6, -0.75, 0.5);
    glColor3f(0, 1, 0);
    glVertex3f(0.6, -0.75, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0.75, 0);
  glEnd();
  }
  if(polygon == 4)
  {
	  glBegin(GL_POLYGON);
	    glColor3f(1, 0, 0);
	    glVertex2f(-0.5, -0.5);
	    glColor3f(0, 1, 0);
	    glVertex2f(0.5, -0.5);
	    glColor3f(0, 0, 1);
	    glVertex2f(0.5, 0.5);
	    glColor3f(0, 0, 1);
	    glVertex2f(-0.5, 0.5);
	  glEnd();

  }
  //Preenchimento linha*coluna (complexidade [2N])
  if(polygon == 5)
  {
	  int lin, col;
	  lin = 7;
	  col = 5;
	  glBegin(GL_LINES);
	  	  for(int i =0;i<col+1;i++)
	  	  {
		  	  glColor3f(0,0,0);
	  		  glVertex2f(i,0);
	  		  glVertex2f(i,lin);
	  	  }
	  	  for(int i=0;i<lin+1;i++)
	  	  {
	  		glColor3f(0,0,0);
	  		glVertex2f(0,i);
	  		glVertex2f(col,i);
	  	  }
	  glEnd();
  }
  //Preenchiimento em L
  if(polygon == 6)
  {
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
  }if(polygon == 7)
  {
	  glBegin(GL_QUADS);
	  glColor3f(0.4,0.82,0.98);
	  glVertex2f(-1,-1);
	  glVertex2f(-1,22);
	  glVertex2f(22,22);
	  glVertex2f(22,-1);
	  glColor3f(0.57,0.03,0.41);
	  glVertex2f(8,2);
	  glVertex2f(8,6);
	  glVertex2f(16,6);
	  glVertex2f(16,2);
	  glColor3f(0.53,0.30,0.04);
	  glVertex2f(10,2);
	  glVertex2f(10,4);
	  glVertex2f(12,4);
	  glVertex2f(12,2);
	  glEnd();
	  glBegin(GL_TRIANGLES);
	  glColor3f(0.98,0.46,0.04);
	  glVertex2f(8,6);
	  glVertex2f(12,8);
	  glVertex2f(16,6);
	  glEnd();
  }
  // Flush drawing command buffer to make drawing happen as soon as possible.
  glFlush();
}

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
	case 't' : case 'T' :
	polygon = 3;
	break;
	case 'q' : case 'Q' :
	polygon = 4;
	break;
	case 'g' : case 'G' :
	polygon = 5;
	break;
	case 'h' : case 'H' :
	polygon = 6;
	break;
	case 'c' : case 'C' :
	polygon = 7;
	break;
	}
display();
}


int main(int argc, char** argv)
{

	size_i = 20;
	size_j = 20;




 //Inicializa a biblioteca GLUT e negocia uma seção com o gerenciador de janelas.
 //É possível passar argumentos para a função glutInit provenientes da linha de execução, tais como informações sobre a geometria da tela
  glutInit(&argc, argv);

  //Informa à biblioteca GLUT o modo do display a ser utilizado quando a janela gráfica for criada.
  // O flag GLUT_SINGLE força o uso de uma janela com buffer simples, significando que todos os desenhos serão feitos diretamente nesta janela.
  // O flag GLUT_RGB determina que o modelo de cor utilizado será o modelo RGB.
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  //Define o tamanho inicial da janela, 256x256 pixels, e a posição inicial do seu canto superior esquerdo na tela, (x, y)=(100, 100).
  glutInitWindowSize (400, 400);
  glutInitWindowPosition (100, 100);

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

  return 0;

}
