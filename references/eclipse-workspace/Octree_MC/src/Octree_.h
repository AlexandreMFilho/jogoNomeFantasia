
#ifndef OCTREE_H_
#define OCTREE_H_

#include "Octree_MC.h"

#include <cstdio>
#include <iostream>
#include <vector>

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

struct AddPoints{
	float x;
	float y;
	float z;
	int edge;
};

// Octree class
class Octree
{
public:
	Octree* parent;
	int sinal,nivel,nivel_max;
	//const int nive_max;
	Point *V_0, *V_6;
	Octree* children[8];
	std::vector<AddPoints> Extrapoints;
	int nExtrapoints = 0;
	bool isfather;
	float cube[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
	int cont_pos_vert_=0;
	int cont_zero_vert_=0;
	int cont_neg_vert_=0;
	float isovalue_ = 0;
	int topology_ = -1;
	int id;
	int merge_step=1;

public:
	Octree(int , int , int , int , int , int ,Octree*,float,int,int,int); //Construtor
	int sinaliza_octree(Octree*);
	void mesh(Octree*);
	void imprime_octree(Octree*);
	void imprime_octree_F(Octree*,FILE*);
	float get_data(const int,const int,const int);
	void set_data(const int,const int,const int,float);
	void merge(Octree*);
	bool verifica_intersecoes_nivel(Octree*);
	void visualiza_octree(Octree*);
	void visualiza_octree_F(Octree*,FILE*);
	float getcube(int);
	void throw_dot_on_uncle(Octree*,vector<AddPoints>);

};

std::vector<AddPoints> leaf_triangulation(Octree*);
std::vector<AddPoints> on_face_triangulation(int *, int,Octree*);
std::vector<AddPoints> tunnel_triangulation(Octree*);
std::vector<AddPoints> one_inter_point_triangulation(Octree*);
int topology(Octree*);
std::vector<AddPoints> triangulation(Octree*);
int find_topology(Octree*);
void teste_read_triangulation(Octree*);

// Constructor with six arguments

Octree::Octree(int x1, int y1, int z1, int x2, int y2, int z2,Octree* pai,float iso, int lvl_atual,int lvl_max,int identidade)
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
	this->id = identidade;
	this->isfather = true;
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

	//Enquanto não está no nível máximo cria 8 nós filhos.
	if(nivel < nivel_max){
		Octree* temp = nullptr;
		temp = new Octree(x1,y1,z1,(x1+x2)/2,(y1+y2)/2,(z1+z2)/2,this,iso,nivel+1,lvl_max,0);
		this->children[0] = temp;
		temp = new Octree((x1+x2)/2,y1,z1,x2,(y1+y2)/2,(z1+z2)/2,this,iso,nivel+1,lvl_max,1);
		this->children[1] = temp;
		temp = new Octree(((x1+x2)/2),y1,((z1+z2)/2),x2,((y1+y2)/2),z2,this,iso,nivel+1,lvl_max,2);
		this->children[2] = temp;
		temp = new Octree(x1,y1,((z1+z2)/2),((x1+x2)/2),((y1+y2)/2),z2,this,iso,nivel+1,lvl_max,3);
		this->children[3] = temp;
		temp = new Octree(x1,((y1+y2)/2),z1,((x1+x2)/2),y2,(z1+z2)/2,this,iso,nivel+1,lvl_max,4);
		this->children[4] = temp;
		temp = new Octree(((x1+x2)/2),((y1+y2)/2),z1,x2,y2,((z1+z2)/2),this,iso,nivel+1,lvl_max,5);
		this->children[5] = temp;
		temp = new Octree(((x1+x2)/2),((y1+y2)/2),((z1+z2)/2),x2,y2,z2,this,iso,nivel+1,lvl_max,6);
		this->children[6] = temp;
		temp = new Octree(x1,(y1+y2)/2,(z1+z2)/2,(x1+x2)/2,y2,z2,this,iso,nivel+1,lvl_max,7);
		this->children[7] = temp;

	//Chegou no nível máximo, carrega o nó folha.
	}else{
		this->isfather = false;
		int _i = this->V_0->x;
		int _j = this->V_0->y;
		int _k = this->V_0->z;

		if(_i < size_x -1 and _j < size_y-1 and _k < size_z-1)
		{


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
		}
		else{
			for(int p =0; p<8;p++){
				cube[p] = 0.0 - isovalue_;
				cont_pos_vert = 8.0;
				cont_neg_vert = 8.0;
			}
		}

		if((cont_pos_vert_ != 8)&&(cont_neg_vert_ != 8)){
			this->sinal = 1;
		}
	}
}



