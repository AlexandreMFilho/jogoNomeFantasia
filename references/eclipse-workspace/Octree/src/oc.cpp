// Implementation of Octree in c++
#include <iostream>
#include <vector>
#include <math.h>
#include "oc.h"
//#include "Extended_MC_.cpp"

using namespace std;

	// Constructor with six arguments

	Octree::Octree(int x1, int y1, int z1, int x2, int y2, int z2,Octree* pai,float iso,int lvl_max)
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
			parent = pai;
			//point = new Point(x1,y1,z1);
			V_0 = new Point(x1, y1, z1);
			V_6 = new Point(x2, y2, z2);
			isovalue_ = iso;
			nivel = fmax(x2,fmax(y2,z2))/(x2-x1)-1;
			this->nivel_max = lvl_max;
			this->sinal =0;
		// Assigning null to the children
		children.assign(8, nullptr);


		if(nivel < nivel_max){
//TODO p_aux = *children[0] //Esse cara temq apontar para o pai, ou seja para o nó atual, ai eu passo pros filhos ele.
			children[0] = new Octree(x1,y1,z1,(x1+x2)/2,(y1+y2)/2,(z1+z2)/2,this,iso,lvl_max);

			children[1] = new Octree((x1+x2)/2,y1,z1,x2,(y1+y2)/2,(z1+z2)/2,this,iso,lvl_max);

			children[2] = new Octree(((x1+x2)/2),y1,((z1+z2)/2),x2,((y1+y2)/2),z2,this,iso,lvl_max);

			children[3] = new Octree(x1,y1,((z1+z2)/2),((x1+x2)/2),((y1+y2)/2),z2,this,iso,lvl_max);

			children[4] = new Octree(x1,((y1+y2)/2),z1,((x1+x2)/2),y2,z2,this,iso,lvl_max);

			children[5] = new Octree(((x1+x2)/2),((y1+y2)/2),z1,x2,y2,((z1+z2)/2),this,iso,lvl_max);

			children[6] = new Octree(((x1+x2)/2),((y1+y2)/2),((z1+z2)/2),x2,y2,z2,this,iso,lvl_max);

			children[7] = new Octree(x1,(y1+y2)/2,(z1+z2)/2,(x1+x2)/2,y2,z2,this,iso,lvl_max);


		}
	}

	/*
	int Octree::sinaliza_octree(Octree* oct){
		if(this->nivel == nivel_max){

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
			return this->sinal;
		}
		else{
			int cont =0;
			cont += sinaliza_octree(this->children[0]);
			cont += sinaliza_octree(this->children[1]);
			cont += sinaliza_octree(this->children[2]);
			cont += sinaliza_octree(this->children[3]);
			cont += sinaliza_octree(this->children[4]);
			cont += sinaliza_octree(this->children[5]);
			cont += sinaliza_octree(this->children[6]);
			cont += sinaliza_octree(this->children[7]);

			if((cont != 0) || (cont != 8)){
				this->sinal = 1;
			}
		}
		return this->sinal;

	}
	*/

	void Octree::imprime_octree(Octree* oct)
	{
		for(int i = 0;i<=this->nivel;i++){
			printf("\t");
		}
		printf("(Nivel:%d , i:%d , j:%d , k:%d , sig:%d )\n",this->nivel,this->V_0->x,this->V_0->y,this->V_0->z,this->sinal);

		if(this->nivel < nivel_max){

			for(int i =0;i<8;i++){
				imprime_octree(oct->children[i]);
			}

		}
	}

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
