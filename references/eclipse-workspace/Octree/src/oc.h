
#include <iostream>
#include <vector>
#include<math.h>
//#include"oc.cpp"
using namespace std;

//int size_x, size_y, size_z;
//int tamanho_max = fmax(size_x,fmax(size_y,size_z));
//int nivel_max = log(tamanho_max)/(log(2));
//Octree* p_aux;x

// Structure of a point
struct Point {
	int x;
	int y;
	int z;
	Point()
		: x(-1), y(-1), z(-1)
	{
	}

	Point(int a, int b, int c)
		: x(a), y(b), z(c)
	{
	}
};
// Octree class
class Octree
{
	Octree* parent;
	int sinal,nivel,nivel_max;
	Point *V_0, *V_6;
	vector<Octree*> children;
	float cube[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	int cont_pos_vert_=0;
	int cont_zero_vert_=0;
	int cont_neg_vert_=0;
	float isovalue_ = 0;

public:
	Octree(int , int , int , int , int , int ,Octree*,float,int); //Construtor
	//int sinaliza_octree(Octree*);
	//void percorre_octree(Octree*);
	void imprime_octree(Octree*);
};