//Esta função percorre a octree e preenche cada nó superior com a informação
//do sinal de seus filho. Se todos os seus filhos não tiverem nenhuma
//configuração complexa, quer dizer que o merge pode acontecer, caso contrário
//será necessário entrar para ver os filhos.
int Octree::sinaliza_octree(Octree* oct){
	//Quando em um nó folha, verifica a topologia dele e carrega na variável do nó
	//a topologia e retorna o sinal do nó folha para o pai.
	if(oct->nivel == oct->nivel_max){
		//chamar o topology
		//if(oct->sinal == 1){
		if((oct->cont_pos_vert_==8)||(oct->cont_neg_vert_==8)){
			oct->topology_ =-1;
			oct->sinal = 1;
			return 1;//return oct->sinal
		}
		else{
			oct->topology_ = topology(oct);
			if(oct->topology_ ==1)return 1;
			else return 0;
			/*
			//TODO se ele topology=1(LEAF), return topology, else return =0;
			//TODO OBS retirar linha 244
		//}
		return oct->sinal;
		//TODO
*/
		}
	}
	//Se não for um nó folha entra nos 8 filhos trazendo o sinal deles,se todos
	//ou nenhum possuei topologia simples, então neste nó pode ser feito o merge, logo
	//seu sinal será 0(sem informação relevante nos filhos, 1 Com informação relevante
	//nos filhos, necessário ir neles.
	else{
		int cont = 0;
		cont += sinaliza_octree(oct->children[0]);
		cont += sinaliza_octree(oct->children[1]);
		cont += sinaliza_octree(oct->children[2]);
		cont += sinaliza_octree(oct->children[3]);
		cont += sinaliza_octree(oct->children[4]);
		cont += sinaliza_octree(oct->children[5]);
		cont += sinaliza_octree(oct->children[6]);
		cont += sinaliza_octree(oct->children[7]);

		//Todos ou nenhum dos nós fihos possuem topologia simples, nó pai recebe 0,
		//e pode ser vitima do merge.
		if(cont == 8){
			//SE contador ==8, signal =1(faz merge), else signal =0(não faz)
			oct->sinal = 1;

		}
		//Não pode ser vitima do merge pois os nós filhos possuem informação relevante.
		else{
			oct->sinal = 0;
		}
	}
	//retorna o sinal deste nó para o pai dele.
	return oct->sinal;
}

void Octree::merge(Octree * oct){

	//int cont = 0;
	bool issimple = true, domerge = true;
	Octree* p= nullptr;

	if(oct->nivel == oct->nivel_max - 1){

		//Percorre os filhos pegando o sinal e a topologia
		for(int i=0;i<8;i++){
			//cont += oct->children[i]->sinal;
			//top[i] = oct->children[i]->topology_;
			if(oct->children[i]->topology_ == 0 || oct->children[i]->topology_ == 2 || oct->children[i]->topology_ == 3){
				issimple = false;
				domerge = false;
				break;
			}
/*
			if(!issimple){
				break;
			}*/
		}

		//SE Todos os filhos estão vazios
		if(domerge){
			//Merge e mata os filhos
			oct->isfather = false;
			for(int i=0;i<8;i++){
				oct->cube[i] = oct->children[i]->cube[i];
				p = oct->children[i];
				p->parent = nullptr;
				oct->children[i] = nullptr;
				//delete(p);
			}
			oct->topology_ = LEAF;
			oct->merge_step = oct->merge_step *2;
		//SENAO Verificar se são casos simples
		}
	}
	else{
		merge(oct->children[0]);
		merge(oct->children[1]);
		merge(oct->children[2]);
		merge(oct->children[3]);
		merge(oct->children[4]);
		merge(oct->children[5]);
		merge(oct->children[6]);
		merge(oct->children[7]);
	}

/*Vai até o penultimo nível e verifica se é possível
 *fazer o merge com os filhos.
	Se todos vazios fazer o merge.
	 merge {
	 Incializar os vertices do no pai com vertices I dos filhos I.
	Matar os 8 filhos.
	}
	Se tiver alguém la dentro. se (sinal == 1)
*/
}

//-----------------------------------------------------------------------------------------------20/07/22
/*
 * Ao ocorrer o merge, navegar até as folhas e em cada ponto de interseção, verificar nele.
 * 1 - a face do nó que ele se encontra.
 * pela tabela 1 de correspondência de faces entre filho e pai e descobrir se esse ponto está em uma face
 * interna ou externa.
 * TABELA 1
 * face nó corrente |
 * 			0
 * (interna, ele divide com o irmão) (externa, face x do filho corresponde a face x do pai.)
 * se interna, passar para o próximo ponto de interseção.
 * se externa, verificar na tabela 2 de correspondência de faces entre irmãos subindo os níveis procurando
 * nos irmãos do nó corrente a face do
 * corrente
 *
*/
//-----------------------------------------------------------------------------------------------

