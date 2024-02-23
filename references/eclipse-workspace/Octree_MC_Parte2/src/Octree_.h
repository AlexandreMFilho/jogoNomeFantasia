
#ifndef OCTREE_H_
#define OCTREE_H_

#include<iostream>
#include<math.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

float *matriz = nullptr;

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
	//const int nive_max;
	Point *V_0, *V_6;
	Octree* children[8];
	float cube[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	int cont_pos_vert_=0;
	int cont_zero_vert_=0;
	int cont_neg_vert_=0;
	float isovalue_ = 0;

public:
	Octree(int , int , int , int , int , int ,Octree*,float,int,int); //Construtor
	int sinaliza_octree(Octree*);
	void mesh(Octree*);
	void imprime_octree(Octree*);
	float get_data(const int,const int,const int);
	void set_data(const int,const int,const int,float);


};

// Constructor with six arguments

Octree::Octree(int x1, int y1, int z1, int x2, int y2, int z2,Octree* pai,float iso, int lvl_atual,int lvl_max)
{
	// This use to construct Octree
	// with boundaries defined
	if (x2 < x1
			|| y2 < y1
			|| z2 < z1) {
		cout << "boundary points are not valid" << endl;
		return;
	}

	//point = nullptr;
	this->parent = pai;
	//point = new Point(x1,y1,z1);
	this->sinal = 0; 						//0 = cubo vazio ou cheio | 1 = cubo intersectado
	this->nivel_max = lvl_max;
	this->nivel = lvl_atual;//fmax(x2,fmax(y2,z2))/(x2-x1)-1;
	this->V_0 = new Point(x1, y1, z1);
	this->V_6 = new Point(x2, y2, z2);
	// Assigning null to the children
	for(int i=0;i<8;i++){
		this->children[i] = nullptr;
	}
	this->isovalue_ = iso;

	if(nivel < nivel_max){
		Octree* temp = nullptr;
		temp = new Octree(x1,y1,z1,(x1+x2)/2,(y1+y2)/2,(z1+z2)/2,this,iso,nivel+1,lvl_max);
		this->children[0] = temp;
		temp = new Octree((x1+x2)/2,y1,z1,x2,(y1+y2)/2,(z1+z2)/2,this,iso,nivel+1,lvl_max);
		this->children[1] = temp;
		temp = new Octree(((x1+x2)/2),y1,((z1+z2)/2),x2,((y1+y2)/2),z2,this,iso,nivel+1,lvl_max);
		this->children[2] = temp;
		temp = new Octree(x1,y1,((z1+z2)/2),((x1+x2)/2),((y1+y2)/2),z2,this,iso,nivel+1,lvl_max);
		this->children[3] = temp;
		temp = new Octree(x1,((y1+y2)/2),z1,((x1+x2)/2),y2,(z1+z2)/2,this,iso,nivel+1,lvl_max);
		this->children[4] = temp;
		temp = new Octree(((x1+x2)/2),((y1+y2)/2),z1,x2,y2,((z1+z2)/2),this,iso,nivel+1,lvl_max);
		this->children[5] = temp;
		temp = new Octree(((x1+x2)/2),((y1+y2)/2),((z1+z2)/2),x2,y2,z2,this,iso,nivel+1,lvl_max);
		this->children[6] = temp;
		temp = new Octree(x1,(y1+y2)/2,(z1+z2)/2,(x1+x2)/2,y2,z2,this,iso,nivel+1,lvl_max);
		this->children[7] = temp;

	}else{
		int _i = this->V_0->x;
		int _j = this->V_0->y;
		int _k = this->V_0->z;

		cube[0] = get_data(_i, _j, _k) - isovalue_;

		if(cube[0] > 0.0)
			++cont_pos_vert_;
		if(cube[0] < 0.0)
			++ cont_neg_vert_;
		if (cube[0] == 0.0) cont_zero_vert_++;


		cube[1] = get_data(_i, _j, _k + 1) - isovalue_;
		if(cube[1] > 0.0)
			++cont_pos_vert_;
		if(cube[1] < 0.0)
			++ cont_neg_vert_;
		if (cube[1] == 0.0) cont_zero_vert_++;


		cube[2] = get_data(_i, _j +1, _k+1) - isovalue_;
		if(cube[2] > 0.0)
			++cont_pos_vert_;
		if(cube[2] < 0.0)
			++ cont_neg_vert_;
		if (cube[2] == 0.0) cont_zero_vert_++;


		cube[3] = get_data(_i, _j + 1, _k) - isovalue_;
		if(cube[3] > 0.0)
			++cont_pos_vert_;
		if(cube[3] < 0.0)
			++ cont_neg_vert_;
		if (cube[3] == 0.0) cont_zero_vert_++;


		cube[4] = get_data(_i + 1, _j, _k)- isovalue_;
		if(cube[4] > 0.0)
			++cont_pos_vert_;
		if(cube[4] < 0.0)
			++ cont_neg_vert_;
		if (cube[4] == 0.0) cont_zero_vert_++;


		cube[5] = get_data(_i+1, _j, _k+1)- isovalue_;
		if(cube[5] > 0.0)
			++ cont_pos_vert_;
		if(cube[5] < 0.0)
			++ cont_neg_vert_;
		if (cube[5] == 0.0) cont_zero_vert_++;


		cube[6] = get_data(_i+1, _j+1, _k+1) - isovalue_;
		if(cube[6] > 0.0)
			++ cont_pos_vert_;
		if(cube[6] < 0.0)
			++ cont_neg_vert_;
		if (cube[6] == 0.0) cont_zero_vert_++;


		cube[7] = get_data(_i+1, _j+1, _k) - isovalue_;
		if(cube[7] > 0.0)
			++cont_pos_vert_;
		if(cube[7] < 0.0)
			++ cont_neg_vert_;
		if (cube[7] == 0.0) cont_zero_vert_++;


		if((cont_pos_vert_ != 8)&&(cont_neg_vert_ != 8)){
			this->sinal = 1;
		}
	}
}

