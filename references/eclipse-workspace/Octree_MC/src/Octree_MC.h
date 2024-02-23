#include <iostream>
#include <teem/nrrd.h>
#include "chull.h"



//CREATE FINAL MESH =======================================================================================
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <wrap/io_trimesh/export_off.h>
//===================================================================================================
using namespace std;

char *output_mesh_file = nullptr;
char *output_debug_file = nullptr;
FILE *snap_mesh = nullptr;

enum {TUNNEL, LEAF, ADD_INT_P, ON_FACE};

int dim = 3;
int size_x, size_y, size_z;
//Ajuste dos tamanhos do grid para utilização da octree /Menor maior potencia cubica de 2 com relaçao a matriz do dado
int real_size_x, real_size_y, real_size_z; //
double sx=1, sy=1, sz=1;
float isovalue = 0.0;

float *f_grid_data = nullptr;
unsigned char *u_grid_data = nullptr;
short *s_grid_data = nullptr;

int case_ = 0;
int data_type = 0;
int n_vert = 8;
int n_edge = 12;
int n_face = 6;
int n_e_connect = 6;
int n_v_connct = 3;
int n_groups = 20;
int real_n_groups = 0;
int ngroups = 0;
int n_trig_p_group = 10;
int n_edges_p_face = 4;

int n_amb_face = 0;
int cube_sinal = 1;

int _i, _j, _k;

int add_interior_point = 0;

int int_ambiguity = 0;

int cont_face_sinal = 0.0;

int cont_pos_vert = 0;
int cont_zero_vert = 0;
int cont_neg_vert = 0;

int vert_on_trig[8];
int ind_vert_on_trig = 0;

float _cube[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

int amb_face[6] = {-1,-1,-1,-1,-1,-1};
float face_critical_sinal[6] = {-1,-1,-1,-1,-1,-1};

float vert_coord[8][3] = {{0, 0, 0},
{ 1, 0, 0},
{ 1, 1 , 0},
{ 0, 1, 0},
{ 0, 0, 1},
{ 1, 0, 1 },
{ 1, 1, 1  },
{ 0, 1, 1}};

int transform_index [8] = {0,1,3,2,4,5,7,6};

int v_connectivity[8][3] = {{1,3,4}, {0,2,5}, {1,3,6}, {0,2,7},{0,5,7},{1,4,6}, {2,5,7}, {3,4,6}};

int ev_connectivity[8][3] = {{3,0,8},{0,1,9},{1, 10, 2},{3,2,11},{8,4,7},{4,9,5},{5,10,6},{6,7,11}};

int e_connectivity[12][6] = {{3,8,2,4,1,9},{0,9,3,5,2,10},{1,10,0,6,3,11}, {2,11,1,7,0,8},
                             {8,7,0,6,2,5},{2,4,1,7,10,6},{5,10,2,4,7,11},{6,11,3,5,4,8},
                             {0,3,9,11,4,7},{0,1,8,10,4,5},{1,2,9,11,5,6},{2,3,10,8,6,7}};

int edge_nodes[12][2] = {{0,1},{1,2},{3,2},{0,3},{4,5},{5,6},{7,6},{4,7}, {0,4}, {1,5}, {2,6}, {3,7}};

int face_edges[6][4]= {{0,9,4,8},{1,10,5,9},{2,11,6,10}, {3,8,7,11}, {0,1,2,3}, {4,5,6,7}};

int face_nodes[6][4]      = {{0, 1, 5, 4},{1, 2, 6, 5},{2, 3, 7, 6},{3, 0, 4, 7},{0, 1, 2, 3},{4, 5, 6, 7}};

int ev_conect_on_face[6][4][2]= {{{8,0},{0,9},{9,4},{4,8}},
                                 {{9,1},{1,10},{10,5},{5,9}},
                                 {{10,2},{2,11},{11,6},{6,10}},
                                 {{11,3},{3,8},{8,7},{7,11}},
                                 {{3,0},{0,1},{1,2},{2,3}},
                                 {{7,4},{4,5},{5,6},{6,7}}};

int oposite_on_face[6][4] = {{5, 4, 0, 1},{6, 5, 1, 2},{7, 6, 2, 3},{4, 7, 3, 0},{2, 3, 0, 1},{6, 7, 4, 5}};

int ev_op_connect_on_face[6][4][2] = {{{9,4},{4,8},{8,0},{0,9}},
                                      {{10,5},{5,9},{9,1},{1,10}},
                                      {{11,6},{6,10},{10,2},{2,11}},
                                      {{8,7},{7,11},{11,3},{3,8}},
                                      {{1,2},{2,3},{3,0},{0,1}},
                                      {{5,6},{6,7},{7,4},{4,5}}};

int face_neighbor_id[6][3] = {{0,-1,0}, {1,0,0}, {0,1,0}, {-1,0,0},{0,0,-1}, {0,0,1}};

int diagonal_opposite[8] = {6,7,4,5,2,3,0,1};

int v_group[8];
int *e_group;

//int *visited_cube;

int on_face[4];

int n_on_face = 0;

int **group_of_edges;
int **group_trigs;
int *n_group_edge;

int amb_config = -1;
int n_critical_point = -1;
float c_point_value[2] = {-1,-1};
int tunnel_orientation = 0;
int interior_topology = -1;

int verification = 0;

std::vector<char> snap_mesh_element;
std::vector<int> snap_mesh_index;
std::vector<int> snap_mesh_cube;

std::vector<float> _x;
std::vector<float> _y;
std::vector<float> _z;

int n_trig = 0;
int final_nTrig = 0;

int _interior_topology = 0;

int edge_f_to_c[12][2] = {{0,1},{1,2},{3,2},{0,3},{4,5},{5,6},{7,6},{4,7},{0,4},{1,5},{2,6},{5,7}};

//========================================================================================================

class MyFace;
class MyVertex;

 struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,  vcg::Use<MyFace>::AsFaceType>{};

 class MyVertex  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::Color4b, vcg::vertex::BitFlags  >{};
 class MyFace    : public vcg::Face < MyUsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::FFAdj, vcg::face::BitFlags > {};
 class MyMesh    : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace> > {};

 using namespace vcg;

 //========================================================================================================

 FILE* arquivo = nullptr;
 char nome[80];
 float dado_vol;
 int contador_global =0;

 FILE* abre_arquivo();
 void fecha_arquivo(FILE*);
 int pot_2(int);

 void write_mesh();
 void read_nhdr(char *filename);
 float get_data(const int, const int, const int);
 void set_data(const int, const int, const int, float);
 void initializing_groups();
 void find_amb_faces(float);
 void cubeSinal();
 int get_edge(float, float, float);
 bool interior_test_pos();
 bool interior_test_neg();
 void find_v_groups_pos();
 void find_v_groups_neg();
 void join_v_groups_by_face_pos();
 void join_v_groups_by_face_neg();
 void find_e_groups_pos();
 void find_e_groups_neg();
 void groups_of_edges();
 void read_triangulation();
 int run(char*);
 int maior_pot_2(int);
 FILE* abre_arquivo();
 void fecha_arquivo(FILE*);




