void Octree::imprime_octree(Octree* oct)
{
	for(int i = 0;i<=oct->nivel;i++){
		printf("__");
	}
	printf("(Nivel:%d , V_0:%d%d%d ,V_6:%d%d%d, sig:%d, topology:%d, isfather:%d)\n",oct->nivel,oct->V_0->x,oct->V_0->y,oct->V_0->z,oct->V_6->x,oct->V_6->y,oct->V_6->z,oct->sinal,oct->topology_,oct->isfather);
	if(!(oct->sinal == 0) and (!(oct->nivel == oct->nivel_max)) ){
		for(int i=0;i<8;i++){
			imprime_octree(oct->children[i]);
		}
	}
}

void Octree::imprime_octree_F(Octree* oct,FILE* arq)
{
	fprintf(arq,"\nIMPRIME\n");
	for(int i = 0;i<=oct->nivel;i++){
		fprintf(arq,"__");
	}
	fprintf(arq,"(Nivel:%d , V_0:%d%d%d ,V_6:%d%d%d, sig:%d, topology:%d, isfather:%d)\n",oct->nivel,oct->V_0->x,oct->V_0->y,oct->V_0->z,oct->V_6->x,oct->V_6->y,oct->V_6->z,oct->sinal,oct->topology_,oct->isfather);
	if(!(oct->sinal == 0) and (!(oct->nivel == oct->nivel_max)) ){
		for(int i=0;i<8;i++){
			imprime_octree(oct->children[i]);
		}
	}
}

void Octree::visualiza_octree(Octree* oct)
{
	for(int i = 0;i<=oct->nivel;i++){
		printf("__");
	}
	printf("(Nivel:%d , V_0:%d%d%d ,V_6:%d%d%d, sig:%d, topology:%d, isfather:%d)\n",oct->nivel,oct->V_0->x,oct->V_0->y,oct->V_0->z,oct->V_6->x,oct->V_6->y,oct->V_6->z,oct->sinal,oct->topology_,oct->isfather);
	if((!(oct->nivel == oct->nivel_max)) and oct->isfather == true ){
		for(int i=0;i<8;i++){
			visualiza_octree(oct->children[i]);
		}
	}
}
void Octree::visualiza_octree_F(Octree* oct,FILE* arq)
{
	contador_global +=1;
	for(int i = 0;i<=oct->nivel;i++){
		fprintf(arq,"__");
	}
	fprintf(arq,"(Nivel:%d , V_0:%d%d%d ,V_6:%d%d%d, sig:%d, topology:%d, isfather:%d)\n",oct->nivel,oct->V_0->x,oct->V_0->y,oct->V_0->z,oct->V_6->x,oct->V_6->y,oct->V_6->z,oct->sinal,oct->topology_,oct->isfather);
	if(oct->isfather == true){
		for(int i=0;i<8;i++){
			visualiza_octree_F(oct->children[i],arq);
		}
	}
}

float Octree::get_data(const int i, const int j, const int k)
{
	return f_grid_data[i + j * size_x + k * size_y * size_z];

}