int Octree::sinaliza_octree(Octree* oct){
	if(oct->nivel == oct->nivel_max){
		return oct->sinal;
	}
	else{
		int cont =0;
		cont += sinaliza_octree(oct->children[0]);
		cont += sinaliza_octree(oct->children[1]);
		cont += sinaliza_octree(oct->children[2]);
		cont += sinaliza_octree(oct->children[3]);
		cont += sinaliza_octree(oct->children[4]);
		cont += sinaliza_octree(oct->children[5]);
		cont += sinaliza_octree(oct->children[6]);
		cont += sinaliza_octree(oct->children[7]);

		if((cont == 0) or (cont == 8)){
			oct->sinal = 0;
		}else{
			oct->sinal = 1;
		}
	}
	return oct->sinal;

}


void Octree::imprime_octree(Octree* oct)
{
	for(int i = 0;i<=oct->nivel;i++){
		printf("__");
	}
	printf("(Nivel:%d , V_0:%d%d%d ,V_6:%d%d%d, sig:%d )\n",oct->nivel,oct->V_0->x,oct->V_0->y,oct->V_0->z,oct->V_6->x,oct->V_6->y,oct->V_6->z,oct->sinal);
	if(!(oct->sinal == 0) and !(oct->nivel == oct->nivel_max) ){
		for(int i=0;i<8;i++){
			imprime_octree(oct->children[i]);
		}
	}
}


float Octree::get_data(const int i, const int j, const int k)
{
	return f_grid_data[i + j * size_x + k * size_y * size_z];

}
/*
void Octree::set_data(const int i, const int j, const int k, float value)
{
	matriz[i + j * size_x + k * size_y * size_z] = value;
}
*/
/*
	void Octree::percorre_octree(Octree* oct){
		if(this->nivel == nivel_max){

			for(int i =0;i<8;i++){
				_cube[i] = this->cube[i];
			}
			cont_pos_vert = this->cont_pos_vert_;
			cont_zero_vert = this->cont_zero_vert_;
			cont_neg_vert = this->cont_neg_vert_;

			topology();
		}else{
			percorre_octree(oct->children[0]);
			percorre_octree(oct->children[1]);
			percorre_octree(oct->children[2]);
			percorre_octree(oct->children[3]);
			percorre_octree(oct->children[4]);
			percorre_octree(oct->children[5]);
			percorre_octree(oct->children[6]);
			percorre_octree(oct->children[7]);
		}
	}
 */

/* Função recurssao para atribuir os sinais dos nós
função para percorre a octree e chamar a função que constroi a configuração

	_cube[i] = cube[i];
	cont_pos_vert = cont_pos_vert_;
	cont_zero_vert = cont_zero_vert_;
	cont_neg_vert = cont_neg_vert_;

	topology();

zlib z2lib png teem
 */


#endif // OCTREE_H_
