0#include <GL/glut.h>


void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

int polygon = 3;

int size_i, size_j;



void init(void)
{
  glOrtho (-1, size_i+1, -1, size_j+1, -1 ,1);
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
	    glVertex3f(-0.5, -0.5, 0);
	    glColor3f(0, 1, 0);
	    glVertex3f(0.5, -0.5, 0);
	    glColor3f(0, 0, 1);
	    glVertex3f(0.5, 0.5, 0);
	    glColor3f(0, 0, 1);
	    glVertex3f(-0.5, 0.5, 0);
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
	}
display();
}


int main(int argc, char** argv)
{

	size_i = 10;
	size_j = 10;




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