/*
void Octree::mesh(Octree* oct){
	std::vector<AddPoints> extrapoints;
	extrapoints.clear();
	//AddPoints pont;
	//extrapoints.emplace_back(pont);
	//for(int i =0;i<=oct->nivel;i++){
	//	printf("__");
	//}
	//printf("\n");

	if((oct->nivel == oct->nivel_max)){//||((oct->topology_ == LEAF)and(oct->isfather == false))){// or (oct->sinal == 0 and isfather == false)){
	//if((!(oct->nivel == oct->nivel_max)) and oct->isfather == true ){
	//if(!(oct->sinal == 0) and (!(oct->nivel == oct->nivel_max)) ){
		for(int i =0;i<8;i++){
				_cube[i] = oct->cube[i];
				//printf("%f ",_cube[i]);
			}
			cont_pos_vert = oct->cont_pos_vert_;
			cont_zero_vert = oct->cont_zero_vert_;
			cont_neg_vert = oct->cont_neg_vert_;
			_i = oct->V_0->x;
			_j = oct->V_0->y;
			_k = oct->V_0->z;
			_interior_topology = oct->topology_;
			extrapoints = triangulation(oct);
			//printf("%d %d %d\n",cont_pos_vert,cont_zero_vert,cont_neg_vert);
			//COLOCAR AQUI VERIFICAÇÂO SE OS PONTOS DEVEM OU NÇAR SER ADDICINADOS
			//int size=extrapoints.size();
			if(extrapoints.size() >0)throw_dot_on_uncle(oct,extrapoints);//--------------------------------------
			//passa o extrapoints dentro da verificação e de fato adicionar o retorno da verificação
	}
	else if(oct->isfather == true){
		for(int i=0;i<8;i++){
			mesh(oct->children[i]);
		}
	}

	//LIMPAR O VETOR após usar.
	//Ver na doc do Vector como limpar ele.
}

*/
void Octree::mesh(Octree* oct){
	std::vector<AddPoints> extrapoints;
	extrapoints.clear();

	if(oct->isfather){
		for(int i=0;i<8;i++){
			mesh(oct->children[i]);
		}
	}else{
		if(oct->topology_ == LEAF){
			for(int i =0;i<8;i++){
				_cube[i] = oct->cube[i];
			}
			cont_pos_vert = oct->cont_pos_vert_;
			cont_zero_vert = oct->cont_zero_vert_;
			cont_neg_vert = oct->cont_neg_vert_;
			_i = oct->V_0->x;
			_j = oct->V_0->y;
			_k = oct->V_0->z;
			_interior_topology = oct->topology_;
			extrapoints = triangulation(oct);
			if(extrapoints.size() >0)throw_dot_on_uncle(oct,extrapoints);
		}
	}
}

bool Octree::verifica_intersecoes_nivel(Octree* pai){
	int aux_sinal = 0, f1=0,f2=0,ef1=0,ef2=0;
	//Checa todas as 12 arestas.
	for(int aresta_num=0; aresta_num<12;aresta_num++){
		f1 = edge_f_to_c[aresta_num][0];
		f2 = edge_f_to_c[aresta_num][1];

		ef1 = aresta_num; //aresta filho 1 ?
		ef2 = aresta_num; //aresta filho 2 ?

		//acessar f1
		if(pai->children[f1]->getcube(edge_nodes[ef1][0]) * pai->children[f1]->getcube(edge_nodes[ef1][1]) < 0){ //checar se a aresta X do filho1 é intersectada, utilizando os vertices que a compoem.
			aux_sinal+=1;
		}
		//acessar f2
		if(pai->children[f2]->getcube(edge_nodes[ef2][0]) * pai->children[f2]->getcube(edge_nodes[ef2][1]) < 0){
			aux_sinal+=1;
		}
		if(aux_sinal >= 2){
			return false;
		}
	}
	return true;
}


float Octree::getcube(int n){
	return this->cube[n];
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

//Esta matriz me informa através de um dado nó, na determinada face dele, qual outro nó compartilha esta mesma face com ele.
//[no][face][0] -> face y do nó que compartilha aquela face
//[no][face][1] -> (Se >0){irmão y}(senão se == -1){pai x}
int correspondence_faces_to_node[8][6][2] = {
		{{0,-1},{3,1},{0,3},{3,-1},{4,-1},{4,4}},//---------------
		{{0,-1},{1,-1},{0,2},{1,0},{4,-1},{4,5}},
		{{2,1},{1,-1},{2,-1},{1,3},{4,-1},{4,6}},
		{{2,0},{3,2},{2,-1},{3,-1},{4,-1},{4,7}},
		{{0,-1},{3,5},{0,7},{3,-1},{5,0},{5,-1}},
		{{0,-1},{1,-1},{0,6},{1,4},{5,1},{5,-1}},
		{{2,5},{1,-1},{2,-1},{1,7},{5,2},{5,-1}},
		{{2,0},{3,6},{2,-1},{3,-1},{5,3},{5,-1}}
};

//Esta matriz me informa quais faces, uma dada aresta faz parte. ex: a aresta 0, faz parte das faces 0 e 4.
int edges_in_face[12][2]={
		{0,4},{4,1},{4,2},{4,3},{0,5},{1,5},{2,5},{3,5},{0,3},{0,1},{1,2},{2,3}
};

/*
 * Criar uma funcção onde a aresta de um filho retorne a face do tio e qual tio.
 * Retornar a qual tio é incidente e a qual face dele.
 * recebo uma ponto(Points), a aresta que ele está, e o índice do filho ela pertence.
 * verificar se é necessário, se for entre irmão n precisa.
 * alocar o ponto na variável Extrapoints do tio correspondente.
 * *Se os irmãos estão subdivididos, não preciso me preocupar.
 * Essa função será colocada dentro do triangulation
 * uma função da classe Octree chamada dentro da Triangulation.
*/

void Octree::throw_dot_on_uncle(Octree* oct,vector<AddPoints> pontos){

	int face_1, face_2,face_valida = -1,pai,tio,edge;
	int id_node = oct->id;

	for(int i=0;i<pontos.size();i++){
		edge = pontos[i].edge;

		//Preciso verificar a qual aresta pertence o ponto de interseção
		//com isso, eu sei em que face do meu nó ele se encontra.
		//Esta aresta faz parte de duas faces neste nó, aqui eu armazeno as duas:
		face_1 = edges_in_face[edge][0];
		face_2 = edges_in_face[edge][1];

		 //Sabendo a face do nó, eu sei se essa face é dividida com um irmão ou com o pai.
		//Aqui eu verifico qual das duas faces, é compartilhada com o nó pai e a armazeno na variavel face_valida.
		 //Se for com um irmão eu não faço nada.
		 //Se for com o pai, então eu vou em um dos tios e coloco esse ponto nele.
		if(correspondence_faces_to_node[id_node][face_1][1] == -1){
			face_valida = face_1;
		}else if(correspondence_faces_to_node[id_node][face_2][1] == -1){
			face_valida = face_2;
		}
		//Se a face for váida, fará algo, do contrario não faz.
		if(face_valida != -1){
			 //Ao saber qual face o meu nó divide com o pai, eu preciso saber qual o id no pai.
			 pai = oct->parent->id;
			 //Com o id do pai, eu descubro qual tio compartilha aquela face com ele, assim eu sei quem irá receber os pontos
			 tio = correspondence_faces_to_node[pai][face_valida][1];
			 //Aqui eu, parto do nó inicial, subo para o pai dele, para o avô e então chego no tio para colocar os pontos nele.
			 oct->parent->parent->children[tio]->Extrapoints.emplace_back(pontos[i]);
			 oct->parent->parent->children[tio]->nExtrapoints += 1;
		}
	}
}


//Tabelas retiradas da matriz correspondende_faces_to_node[][][];
/* 1 -  esta face coincide com a do pai. -1 - esta face é compartilhada com um irmão
int face_F0_e_pai[6] = {1,-1,-1,1,1,-1};
int face_F1_e_pai[6] = {1,1,-1,-1,1,-1};
int face_F2_e_pai[6] = {-1,1,1,-1,1,-1};
int face_F3_e_pai[6] = {-1,-1,1,1,1,-1};
int face_F4_e_pai[6] = {1,-1,-1,1,-1,1};
int face_F5_e_pai[6] = {1,1,-1,-1,-1,1};
int face_F6_e_pai[6] = {-1,1,1,-1,-1,1};
int face_F7_e_pai[6] = {-1,-1,1,1,-1,1};


// No nó x quais irmãos compartilham faces com ele
int filho_0_faces_interior[3] = { 1,2,5};
int filho_1_faces_interior[3] = { 2,3,5};
int filho_2_faces_interior[3] = { 0,3,5};
int filho_3_faces_interior[3] = { 0,1,5};
int filho_4_faces_interior[3] = { 1,2,4};
int filho_5_faces_interior[3] = { 2,3,4};
int filho_6_faces_interior[3] = { 0,3,4};
int filho_7_faces_interior[3] = { 0,1,4};


//A prof vai me mandar as alterações em pseudocódigo, elas consistem
 * em alterações dentro da leaf_triangulation onde faz a leitura de
 * um nó com os Adddedpoints, faz a leitura de um nó folha, que irá
 * retornar todos os seus pontos de interseção e terei q verificar
 * nele quais pontos serão passados para o tio. Minha função ja faz
 * isso, mas assumindo estar recebendo os pontos corretos,
 * TODO modificar a função throw_dot_on_uncle para que isso aconteça.
 * A leaf_triangulation é chamada dentro de triangulation, então
 * agora a  leaf triangulation retornará um vector<Addedpoints>,
 * que a triangulation retornará dentro de mesh(), assim a
 * triangulation irá retornar também um vector<Addedpoints>, com esse
 * vector irei chamar throw_dot_on_uncle passando o como parâmetro
 *, agora essa função deverá ler o vector e pela identificação da
 *, aresta verificar se esse ponto será ou não enviado.
 *,
 *, A estrutura
 *, AddedPoints{
 *, float x
 *, floar y
 *, float z
 *, int aresta
 *, }
 *,
 *, **TODO Modificar as variáveis de Points para serem floats
 *, TODO Criar uma solução caso a matriz lida não seja potencia de 2
 *


*/



#endif // OCTREE_H_
