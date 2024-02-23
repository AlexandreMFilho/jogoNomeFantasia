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
FILE *snap_mesh = nullptr;

enum {TUNNEL, LEAF, ADD_INT_P, ON_FACE};

int dim = 3;
int size_x, size_y, size_z;
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




//========================================================================================================

class MyFace;
class MyVertex;

 struct MyUsedTypes : public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType,  vcg::Use<MyFace>::AsFaceType>{};

 class MyVertex  : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::Color4b, vcg::vertex::BitFlags  >{};
 class MyFace    : public vcg::Face < MyUsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::FFAdj, vcg::face::BitFlags > {};
 class MyMesh    : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace> > {};

 using namespace vcg;

 //========================================================================================================

#include "Octree_.h"
#include <stdio.h>

 FILE* arquivo = nullptr;
 char nome[80];
 float dado_vol;

 FILE* abre_arquivo();
 void fecha_arquivo(FILE*);
 int pot_2(int);


 //========================================================================================================
void write_mesh()
{
	MyMesh Mesh;
	vector<Point3f> coordVec;
	vector<Point3i> indexVec;

	for(int t = 0;t<n_trig;++t)
	{
	   coordVec.push_back(Point3f(_x[3 * t],_y[3 * t],_z[3 * t]));
	   coordVec.push_back(Point3f(_x[3 * t + 1],_y[3 * t + 1],_z[3 * t + 1]));
	   coordVec.push_back(Point3f(_x[3 * t + 2],_y[3 * t + 2],_z[3 * t + 2]));
	}

	for(int i = 0; i < n_trig; ++i)
		{indexVec.push_back(Point3i(3*i, 3*i + 1, 3*i + 2));}

	tri::BuildMeshFromCoordVectorIndexVector(Mesh,coordVec,indexVec);
	tri::UpdateTopology<MyMesh>::FaceFace(Mesh);
	tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(Mesh);
	tri::UpdateNormal<MyMesh>::PerVertexFromCurrentFaceNormal(Mesh);
	bool oriented, orientable;
	tri::Clean<MyMesh>::OrientCoherentlyMesh(Mesh, oriented,orientable);
	tri::Clean<MyMesh>::RemoveDuplicateVertex(Mesh);
	tri::Clean<MyMesh>::RemoveDuplicateEdge(Mesh);
	tri::io::ExporterOFF<MyMesh>::Save(Mesh,output_mesh_file);
}
//===================================================================================================
void read_nhdr(char *filename)
{
	char *err = nullptr;
	cube_sinal = 1;

	/* create a nrrd; at this point this is just an empty container */
	Nrrd *nin = nrrdNew();

	/* read in the nrrd from file */
	if (nrrdLoad(nin, filename, NULL))
	{
		err = biffGetDone(NRRD);
		fprintf(stderr, "read_nhdr: trouble reading \"%s\":\n%s", filename,
				err);
		free(err);
		return;
	}

	/* say something about the array */
	printf("read_nhdr: \"%s\" is a %d-dimensional nrrd of type %d (%s)\n",
			filename, nin->dim, nin->type, airEnumStr(nrrdType, nin->type));
	printf("read_nhdr: the array contains %d elements, each %d bytes in size\n",
			(int) nrrdElementNumber(nin), (int) nrrdElementSize(nin));

	size_x = nin->axis[0].size;
	size_y = nin->axis[1].size;
	size_z = nin->axis[2].size;


	sx = nin->axis[0].spacing;
	if(sx != sx) sx = 1;

	sy = nin->axis[1].spacing;
	if(sy != sy) sy = 1;

	sz = nin->axis[2].spacing;
	if(sz != sz) sz = 1;

	int n_data = (int) nrrdElementNumber(nin);

	data_type = nin->type;


	if(data_type == 2)
	{
		u_grid_data = new unsigned char[n_data];
		memcpy(u_grid_data, nin->data, n_data * sizeof(unsigned char));

		f_grid_data = new float[n_data];

		for (int i = 0; i < n_data; ++i)
			f_grid_data[i] = (float)u_grid_data[i];

		delete[] u_grid_data;
	}
	if (data_type == 3)
	{
		s_grid_data = new short[n_data];
		memcpy(s_grid_data, nin->data, n_data * sizeof(short));

		f_grid_data = new float[n_data];

		for (int i = 0; i < n_data; ++i)
			f_grid_data[i] = (float)s_grid_data[i];

		delete[] s_grid_data;
	}
	if(data_type == 9)
	{
		f_grid_data = new float[n_data];
		memcpy(f_grid_data, nin->data, n_data * sizeof(float));
	}

	nrrdNuke(nin);

}
//===================================================================================================

float get_data(const int i, const int j, const int k)
{
		return f_grid_data[i + j * size_x + k * size_x * size_y];

}
//===================================================================================================
void set_data(const int i, const int j, const int k, float value)
{
		f_grid_data[i + j * size_x + k * size_x * size_y] = value;
}

/*
float get_data(const int i, const int j, const int k)
{
		return matriz[i + j * size_x + k * size_x * size_y];

}
//===================================================================================================
void set_data(const int i, const int j, const int k, float value)
{
		matriz[i + j * size_x + k * size_x * size_y] = value;
}
*/
//===================================================================================================
void initializing_groups()
{
	for (int i = 0; i < n_vert; ++i)
		v_group[i] = -1;

	for (int i = 0; i < n_edge; ++i)
		e_group[i] = -1;


	for (int i = 0; i < n_groups; ++i)
		for (int j = 0; j < 3 * n_trig_p_group; ++j)
			group_of_edges[i][j] = -1;

	for (int i = 0; i < n_groups; ++i)
		for (int j = 0; j < 3 * n_trig_p_group; ++j)
			group_trigs[i][j] = -1;

	for (int i = 0; i < n_groups; ++i)
		n_group_edge[i] = 0;
}
//===================================================================================================
void find_amb_faces(float vert[8])
{
	double A, B, C, D;
	double sinal_d1, sinal_d2;

	interior_topology = -1;

	n_amb_face = 0;
	amb_config = 0;
	add_interior_point = 0;
	int_ambiguity = 0;

	for (int i = 0; i < n_face; ++i)
	{
		A = vert[face_nodes[i][0]];
		B = vert[face_nodes[i][1]];
		C = vert[face_nodes[i][2]];
		D = vert[face_nodes[i][3]];

		sinal_d1 = A * C;
		sinal_d2 = B * D;

		if ((sinal_d1 > 0) && (sinal_d2 > 0))
		{
			int cont = 0;

			for (int j = 0; j < 4; ++j)
			{
				if ((cube_sinal == 1) && (_cube[face_nodes[i][j]] >= 0.0))
					cont++;
				if ((cube_sinal == -1) && (_cube[face_nodes[i][j]] <= 0.0))
					cont++;
			}
			if ((cont == 2) && (A * C >= 0.0) && (B * D >= 0.0))
			{
				amb_config = 1;
				amb_face[i] = 1;
				face_critical_sinal[i] = (sinal_d1 - sinal_d2)
						/ (A + C - B - D);

				n_amb_face++;
			}
			else
				amb_face[i] = 0;
		}
		else
			amb_face[i] = 0;
	}
}
//===================================================================================================
void cubeSinal()
{
	cont_face_sinal = 0.0;

	for (int i = 0; i < n_face; ++i)
	{
		if ((cube_sinal == 1) && (amb_face[i] == 1) && (face_critical_sinal[i] > 0))
			cont_face_sinal++;
		if ((cube_sinal == -1) && (amb_face[i] == 1) && (face_critical_sinal[i] < 0))
			cont_face_sinal++;
	}

	// case 7
	if (n_amb_face == 3)
		if (cont_face_sinal == 3)
			cube_sinal = -1 * cube_sinal; // case 7.4

	//if it is case 10 or 12
	if (n_amb_face == 2)
		if (cont_face_sinal == 2)
			cube_sinal = -1 * cube_sinal;

	// case 13
	if (n_amb_face == 6)
	{
		// case 13.1
		if (cont_face_sinal == 6)
			cube_sinal = -1 * cube_sinal;
		// case 13.2
		if (cont_face_sinal == 5)
			cube_sinal = -1 * cube_sinal;
		// case 13.3
		if (cont_face_sinal == 4)
			cube_sinal = -1 * cube_sinal;
		// case 13.4 or 13.5
		if (cont_face_sinal == 3)
		{

		}
	}
}
//================================================================================================================
int get_edge(float x, float y, float z)
{
  int edge;
  if(z == 0)
  {
    if((x+y)== 0.5)
    {
      if (x == 0.5) edge = 0;
      else edge = 3;
    }
    else
    {
      if(x == 1) edge = 1;
      else edge = 2;
    }
  }
  else if(z == 1)
  {
    if((x+y)== 0.5)
    {
      if (x == 0.5) edge = 4;
      else edge = 7;
    }
    else
    {
      if(x == 1) edge = 5;
      else edge = 6;
    }
  }
  else
  {
    if((x+y)== 0) edge = 8;
    else if((x+y)== 1)
    {
      if(x==0)edge = 11;
      else edge = 9;
    }
    else edge = 10;

  }
  return(edge);
}
//===================================================================================================
bool interior_test_pos()
{
  int sum_of_groups = 0;
  for(int i = 0; i < n_groups; ++i)
  {
    if(n_group_edge[i] != 0)
      sum_of_groups++;
  }
  if(sum_of_groups == 1)
  {
    interior_topology = LEAF;
    return false;
  }
  double a = -_cube[0] + _cube[1] + _cube[3] - _cube[2] + _cube[4] - _cube[5]
  - _cube[7] + _cube[6], b = _cube[0] - _cube[1] - _cube[3] + _cube[2], c =
    _cube[0] - _cube[1] - _cube[4] + _cube[5], d = _cube[0] - _cube[3]
  - _cube[4] + _cube[7], e = -_cube[0] + _cube[1], f = -_cube[0] + _cube[3],
                                                                        g = -_cube[0] + _cube[4], h = _cube[0];

  double x1, y1, z1, x2, y2, z2;
  double dx = b * c - a * e, dy = b * d - a * f, dz = c * d - a * g;

  n_critical_point = 0;
  tunnel_orientation = 0;

  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    if (dx * dy * dz < 0)
    {
      interior_topology = LEAF;
      return true;
    }

    double disc = sqrt(dx * dy * dz);

    x1 = (-d * dx - disc) / (a * dx);
    y1 = (-c * dy - disc) / (a * dy);
    z1 = (-b * dz - disc) / (a * dz);

    if ((x1 > 0) && (x1 < 1) && (y1 > 0) && (y1 < 1) && (z1 > 0)
          && (z1 < 1))
    {
      c_point_value[n_critical_point] = a * x1 * y1 * z1 + b * x1 * y1
      + c * x1 * z1 + d * y1 * z1 + e * x1 + f * y1 + g * z1 + h;
      n_critical_point++;
    }

    x2 = (-d * dx + disc) / (a * dx);
    y2 = (-c * dy + disc) / (a * dy);
    z2 = (-b * dz + disc) / (a * dz);

    if ((x2 > 0) && (x2 < 1) && (y2 > 0) && (y2 < 1) && (z2 > 0)
          && (z2 < 1))
    {
      c_point_value[n_critical_point] = a * x2 * y2 * z2 + b * x2 * y2
      + c * x2 * z2 + d * y2 * z2 + e * x2 + f * y2 + g * z2 + h;
      n_critical_point++;
    }

    if (n_critical_point == 0)
    {
      if (add_interior_point != 1)
        interior_topology = LEAF;

      return false;
    }

    if ((n_critical_point == 1) && (interior_topology != ADD_INT_P))
    {

      if (c_point_value[0] > 0)
        interior_topology = TUNNEL;
      else
        interior_topology = LEAF;

      return true;
    }

    if ((n_critical_point == 2) && (interior_topology != ADD_INT_P))
    {

      if (c_point_value[0] * c_point_value[1] > 0)
      {
        interior_topology = TUNNEL;

        if (c_point_value[0] > 0)
          tunnel_orientation = 1;
        else
          tunnel_orientation = -1;
      }
      else
        interior_topology = LEAF;

      return true;
    }

  }
  else
  {
    interior_topology = LEAF;
    return false;
  }

  return false;
}
//===================================================================================================
bool interior_test_neg()
{
  int sum_of_groups = 0;
  for(int i = 0; i < n_groups; ++i)
  {
    if(n_group_edge[i] != 0)
      sum_of_groups++;
  }
  if(sum_of_groups == 1)
  {
    interior_topology = LEAF;
    return false;
  }

  double a = -_cube[0] + _cube[1] + _cube[3] - _cube[2] + _cube[4] - _cube[5]
  - _cube[7] + _cube[6], b = _cube[0] - _cube[1] - _cube[3] + _cube[2], c =
    _cube[0] - _cube[1] - _cube[4] + _cube[5], d = _cube[0] - _cube[3]
  - _cube[4] + _cube[7], e = -_cube[0] + _cube[1], f = -_cube[0] + _cube[3],
                                                                        g = -_cube[0] + _cube[4], h = _cube[0];

  double x1, y1, z1, x2, y2, z2;
  double dx = b * c - a * e, dy = b * d - a * f, dz = c * d - a * g;

  n_critical_point = 0;
  tunnel_orientation = 0;

  if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
  {
    if (dx * dy * dz < 0)
    {
      interior_topology = LEAF;
      return true;
    }

    double disc = sqrt(dx * dy * dz);

    x1 = (-d * dx - disc) / (a * dx);
    y1 = (-c * dy - disc) / (a * dy);
    z1 = (-b * dz - disc) / (a * dz);

    if ((x1 > 0) && (x1 < 1) && (y1 > 0) && (y1 < 1) && (z1 > 0)
          && (z1 < 1))
    {
      c_point_value[n_critical_point] = a * x1 * y1 * z1 + b * x1 * y1
      + c * x1 * z1 + d * y1 * z1 + e * x1 + f * y1 + g * z1 + h;
      n_critical_point++;
    }

    x2 = (-d * dx + disc) / (a * dx);
    y2 = (-c * dy + disc) / (a * dy);
    z2 = (-b * dz + disc) / (a * dz);

    if ((x2 > 0) && (x2 < 1) && (y2 > 0) && (y2 < 1) && (z2 > 0)
          && (z2 < 1))
    {
      c_point_value[n_critical_point] = a * x2 * y2 * z2 + b * x2 * y2
      + c * x2 * z2 + d * y2 * z2 + e * x2 + f * y2 + g * z2 + h;
      n_critical_point++;
    }

    if (n_critical_point == 0)
    {
      interior_topology = LEAF;
      return false;
    }

    if ((n_critical_point == 1) && (interior_topology != ADD_INT_P))
    {
      if (c_point_value[0] < 0)
        interior_topology = TUNNEL;
      else
        interior_topology = LEAF;

      return true;
    }

    if ((n_critical_point == 2) && (interior_topology != ADD_INT_P))
    {
      if ((c_point_value[0] * c_point_value[1] > 0))
      {
        interior_topology = TUNNEL;

        if (c_point_value[0] > 0)
          tunnel_orientation = -1;
        else
          tunnel_orientation = 1;
      }
      else
        interior_topology = LEAF;

      return true;
    }

  }
  else
  {
    interior_topology = LEAF;
    return false;
  }

  return false;
}
//==============================================================================================
void leaf_triangulation() {

  char *ch_element_index;
  int *ch_vert_edge_index;

  int index_cont = 0;
  int nodes_group;
  int sum_of_groups = 0;
  float sum_coord;
  //int edge;

  float *points = nullptr;
  int npoints;

  ind_vert_on_trig = 0;

  for (int i = 0; i < n_groups; ++i) {
    if (n_group_edge[i] != 0)
      sum_of_groups++;
  }



  if (sum_of_groups != 0) {

    for (int i = 0; i < n_groups; ++i)
    {
      if (n_group_edge[i] != 0)
      {
        nodes_group = 0;
        for (int j = 0; j < 8; ++j) {
          if (v_group[j] == i) {
            nodes_group++;
            vert_on_trig[ind_vert_on_trig] = j;
            ind_vert_on_trig++;
          }
        }


        index_cont = 0;

    	//*********************************************************
    	//MODIFICAR AS VARIÁVEIS npoints e points:
        //- ACRESCENTAR AO NÚMERO TOTAL DE PONTOS A QUANTIDADE DE PONTOS ADICIONAIS  E CARREGAR O VETOR npoints
        // COM AS COORDENADAS X, Y, Z DOS PONTOS ADICIONAIS
        // - UTILIZAR CRITÉRIO DA MENOR DISTÂNCIA PARA DETERMINAR OS GRUPOS DOS NOVOS PONTOS
        //- AS COORDENADAS DEVEM SER BASEADAS EM UM CUBO UNITÁRIO COM V0 = (0,0,0) E V6 = (1,1,1)
    	//*********************************************************

        npoints = n_group_edge[i] + nodes_group;
        points = new float[3*npoints] ;


        for (int j = 0; j < 8; ++j) {
          if (v_group[j] == i) {
            points[3*index_cont]     = vert_coord[j][0];
            points[3*index_cont + 1] = vert_coord[j][1];
            points[3*index_cont + 2] = vert_coord[j][2];
            index_cont++;
          }
        }

        for (int j = 0; j < n_group_edge[i]; ++j) {

          points[3*index_cont    ] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][0]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][0])/ 2;
          points[3*index_cont + 1] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][1]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][1])/ 2;
          points[3*index_cont + 2] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][2]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][2])/ 2;

          index_cont++;
        }

        Chull3D * poly = new Chull3D(points, npoints);
        poly->compute();

        delete [] points;

        int nPoint = poly->get_n_vertices();
        float *vertices = new float[3*nPoint];

        int n_trig_ch = poly->get_n_faces();
        int * triangles = new int[3*n_trig_ch];

        poly->get_convex_hull(&vertices, &nPoint, &triangles, &n_trig_ch);

        ch_element_index = new char[3*n_trig_ch];
        ch_vert_edge_index = new int [3*n_trig_ch];

        for (int t = 0; t < n_trig_ch; ++t)
        {

            // Aqui utilizo a soma das coordenadas de um ponto para determinar se ele é um vértice do cubo ou o ponto médio de uma das arestas
            // Se a soma das coordenadas for um número inteiro se trata de um vértice do cubo, caso contrário, será o ponto médio de uma das arestas
            // aqui, deve ser incluída uma condicão paa verificar se é um ponto sobre uma das faces.
            // Nesta nova condição se atribuiria ch_element_index[3*t + v] = 'f'; e um critério para determinar sobre qual face o ponto está
          for(int v = 0; v < 3; ++ v)
          {
            sum_coord = vertices[3*triangles[3*t + v]] + vertices[3*triangles[3*t + v]+1] + vertices[3*triangles[3*t + v]+2];
            if(sum_coord == floor(sum_coord))
            {
              ch_element_index[3*t + v] = 'v';
              ch_vert_edge_index[3*t + v] = transform_index[(int)(vertices[3*triangles[3*t+ v]] + vertices[3*triangles[3*t+ v]+1]*2 + vertices[3*triangles[3*t + v]+2]*4)];
            }
            else
            {
              ch_element_index[3*t + v] = 'e';
              ch_vert_edge_index[3*t + v] = get_edge(vertices[3*triangles[3*t + v]], vertices[3*triangles[3*t + v]+1], vertices[3*triangles[3*t + v]+2]);
            }
          }
        }


        delete [] vertices;
        delete [] triangles;

        char s[3];
        int v[3];
        int cont = 0;
        int cont_e = 0;
        int cont_v = 0;
        int c_face = 0;

        for(int k = 0; k < n_trig_ch; ++k )
        {
          s[0] = ch_element_index[3*k];
          s[1] = ch_element_index[3*k + 1];
          s[2] = ch_element_index[3*k + 2];

          v[0] = ch_vert_edge_index[3*k];
          v[1] = ch_vert_edge_index[3*k + 1];
          v[2] = ch_vert_edge_index[3*k + 2];


          c_face = 0;


          // aqui estou passando pela triangulação gerada e detectando os triângulos coplanares às faces do cubo, para serem removidos da triangulação final
          // Quando um triângulo com alguns de seus vértices sobre uma face do cubo será coplanar a esta face?
          for (int i = 0; i < n_face; ++i)
          {
            cont = 0;
            cont_e = 0;
            cont_v = 0;

            for (int w = 0; w < 3; ++w)
            {
              if (s[w] == 'e')
              {
                for (int j = 0; j < n_edges_p_face; ++j)
                  if (v[w] == face_edges[i][j])
                    cont_e++;
              }
              else
              {
                for (int j = 0; j < n_edges_p_face; ++j)
                  if (v[w] == face_nodes[i][j])
                    cont_v++;
              }
            }

            cont = cont_e + cont_v;
            if(cont != 3) continue;
            if (cont == 3)
            {
              c_face = 1;
              break;
            }

          }

          if (c_face == 0)
          {

            snap_mesh_element.push_back(s[0]);
            snap_mesh_element.push_back(s[1]);
            snap_mesh_element.push_back(s[2]);

            snap_mesh_index.push_back(v[0]);
            snap_mesh_index.push_back(v[1]);
            snap_mesh_index.push_back(v[2]);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;
          }
        }

        delete[] ch_element_index;
        delete[] ch_vert_edge_index;

      }
    }
  }
  else {
    if (cont_zero_vert >= 3) {

      int get_zero_verts = 0;
      int get_pos_verts = 0;
      int get_neg_verts = 0;

      //int * _index;

      int n_vert_t = 0;

      if(cont_zero_vert >=5)
      {
        get_zero_verts = 1;
        n_vert_t = cont_zero_vert;
      }
      else
      {

        if(cont_pos_vert >= cont_neg_vert)
        {
          get_pos_verts = 1;
          n_vert_t = cont_zero_vert + cont_pos_vert;

        }
        else
        {
          get_neg_verts = 1;
          n_vert_t = cont_zero_vert + cont_neg_vert;

        }
      }

      //_index = new int[n_vert_t];
      points = new float[3*(n_vert_t)] ;
      npoints = n_vert_t;

      index_cont = 0;

      if(get_zero_verts == 1)
      {
        for (int i = 0; i < 8; ++i) {
          if (_cube[i] == 0.0) {

            points[3*index_cont    ] = vert_coord[i][0];
            points[3*index_cont + 1] = vert_coord[i][1];
            points[3*index_cont + 2] = vert_coord[i][2];

            index_cont++;

          }
        }
      }
      if(get_pos_verts == 1)
      {
        for (int i = 0; i < 8; ++i) {
          if (_cube[i] >= 0.0) {
            points[3*index_cont    ] = vert_coord[i][0];
            points[3*index_cont + 1] = vert_coord[i][1];
            points[3*index_cont + 2] = vert_coord[i][2];

            index_cont++;
          }
        }
      }
      if(get_neg_verts == 1)
      {
        for (int i = 0; i < 8; ++i) {
          if (_cube[i] <= 0.0) {
            points[3*index_cont    ] = vert_coord[i][0];
            points[3*index_cont + 1] = vert_coord[i][1];
            points[3*index_cont + 2] = vert_coord[i][2];

            index_cont++;
          }
        }
      }

      Chull3D * poly = new Chull3D(points, npoints);
      poly->compute();

      int nPoint = poly->get_n_vertices();
      float *vertices = new float[3*nPoint];

      int n_trig_ch = poly->get_n_faces();
      int * triangles = new int[3*n_trig_ch];

      poly->get_convex_hull(&vertices, &nPoint, &triangles, &n_trig_ch);

      ch_element_index = new char[3*n_trig_ch];
      ch_vert_edge_index = new int[3*n_trig_ch];

      for (int t = 0; t < n_trig_ch; ++t)
      {
        for(int v = 0; v < 3; ++ v)
        {
          ch_element_index[3*t + v] = 'v';
          ch_vert_edge_index[3*t + v] = transform_index[(int)(vertices[3*triangles[3*t+ v]] + vertices[3*triangles[3*t+ v]+1]*2 + vertices[3*triangles[3*t + v]+2]*4)];
        }
      }

      delete [] vertices;
      delete [] triangles;

      char s[3];
      int v[3];

      int cont = 0;
      int cont_e = 0;
      int cont_v = 0;

      int c_face = 0;

      for(int k = 0; k < n_trig_ch; ++k )
      {
        s[0] = ch_element_index[3*k];
        s[1] = ch_element_index[3*k + 1];
        s[2] = ch_element_index[3*k + 2];

        v[0] = ch_vert_edge_index[3*k];
        v[1] = ch_vert_edge_index[3*k + 1];
        v[2] = ch_vert_edge_index[3*k + 2];

        c_face = 0;

        for (int i = 0; i < n_face; ++i)
        {
          cont = 0;
          cont_e = 0;
          cont_v = 0;

          for (int w = 0; w < dim; ++w)
          {
            if (s[w] == 'e')
            {
              for (int j = 0; j < n_edges_p_face; ++j)
                if (v[w] == face_edges[i][j])
                  cont_e++;
            }
            else
            {
              for (int j = 0; j < n_edges_p_face; ++j)
                if (v[w] == face_nodes[i][j])
                  cont_v++;
            }
          }

          cont = cont_e + cont_v;
          if(cont != 3) continue;
          if (cont == dim)
          {
            c_face = 1;
            break;
          }

        }

        if (c_face == 0)
        {

          snap_mesh_element.push_back(s[0]);
          snap_mesh_element.push_back(s[1]);
          snap_mesh_element.push_back(s[2]);

          snap_mesh_index.push_back(v[0]);
          snap_mesh_index.push_back(v[1]);
          snap_mesh_index.push_back(v[2]);

          snap_mesh_cube.push_back(_i);
          snap_mesh_cube.push_back(_j);
          snap_mesh_cube.push_back(_k);

          n_trig++;
        }
      }

      delete[] ch_element_index;
      delete[] ch_vert_edge_index;

    }
  }

}
//===================================================================================================
void on_face_triangulation(int *verts, int n_vert)
{
  if(n_vert == 3)
  {

    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');

    snap_mesh_index.push_back(verts[0]);
    snap_mesh_index.push_back(verts[1]);
    snap_mesh_index.push_back(verts[2]);

    snap_mesh_cube.push_back(_i);
    snap_mesh_cube.push_back(_j);
    snap_mesh_cube.push_back(_k);

    n_trig++;
  }

  if(n_vert == 4)
  {

    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');

    snap_mesh_index.push_back(verts[0]);
    snap_mesh_index.push_back(verts[1]);
    snap_mesh_index.push_back(verts[2]);

    snap_mesh_cube.push_back(_i);
    snap_mesh_cube.push_back(_j);
    snap_mesh_cube.push_back(_k);

    n_trig++;

    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');
    snap_mesh_element.push_back('v');

    snap_mesh_index.push_back(verts[0]);
    snap_mesh_index.push_back(verts[1]);
    snap_mesh_index.push_back(verts[2]);

    snap_mesh_cube.push_back(_i);
    snap_mesh_cube.push_back(_j);
    snap_mesh_cube.push_back(_k);

    n_trig++;
  }

}

//===================================================================================================
void tunnel_triangulation()
{
  float sum_coord = 0;
  //int edge = 0;
  int n_edge_group = 0;

  int index_cont = 0;
  for (int i = 0; i < n_groups; ++i)
    n_edge_group += n_group_edge[i];

  int pos_nodes = 0;
  for (int i = 0; i < 8; ++i)
    if (_cube[i] == 0.0)
      pos_nodes++;



	//*********************************************************
	//MODIFICAR AS VARIÁVEIS npoints e points:
    //- ACRESCENTAR AO NÚMERO TOTAL DE PONTOS A QUANTIDADE DE PONTOS ADICIONAIS  E CARREGAR O VETOR npoints
    // COM AS COORDENADAS X, Y, Z DOS PONTOS ADICIONAIS
    //- AS COORDENADAS DEVEM SER BASEADAS EM UM CUBO UNITÁRIO COM V0 = (0,0,0) E V6 = (1,1,1)
	//*********************************************************
  	int npoints = n_edge_group + pos_nodes;
    float *points = new float[3*npoints] ;



    for (int i = 0; i < 8; ++i)
    {
      if (_cube[i] == 0.0)
      {

        points[3*index_cont    ] = vert_coord[i][0];
        points[3*index_cont + 1] = vert_coord[i][1];
        points[3*index_cont + 2] = vert_coord[i][2];

        index_cont++;
      }
    }

    for (int i = 0; i < n_groups; ++i)
    {
      if (n_group_edge[i] != 0)
      {
        for (int j = 0; j < n_group_edge[i]; ++j)
        {
          points[3*index_cont    ] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][0]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][0])/ 2;
          points[3*index_cont + 1] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][1]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][1])/ 2;
          points[3*index_cont + 2] = (vert_coord[edge_nodes[group_of_edges[i][j]][0]][2]+ vert_coord[edge_nodes[group_of_edges[i][j]][1]][2])/ 2;

          index_cont++;

        }
      }
    }
    //----------------------------------------------------------------------------------------------
    Chull3D * poly = new Chull3D(points, npoints);
    poly->compute();

    int nPoint = poly->get_n_vertices();
    float *vertices = new float[3*nPoint];

    int n_trig_ch = poly->get_n_faces();
    int * triangles = new int[3*n_trig_ch];

    poly->get_convex_hull(&vertices, &nPoint, &triangles, &n_trig_ch);


    char *ch_element_index = new char[3*n_trig_ch];
    int *ch_vert_edge_index = new int [3*n_trig_ch];

    for (int t = 0; t < n_trig_ch; ++t)
    {
      for(int v = 0; v < 3; ++ v)
      {
        sum_coord = vertices[3*triangles[3*t + v]] + vertices[3*triangles[3*t + v]+1] + vertices[3*triangles[3*t + v]+2];

        // Aqui utilizo a soma das coordenadas de um ponto para determinar se ele é um vértice do cubo ou o ponto médio de uma das arestas
        // Se a soma das coordenadas for um número inteiro se trata de um vértice do cubo, caso contrário, será o ponto médio de uma das arestas
        // aqui, deve ser incluída uma condicão paa verificar se é um ponto sobre uma das faces.
        // Nesta nova condição se atribuiria ch_element_index[3*t + v] = 'f'; e um critério para determinar sobre qual face o ponto está
        if(sum_coord == floor(sum_coord))
        {
          ch_element_index[3*t + v] = 'v';
          ch_vert_edge_index[3*t + v] = transform_index[(int)(vertices[3*triangles[3*t+ v]] + vertices[3*triangles[3*t+ v]+1]*2 + vertices[3*triangles[3*t + v]+2]*4)];
        }
        else
        {
          ch_element_index[3*t + v] = 'e';
          ch_vert_edge_index[3*t + v] = get_edge(vertices[3*triangles[3*t + v]], vertices[3*triangles[3*t + v]+1], vertices[3*triangles[3*t + v]+2]);
        }
      }
    }

    delete [] vertices;
    delete [] triangles;

    char s[3];
    int v[3];
    int cont = 0;
    int closing_face = 0;

    for (int k = 0; k < n_trig_ch; ++k)
    {

      s[0] = ch_element_index[3*k];
      s[1] = ch_element_index[3*k + 1];
      s[2] = ch_element_index[3*k + 2];

      v[0] = ch_vert_edge_index[3*k];
      v[1] = ch_vert_edge_index[3*k + 1];
      v[2] = ch_vert_edge_index[3*k + 2];



      // aqui estou passando pela triangulação gerada e verificando se um dado triângulo deve ser removido, segundo os critérios descritos no artigo.
      // uma nova condição deve ser incluída, pois agora existem triângulos com vértices sobre as faces do cubo. Quando um triângulo com essa característica deve ser removido da triangulação final
      closing_face = 0;
      for (int i = 0; i < n_groups; ++i)
      {
        cont = 0;
        if (n_group_edge[i] != 0)
        {
          for (int w = 0; w < 3; ++w)
          {

            if (s[w] == 'e')
            {
              for (int j = 0; j < n_group_edge[i]; ++j)
              {

                if (v[w] == group_of_edges[i][j])
                  cont++;
              }
            }
            else
            {
              for (int j = 0; j < 8; j++)
              {
                if ((v[w] == j) && (_cube[j] == 0.0) && (v_group[j] == i))
                  cont++;
              }
            }
          }

          if (cont == 3)
          {
            closing_face = 1;
            break;
          }
        }
      }
      if (closing_face == 0)
      {
        snap_mesh_element.push_back(s[0]);
        snap_mesh_element.push_back(s[1]);
        snap_mesh_element.push_back(s[2]);

        snap_mesh_index.push_back(v[0]);
        snap_mesh_index.push_back(v[1]);
        snap_mesh_index.push_back(v[2]);

        snap_mesh_cube.push_back(_i);
        snap_mesh_cube.push_back(_j);
        snap_mesh_cube.push_back(_k);

        n_trig++;
      }
    }

    int common_node = 0;
    int opposite = 0;
    if ((real_n_groups == 2)&&(case_== 13))
    {
      for (int i = 0; i < n_groups; ++i)
      {
        if (n_group_edge[i] == 3)
        {
          if (cube_sinal > 0)
          {
            if (_cube[edge_nodes[group_of_edges[i][0]][0]] > 0)
              common_node = edge_nodes[group_of_edges[i][0]][0];
            else
              common_node = edge_nodes[group_of_edges[i][0]][1];
          }

          else
          {
            if (_cube[edge_nodes[group_of_edges[i][0]][0]] < 0)
              common_node = edge_nodes[group_of_edges[i][0]][0];
            else
              common_node = edge_nodes[group_of_edges[i][0]][1];
          }

          opposite = diagonal_opposite[common_node];

          break;
        }
      }

      snap_mesh_element.push_back('e');
      snap_mesh_element.push_back('e');
      snap_mesh_element.push_back('e');

      snap_mesh_index.push_back(ev_connectivity[opposite][0]);
      snap_mesh_index.push_back(ev_connectivity[opposite][1]);
      snap_mesh_index.push_back(ev_connectivity[opposite][2]);

      snap_mesh_cube.push_back(_i);
      snap_mesh_cube.push_back(_j);
      snap_mesh_cube.push_back(_k);

      n_trig++;

    }


    delete[] ch_element_index;
    delete[] ch_vert_edge_index;
}
//===================================================================================================
void one_inter_point_triangulation()
{

  int intersec_edge[2];
  int cont_edge,cont_z_vert, cont1, cont2;
  int zero_v[2] = {-1,-1};

  for (int i = 0; i < n_face; ++i)
  {
    if (amb_face[i] == 1)
    {
      if (_cube[face_nodes[i][0]] >= 0)
      {
        if (face_critical_sinal[i] > 0)
        {
          if (real_n_groups == 1)
          {

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][0]);
            snap_mesh_index.push_back(face_edges[i][1]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;


            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][2]);
            snap_mesh_index.push_back(face_edges[i][3]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;
          }

          if (real_n_groups == 2)
          {
            for (int j = 0; j < n_groups; ++j)
            {
              if (n_group_edge[j] == 3)
              {
                cont1 = 0;
                cont2 = 0;
                for (int w = 0; w < 3; ++w)
                {
                  if (face_edges[i][0]
                        == group_of_edges[j][w])
                    cont1++;
                  if (face_edges[i][2]
                        == group_of_edges[j][w])
                    cont2++;
                }
              }
            }

            if (cont1 == 0)
            {
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][0]);
              snap_mesh_index.push_back(face_edges[i][1]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;

            }

            if (cont2 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][2]);
              snap_mesh_index.push_back(face_edges[i][3]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;

            }
          }
        }
        else
        {
          if (real_n_groups == 1)
          {

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][3]);
            snap_mesh_index.push_back(face_edges[i][0]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][1]);
            snap_mesh_index.push_back(face_edges[i][2]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;

          }

          if (real_n_groups == 2)
          {
            for (int j = 0; j < n_groups; ++j)
            {
              if (n_group_edge[j] == 3)
              {
                cont1 = 0;
                cont2 = 0;
                for (int w = 0; w < 3; ++w)
                {
                  if (face_edges[i][0]
                        == group_of_edges[j][w])
                    cont1++;
                  if (face_edges[i][2]
                        == group_of_edges[j][w])
                    cont2++;
                }
              }
            }

            if (cont1 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][3]);
              snap_mesh_index.push_back(face_edges[i][0]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;
            }

            if (cont2 == 0)
            {
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][1]);
              snap_mesh_index.push_back(face_edges[i][2]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;
            }

          }
        }
      }
      else
      {
        if (face_critical_sinal[i] > 0)
        {
          if (real_n_groups == 1)
          {

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][3]);
            snap_mesh_index.push_back(face_edges[i][0]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][1]);
            snap_mesh_index.push_back(face_edges[i][2]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;

          }
          if (real_n_groups == 2)
          {
            for (int j = 0; j < n_groups; ++j)
            {
              if (n_group_edge[j] == 3)
              {
                cont1 = 0;
                cont2 = 0;
                for (int w = 0; w < 3; ++w)
                {
                  if (face_edges[i][0]
                        == group_of_edges[j][w])
                    cont1++;
                  if (face_edges[i][2]
                        == group_of_edges[j][w])
                    cont2++;
                }
              }
            }

            if (cont1 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][3]);
              snap_mesh_index.push_back(face_edges[i][0]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;
            }

            if (cont2 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][1]);
              snap_mesh_index.push_back(face_edges[i][2]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;
            }

          }
        }
        else
        {
          if (real_n_groups == 1)
          {

            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][0]);
            snap_mesh_index.push_back(face_edges[i][1]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;


            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('e');
            snap_mesh_element.push_back('i');

            snap_mesh_index.push_back(face_edges[i][2]);
            snap_mesh_index.push_back(face_edges[i][3]);
            snap_mesh_index.push_back(1);

            snap_mesh_cube.push_back(_i);
            snap_mesh_cube.push_back(_j);
            snap_mesh_cube.push_back(_k);

            n_trig++;
          }
          if (real_n_groups == 2)
          {
            for (int j = 0; j < n_groups; ++j)
            {
              if (n_group_edge[j] == 3)
              {
                cont1 = 0;
                cont2 = 0;
                for (int w = 0; w < 3; ++w)
                {
                  if (face_edges[i][0]
                        == group_of_edges[j][w])
                    cont1++;
                  if (face_edges[i][2]
                        == group_of_edges[j][w])
                    cont2++;
                }
              }
            }

            if (cont1 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][0]);
              snap_mesh_index.push_back(face_edges[i][1]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;
            }

            if (cont2 == 0)
            {

              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('e');
              snap_mesh_element.push_back('i');

              snap_mesh_index.push_back(face_edges[i][2]);
              snap_mesh_index.push_back(face_edges[i][3]);
              snap_mesh_index.push_back(1);

              snap_mesh_cube.push_back(_i);
              snap_mesh_cube.push_back(_j);
              snap_mesh_cube.push_back(_k);

              n_trig++;


            }

          }
        }
      }

    }
    else
    {
      cont_edge = 0;
      for (int j = 0; j < n_edges_p_face; ++j)
      {
        if (_cube[edge_nodes[face_edges[i][j]][0]]
              * _cube[edge_nodes[face_edges[i][j]][1]] < 0)
        {
          intersec_edge[cont_edge] = j;
          cont_edge++;
        }
      }

      if (cont_edge == 2)
      {

        snap_mesh_element.push_back('e');
        snap_mesh_element.push_back('e');
        snap_mesh_element.push_back('i');

        snap_mesh_index.push_back(face_edges[i][intersec_edge[0]]);
        snap_mesh_index.push_back(face_edges[i][intersec_edge[1]]);
        snap_mesh_index.push_back(1);

        snap_mesh_cube.push_back(_i);
        snap_mesh_cube.push_back(_j);
        snap_mesh_cube.push_back(_k);

        n_trig++;

      }
      if (cont_edge == 1)
      {
        cont_z_vert = 0;
        for (int j = 0; j < n_edges_p_face; ++j)
        {
          if ((_cube[face_nodes[i][j]] == 0.0)&&(v_group[face_nodes[i][j]] != -1))
          {
            zero_v[cont_z_vert] = face_nodes[i][j];
            cont_z_vert++;
          }
        }

        if(cont_z_vert == 1)
        {
          snap_mesh_element.push_back('e');
          snap_mesh_element.push_back('v');
          snap_mesh_element.push_back('i');

          snap_mesh_index.push_back(face_edges[i][intersec_edge[0]]);
          snap_mesh_index.push_back(zero_v[0]);
          snap_mesh_index.push_back(1);

          snap_mesh_cube.push_back(_i);
          snap_mesh_cube.push_back(_j);
          snap_mesh_cube.push_back(_k);

          n_trig++;

        }
        if(cont_z_vert == 2)
        {

          snap_mesh_element.push_back('e');
          snap_mesh_element.push_back('v');
          snap_mesh_element.push_back('i');

          snap_mesh_index.push_back(face_edges[i][intersec_edge[0]]);
          snap_mesh_index.push_back(zero_v[0]);
          snap_mesh_index.push_back(1);

          snap_mesh_cube.push_back(_i);
          snap_mesh_cube.push_back(_j);
          snap_mesh_cube.push_back(_k);

          n_trig++;

          snap_mesh_element.push_back('e');
          snap_mesh_element.push_back('v');
          snap_mesh_element.push_back('i');

          snap_mesh_index.push_back(face_edges[i][intersec_edge[0]]);
          snap_mesh_index.push_back(zero_v[1]);
          snap_mesh_index.push_back(1);

          snap_mesh_cube.push_back(_i);
          snap_mesh_cube.push_back(_j);
          snap_mesh_cube.push_back(_k);

          n_trig++;


        }
      }
    }
  }

  if (real_n_groups == 2)
    for (int j = 0; j < n_groups; ++j)
      if (n_group_edge[j] == 3)
      {

        snap_mesh_element.push_back('e');
        snap_mesh_element.push_back('e');
        snap_mesh_element.push_back('e');

        snap_mesh_index.push_back(group_of_edges[j][0]);
        snap_mesh_index.push_back(group_of_edges[j][1]);
        snap_mesh_index.push_back(group_of_edges[j][2]);

        snap_mesh_cube.push_back(_i);
        snap_mesh_cube.push_back(_j);
        snap_mesh_cube.push_back(_k);

        n_trig++;
      }

}
//===================================================================================================
int find_topology()
{
  case_ = 0;

/*  int on_face_vert[4] = {-1,-1,-1,-1};

  int n_zero_v = 0;
  int neighbor_id;

  if((cont_zero_vert == 3)||(cont_zero_vert == 4))
  {
    if(((cont_pos_vert+cont_zero_vert)==8)||((cont_neg_vert+cont_zero_vert)== 8))
    {
      for(int i = 0; i < n_face; ++ i)
      {
        n_zero_v = 0;
        for(int j = 0; j < n_edges_p_face; ++j)
        {
          if(_cube[face_nodes[i][j]] == 0.0)
          {
            on_face_vert[n_zero_v] = face_nodes[i][j];
            n_zero_v++;
          }

        }
        if(n_zero_v >= 3)
        {
          neighbor_id = (_i + face_neighbor_id[i][0]) + (_j + face_neighbor_id[i][1])*size_x + (_k + face_neighbor_id[i][2])*size_x*size_y;

          if((visited_cube[neighbor_id] == 0)||(visited_cube[neighbor_id] == -1))
          {
            interior_topology = ON_FACE;
            on_face_triangulation(on_face_vert, n_zero_v);
            return 0;
          }

        }
      }

    }
  }*/
  //-----------------------------------------------------------------------------------------------

  if (n_amb_face == 0)
  {
    if (real_n_groups == 2)
    {
      if (cube_sinal == 1)
        interior_test_pos();
      if (cube_sinal == -1)
        interior_test_neg();

      case_ = 4;
    }
    else
    {

      interior_topology = LEAF;
      case_ = 0;
    }
  }

  if (n_amb_face == 1)
  {
    if (cont_face_sinal == 1)
    {

      interior_topology = LEAF;
      case_ = 6;
    }
    if (cont_face_sinal == 0)
    {
      int_ambiguity = 1;
      if (cube_sinal == 1)
        interior_test_pos();
      if (cube_sinal == -1)
        interior_test_neg();
    }
  }

  if (n_amb_face == 3)
  {
    case_ = 7;

    if (cont_face_sinal == 0)
      interior_topology = LEAF;

    if (cont_face_sinal == 1)
      interior_topology = LEAF;

    if (cont_face_sinal == 2)
      interior_topology = ADD_INT_P;

    if (cont_face_sinal == 3)
    {
      int_ambiguity = 1;
      if (cube_sinal == 1)
      {
        interior_test_pos();

      }
      if (cube_sinal == -1)
      {
        interior_test_neg();

      }
    }

  }

  if (n_amb_face == 2)
  {

    if (cont_face_sinal == 2)
    {
      int_ambiguity = 1;
      if (cube_sinal == 1)
        interior_test_pos();
      if (cube_sinal == -1)
        interior_test_neg();
    }

    if (cont_face_sinal == 1)
    {
      interior_topology = ADD_INT_P;
      add_interior_point = 1;
    }
    if (cont_face_sinal == 0)
    {
      int_ambiguity = 1;
      if (cube_sinal == 1)
        interior_test_pos();
      if (cube_sinal == -1)
        interior_test_neg();
    }
  }

  if (n_amb_face == 6)
  {
    case_ = 13;
    if (cont_face_sinal == 0)
      interior_topology = LEAF;
    if (cont_face_sinal == 6)
    {
      interior_topology = LEAF;
    }

    if (cont_face_sinal == 1)
      interior_topology = LEAF;
    if (cont_face_sinal == 5)
    {
      interior_topology = LEAF;
    }

    if (cont_face_sinal == 2)
      interior_topology = ADD_INT_P;
    if (cont_face_sinal == 4)
    {
      interior_topology = ADD_INT_P;
    }

    if (cont_face_sinal == 3)
    {
      if (real_n_groups == 1)
      {
        interior_topology = ADD_INT_P;
      }
      else
      {
        interior_test_pos();
      }
    }

  }
  if(verification == 1)
  {
    if(interior_topology == TUNNEL) return 0;
    else return 1;
  }

  switch (interior_topology)
  {
  case LEAF:
    leaf_triangulation();
    break;

  case TUNNEL:
    tunnel_triangulation();
    break;

  case ADD_INT_P:
    one_inter_point_triangulation();
    break;

  }
  return 0;
}
//===================================================================================================
void find_v_groups_pos()
{
  int aux_cont_group;
  int cont_group = 0;
  int cont_neighbor = 0;
  int just_added = -1;

  ngroups = 0;

  for (int i = 0; i < n_vert; ++i)
  {
    if ((i > 0) && (_cube[i] < 0.0) && (just_added == 0))
    {
      cont_group++;
      ngroups++;
      just_added = 1;
      continue;
    }
    if ((_cube[i] >= 0.0) && (v_group[i] == -1))
    {
      if (i > 0)
      {

        for (int j = 0; j < n_v_connct; ++j)
        {
          if ((_cube[v_connectivity[i][j]] >= 0)
                && (v_group[v_connectivity[i][j]] != -1))
          {
            v_group[i] = v_group[v_connectivity[i][j]];
            cont_neighbor++;
          }
        }
      }

      if (cont_neighbor != 0)
      {
        cont_neighbor = 0;
        continue;
      }
      else
      {

        cont_group++;
        ngroups++;
        just_added = 0;
        v_group[i] = cont_group;

        for (int j = 0; j < n_v_connct; ++j)
        {
          if (_cube[v_connectivity[i][j]] >= 0)
          {
            v_group[v_connectivity[i][j]] = cont_group;
            cont_neighbor++;
          }
        }

        if (cont_neighbor == 0)
        {
          cont_group++;
          ngroups++;
          just_added = 1;
        }
        else
          cont_neighbor = 0;
      }

      continue;
    }
    if ((_cube[i] >= 0.0) && (v_group[i] != -1))
    {
      aux_cont_group = v_group[i];

      for (int j = 0; j < n_v_connct; ++j)
      {
        if (_cube[v_connectivity[i][j]] >= 0)
        {
          v_group[v_connectivity[i][j]] = aux_cont_group;
          for (int w = 0; w < n_v_connct; ++w)
          {
            if (_cube[v_connectivity[v_connectivity[i][j]][w]] >= 0)
            {
              v_group[v_connectivity[v_connectivity[i][j]][w]] = aux_cont_group;
            }

          }
        }

      }
    }

  }
}
//===================================================================================================
void find_v_groups_neg()
{
  int aux_cont_group;
  int cont_group = 0;
  int cont_neighbor = 0;
  int just_added = -1;

  ngroups = 0;

  for (int i = 0; i < n_vert; ++i)
  {
    if ((i > 0) && (_cube[i] > 0.0) && (just_added == 0))
    {
      cont_group++;
      ngroups++;
      just_added = 1;
      continue;
    }
    if ((_cube[i] <= 0.0) && (v_group[i] == -1))
    {
      if (i > 0)
      {

        for (int j = 0; j < n_v_connct; ++j)
        {
          if ((_cube[v_connectivity[i][j]] <= 0)
                && (v_group[v_connectivity[i][j]] != -1))
          {
            v_group[i] = v_group[v_connectivity[i][j]];

            cont_neighbor++;
          }
        }
      }

      if (cont_neighbor != 0)
      {
        cont_neighbor = 0;
        continue;
      }
      else
      {

        cont_group++;
        ngroups++;
        just_added = 0;
        v_group[i] = cont_group;

        for (int j = 0; j < n_v_connct; ++j)
        {
          if (_cube[v_connectivity[i][j]] <= 0)
          {
            v_group[v_connectivity[i][j]] = cont_group;

            cont_neighbor++;
          }
        }

        if (cont_neighbor == 0)
        {
          cont_group++;
          ngroups++;
          just_added = 1;
        }
        else
          cont_neighbor = 0;
      }

      continue;
    }
    if ((_cube[i] <= 0.0) && (v_group[i] != -1))
    {
      aux_cont_group = v_group[i];

      for (int j = 0; j < n_v_connct; ++j)
      {
        if (_cube[v_connectivity[i][j]] <= 0)
        {
          v_group[v_connectivity[i][j]] = aux_cont_group;
          for (int w = 0; w < n_v_connct; ++w)
          {
            if (_cube[v_connectivity[v_connectivity[i][j]][w]] <= 0)
            {
              v_group[v_connectivity[v_connectivity[i][j]][w]] = aux_cont_group;
            }

          }
        }

      }
    }

  }
}
//===================================================================================================
void join_v_groups_by_face_pos()
{

  int pos_node[2] =
    { -1, -1 };
  int pos_node_cont = 0;
  for (int i = 0; i < n_face; ++i)
  {

    if ((amb_face[i] == 1) && (face_critical_sinal[i] > 0))
    {
      pos_node_cont = 0;
      pos_node[0] = pos_node[1] = 0;

      for (int j = 0; j < 4; ++j)
      {
        if (_cube[face_nodes[i][j]] > 0)
        {
          pos_node[pos_node_cont] = face_nodes[i][j];
          pos_node_cont++;
        }
      }

      if (v_group[pos_node[0]] != v_group[pos_node[1]])
      {
        int vg_1 = v_group[pos_node[0]];
        int vg_2 = v_group[pos_node[1]];
        ngroups++;

        for (int j = 0; j < n_vert; ++j)
        {
          if ((v_group[j] == vg_1) || (v_group[j] == vg_2))
          {
            v_group[j] = ngroups;
          }
        }

      }

    }
  }

}
//===================================================================================================
void join_v_groups_by_face_neg()
{
  int neg_node[2] =
    { -1, -1 };
  int neg_node_cont = 0;

  for (int i = 0; i < n_face; ++i)
  {

    if ((amb_face[i] == 1) && (face_critical_sinal[i] < 0))
    {
      neg_node_cont = 0;
      neg_node[0] = neg_node[1] = 0;

      for (int j = 0; j < 4; ++j)
      {
        if (_cube[face_nodes[i][j]] < 0)
        {
          neg_node[neg_node_cont] = face_nodes[i][j];
          neg_node_cont++;
        }
      }

      if (v_group[neg_node[0]] != v_group[neg_node[1]])
      {
        int vg_1 = v_group[neg_node[0]];
        int vg_2 = v_group[neg_node[1]];
        ngroups++;

        for (int j = 0; j < n_vert; ++j)
        {
          if ((v_group[j] == vg_1) || (v_group[j] == vg_2))
          {
            v_group[j] = ngroups;
          }
        }

      }

    }
  }
}
//===================================================================================================
void find_e_groups_pos()
{
  for (int i = 0; i < n_edge; i++)
  {
    if (((_cube[edge_nodes[i][0]] > 0) && (_cube[edge_nodes[i][1]] < 0)) || ((_cube[edge_nodes[i][0]] < 0) && (_cube[edge_nodes[i][1]] > 0)))
    {
      if (_cube[edge_nodes[i][0]] > 0)
        e_group[i] = v_group[edge_nodes[i][0]];
      else
        e_group[i] = v_group[edge_nodes[i][1]];
    }
  }
}
//===================================================================================================
void find_e_groups_neg()
{
  for (int i = 0; i < n_edge; i++)
  {
    if (((_cube[edge_nodes[i][0]] > 0) && (_cube[edge_nodes[i][1]] < 0)) || ((_cube[edge_nodes[i][0]] < 0) && (_cube[edge_nodes[i][1]] > 0)))
    {
      if (_cube[edge_nodes[i][0]] < 0)
        e_group[i] = v_group[edge_nodes[i][0]];
      else
        e_group[i] = v_group[edge_nodes[i][1]];
    }
  }

}
//===================================================================================================
void groups_of_edges()
{
  int _i = 0;
  int alread_used[12] =
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

  for (int i = 0; i < n_edge; ++i)
  {
    if ((e_group[i] != -1) && (alread_used[i] == -1))
    {
      group_of_edges[e_group[i]][n_group_edge[e_group[i]]] = i;
      alread_used[i] = 1;
      n_group_edge[e_group[i]]++;

      _i = i;
      int cont = 0;

      while (cont < 12)
      {
        for (int j = 0; j < n_e_connect; ++j)
        {
          if ((e_group[e_connectivity[_i][j]] == e_group[_i])
                && (alread_used[e_connectivity[_i][j]] == -1))
          {
            group_of_edges[e_group[_i]][n_group_edge[e_group[_i]]] =
              e_connectivity[_i][j];
            alread_used[e_connectivity[_i][j]] = 1;
            n_group_edge[e_group[_i]]++;

            _i = e_connectivity[_i][j];
          }
        }
        cont++;
      }
    }
  }

  real_n_groups = 0;

  for (int i = 0; i < n_groups; ++i)
  {
    if (n_group_edge[i] != 0)
      real_n_groups++;

  }
}

//===================================================================================================
void read_triangulation()
{

  char s[3];
  int v[3];
  float t;
  int i_, j_, k_;

  float x, y, z;
  float vx[3], vy[3], vz[3];
  float a, b;
  int cont_good_tric;

  final_nTrig = 0;

  if(!_x.empty())
  {
    _x.erase(_x.begin(),_x.end());
    _y.erase(_y.begin(),_y.end());
    _z.erase(_z.begin(),_z.end());
  }

  for(int trig = 0; trig < n_trig; trig++)
  {

    s[0] = snap_mesh_element[3*trig];
    v[0] = snap_mesh_index[3*trig];
    s[1] = snap_mesh_element[3*trig + 1];
    v[1] = snap_mesh_index[3*trig + 1];
    s[2] = snap_mesh_element[3*trig + 2];
    v[2] = snap_mesh_index[3*trig + 2];

    i_= snap_mesh_cube[3*trig];
    j_= snap_mesh_cube[3*trig + 1];
    k_= snap_mesh_cube[3*trig + 2];


    cont_good_tric = 0;
    for (int i = 0; i < 3; ++i)
    {
      if (s[i] == 'e')
      {
        switch (edge_nodes[v[i]][0])
        {
        case 0:
          a = get_data(i_, j_, k_) - isovalue;
          break;

        case 1:
          a = get_data(i_, j_, k_ + 1) - isovalue;
          break;

        case 2:
          a = get_data(i_, j_ + 1, k_ + 1) - isovalue;
          break;

        case 3:
          a = get_data(i_, j_ + 1, k_)  - isovalue;
          break;

        case 4:
          a = get_data(i_ + 1, j_, k_)  - isovalue;
          break;

        case 5:
          a = get_data(i_ + 1, j_, k_+1)  - isovalue;
          break;

        case 6:
          a = get_data(i_ + 1, j_ +1, k_+1)  - isovalue;
          break;

        case 7:
          a = get_data(i_ + 1, j_ + 1, k_) - isovalue;
          break;
        }

        switch (edge_nodes[v[i]][1])
        {

        case 0:
          b = get_data(i_, j_, k_) - isovalue;
          break;

        case 1:
          b = get_data(i_, j_, k_ + 1) - isovalue;
          break;

        case 2:
          b = get_data(i_, j_ + 1, k_ + 1) - isovalue;
          break;

        case 3:
          b = get_data(i_, j_ + 1, k_) - isovalue;
          break;

        case 4:
          b = get_data(i_ + 1, j_, k_) - isovalue;
          break;

        case 5:
          b = get_data(i_ + 1, j_, k_+1) - isovalue;
          break;

        case 6:
          b = get_data(i_ + 1, j_ +1, k_+1) - isovalue;
          break;

        case 7:
          b = get_data(i_ + 1, j_ + 1, k_) - isovalue;
          break;
        }

        t = -a / (b - a);

        switch (v[i])
        {
        case 0:
          y = ( j_ * sy);
          z = ( i_ * sx);
          x = (sz * size_z - ( k_ * sz + t * sz));
          break;

        case 1:
          y = ( j_ * sy + t * sy);
          z = ( i_ * sx);
          x = ( sz * size_z - (k_ * sz + sz));
          break;

        case 2:
          y = (  j_ * sy + sy);
          z = (  i_ * sx);
          x = (  sz * size_z -( k_ * sz + t * sz));
          break;

        case 3:
          y = ( j_ * sy + t * sy);
          z = ( i_ * sx);
          x = ( sz * size_z -( k_ * sz));
          break;

        case 4:
          y = ( j_ * sy);
          z = ( i_ * sx + sx);
          x = ( sz * size_z - (k_ * sz + t * sz));
          break;

        case 5:
          y = (  j_ * sy + t * sy);
          z = (  i_ * sx + sx);
          x = ( sz * size_z - (k_ * sz + sz));
          break;

        case 6:
          y = (j_ * sy + sy);
          z = (i_ * sx + sx);
          x = ( sz * size_z -( k_ * sz + t * sz));
          break;

        case 7:
          y = (  j_ * sy + t * sy);
          z = (  i_ * sx + sx);
          x = ( sz * size_z - ( k_ * sz));
          break;

        case 8:
          y = ( j_ * sy);
          z = ( i_ * sx + t * sx);
          x = (sz * size_z -( k_ * sz));
          break;

        case 9:
          y = (  j_ * sy);
          z = (  i_ * sx + t * sx);
          x = ( sz * size_z - ( k_ * sz + sz));
          break;

        case 10:
          y = ( j_ * sy + sy);
          z = ( i_ * sx + t * sx);
          x = ( sz * size_z - (k_ * sz + sz));
          break;

        case 11:
          y = (  j_ * sy + sy);
          z = (  i_ * sx + t * sx);
          x = ( sz * size_z -  (k_ * sz));
          break;
        }
      }

      if (s[i] == 'v')
      {
        switch (v[i])
        {
        case 0:
          y = ( j_ * sy);
          z = ( i_ * sx);
          x = ( sz * size_z -  (k_ * sz));
          break;

        case 1:
          y = (  j_ * sy);
          z = (  i_ * sx);
          x = ( sz * size_z -  (k_ * sz + sz));
          break;

        case 2:
          y = (  j_ * sy + sy);
          z = (  i_ * sx);
          x = ( sz * size_z -  (k_ * sz + sz));
          break;

        case 3:
          y = ( j_ * sy + sy);
          z = ( i_ * sx);
          x = ( sz * size_z -  (k_ * sz));
          break;

        case 4:

          y = (j_ * sy);
          z = (i_ * sx + sx);

          x = ( sz * size_z - (k_ * sz));

          break;

        case 5:
          y = (j_ * sy);
          z = (i_ * sx + sx);
          x = ( sz * size_z - (k_ * sz + sz));
          break;

        case 6:
          y = (j_ * sy + sy);
          z = (i_ * sx + sx);
          x = ( sz * size_z - (k_ * sz + sz));
          break;

        case 7:
          y = ( j_ * sy + sy);
          z = ( i_ * sx + sx);
          x = ( sz * size_z - (k_ * sz));
          break;
        }
      }
      if (s[i] == 'i')
      {

        y = (j_ * sy + sy / 2.0);
        z = (i_ * sx + sx / 2.0);
        x = ( sz * size_z - (k_ * sz + sz / 2.0));

      }

      if((x == x)&&(y == y)&&(z == z))
      {
        vx[i] = x;
        vy[i] = y;
        vz[i] = z;
        ++cont_good_tric;
      }
    }

    if(cont_good_tric == 3)
    {
      for(int j = 0; j < 3; ++j)
      {
        _x.push_back(vx[j]);
        _y.push_back(vy[j]);
        _z.push_back(vz[j]);
      }

      final_nTrig++;

    }
  }
}

//======================================================================================================
void topology()
{

  cube_sinal = 1;

  if (cont_pos_vert + cont_zero_vert > 4) cube_sinal = -1 * cube_sinal;


  initializing_groups();
  find_amb_faces(_cube);
  cubeSinal();


  if (cube_sinal == 1)
  {
    find_v_groups_pos();
    join_v_groups_by_face_pos();
    find_e_groups_pos();
  }
  else
  {
    find_v_groups_neg();
    join_v_groups_by_face_neg();
    find_e_groups_neg();
  }

  groups_of_edges();
  find_topology();

}

void Octree::mesh(Octree* oct){
	for(int i =0;i<=oct->nivel;i++){
		printf("__");
	}
	printf("\n");
	if(oct->nivel == oct->nivel_max){
		if(oct->sinal == 1){
			for(int i =0;i<8;i++){
				_cube[i] = oct->cube[i];
				printf("%f ",_cube[i]);
			}
			cont_pos_vert = oct->cont_pos_vert_;
			cont_zero_vert = oct->cont_zero_vert_;
			cont_neg_vert = oct->cont_neg_vert_;
			_i = oct->V_0->x;
			_j = oct->V_0->y;
			_k = oct->V_0->z;
			printf("%d %d %d\n",cont_pos_vert,cont_zero_vert,cont_neg_vert);
			if((cont_pos_vert != 8)&&(cont_neg_vert != 8)) topology();
		}
	}
	else{
		mesh(oct->children[0]);
		mesh(oct->children[1]);
		mesh(oct->children[2]);
		mesh(oct->children[3]);
		mesh(oct->children[4]);
		mesh(oct->children[5]);
		mesh(oct->children[6]);
		mesh(oct->children[7]);
	}
}

//====================================================================================================
int run()
{
	e_group = new int[n_edge];

	group_of_edges = new int*[n_groups];
	for (int i = 0; i < n_groups; ++i)
		group_of_edges[i] = new int[3 * n_trig_p_group];

	group_trigs = new int*[n_groups];
	for (int i = 0; i < n_groups; ++i)
		group_trigs[i] = new int[3 * n_trig_p_group];

	n_group_edge = new int[n_groups];
	//================================================================================================

	n_trig = 0;
	//read_nhdr(grid_nhdr);

	//================================================================================================

	arquivo = abre_arquivo();
	fscanf(arquivo,"%d %d %d\n",&size_x,&size_y,&size_z);
	fscanf(arquivo,"%f\n",&isovalue);
	printf("%d %d %d %f\n",size_x,size_y,size_z,isovalue);
	f_grid_data = (float*) malloc(size_x*size_y*size_z*sizeof(float));
	/*
	matriz = (float***) malloc(size_z * sizeof(m));
	for(int i =0;i<size_z+1;i++){
		matriz[i] = (float**) malloc(size_x * size_y * sizeof(isovalue));
	}
	 */
	float mat[size_y][size_x][size_z];

	int indice =0;
	for(int i=0;i<size_x;i++){
		printf("%d\n",i);
		for(int j=0;j<size_y;j++){
			printf("[");
			for(int k=0;k<size_z;k++){
				fscanf(arquivo,"%f",&mat[i][j][k]);
				f_grid_data[indice] = mat[i][j][k];
				//printf("%f ",mat[i][j][k]);
				printf("%f ",f_grid_data[indice]);
				indice++;
			}
			printf("]\n");
		}
	}printf("------------------------\n");

	//================================================================================================


	//int size = size_x*size_y*size_z;
	//visited_cube = new int [size];
	//for(int i = 0; i < size; ++i)
	//	visited_cube[i] = -1;


	printf("ExtendedMC33:: Draw triangulation...");

	Octree* oct = new Octree(0,0,0,size_x,size_y,size_z,nullptr,isovalue,0,pot_2(fmax(size_x,fmax(size_y,size_z))));
	oct->sinaliza_octree(oct);
	oct->imprime_octree(oct);
	oct->mesh(oct);

	/*
	//	*********************************************************
	//MODIFICAR A FORMA COMO O GRID É PERCORRIDO
	//	*********************************************************
	for (_k = 0; _k < size_z - 1; _k++)
		for (_j = 0; _j < size_y - 1; _j++)
			for (_i = 0; _i < size_x - 1; _i++)
			{

				cont_pos_vert = 0;
				cont_zero_vert = 0;
				cont_neg_vert = 0;


				//visited_cube[_i + _j*size_x + _k*size_x*size_y] = 0;


								_cube[0] = get_data(_i, _j, _k) - isovalue;

								if(_cube[0] > 0.0)
									++cont_pos_vert;
								if(_cube[0] < 0.0)
									++ cont_neg_vert;
								if (_cube[0] == 0.0) cont_zero_vert++;


								_cube[1] = get_data(_i, _j, _k + 1) - isovalue;
								if(_cube[1] > 0.0)
									++cont_pos_vert;
								if(_cube[1] < 0.0)
									++ cont_neg_vert;
								if (_cube[1] == 0.0) cont_zero_vert++;


								_cube[2] = get_data(_i, _j +1, _k+1) - isovalue;
								if(_cube[2] > 0.0)
									++cont_pos_vert;
								if(_cube[2] < 0.0)
									++ cont_neg_vert;
								if (_cube[2] == 0.0) cont_zero_vert++;


								_cube[3] = get_data(_i, _j + 1, _k) - isovalue;
								if(_cube[3] > 0.0)
									++cont_pos_vert;
								if(_cube[3] < 0.0)
									++ cont_neg_vert;
								if (_cube[3] == 0.0) cont_zero_vert++;


								_cube[4] = get_data(_i + 1, _j, _k)- isovalue;
								if(_cube[4] > 0.0)
									++cont_pos_vert;
								if(_cube[4] < 0.0)
									++ cont_neg_vert;
								if (_cube[4] == 0.0) cont_zero_vert++;


								_cube[5] = get_data(_i+1, _j, _k+1)- isovalue;
								if(_cube[5] > 0.0)
									++ cont_pos_vert;
								if(_cube[5] < 0.0)
									++ cont_neg_vert;
								if (_cube[5] == 0.0) cont_zero_vert++;


								_cube[6] = get_data(_i+1, _j+1, _k+1) - isovalue;
								if(_cube[6] > 0.0)
									++ cont_pos_vert;
								if(_cube[6] < 0.0)
									++ cont_neg_vert;
								if (_cube[6] == 0.0) cont_zero_vert++;


								_cube[7] = get_data(_i+1, _j+1, _k) - isovalue;
								if(_cube[7] > 0.0)
									++cont_pos_vert;
								if(_cube[7] < 0.0)
									++ cont_neg_vert;
								if (_cube[7] == 0.0) cont_zero_vert++;



				if((cont_pos_vert != 8)&&(cont_neg_vert != 8)) topology();


	}*/

	printf("done!\n");

	printf("ExtendedMC33:: Writing mesh...");
	delete[]e_group;
	delete[] group_of_edges;
	delete[]group_trigs;
	delete[]n_group_edge;
	//delete[] visited_cube;

	snap_mesh_element.clear();
	snap_mesh_index.clear();
	snap_mesh_cube.clear();

	read_triangulation();

	write_mesh();

	_x.clear();
	_y.clear();
	_z.clear();

	printf("done!\n");


	return 0;
}
//===================================================================================================


int pot_2(int val){
	int aux = 1;
	if(val == 2){
		aux = 1;
	}else{
		aux = aux + pot_2(val/2);
	}
	return aux;
}


FILE* abre_arquivo(){
	FILE* fp;
	printf("Digite o nome da matriz fonte a ser lida:\n");
	scanf("%s",nome);
	fp = fopen(nome,"r");
	if(fp == NULL){
		printf("Arquivo inexistente.\n");
		return nullptr;
	}
	printf("Arquivo encontrado e aberto..\n");
	return fp;
}

void fecha_arquivo(FILE* file){
	fclose(file);
}



//===================================================================================================
int main(int argc, char **argv)
{
  printf("Extented Marching Cubes 33:\n");


   /* if(argc != 4)
	    {
	   printf("ExtendedMC33: Filename(.nhdr) isoval (float) lambda (float) OutputFileName (.off) \n");
	   return 1;
	    }

	char* filename    = argv[1];
	isovalue = atof(argv[2]);
	output_mesh_file = argv[4];*/

  	// /home/alexandre/input/luna.txt

    //char* filename    = "/home/alexandre/eclipse-workspace/Octree_MC/input/uva.txt";
	//isovalue = 48.5;
	//output_mesh_file = "/home/alexandre/eclipse-workspace/Extended_MC_/output/ct-chest-48-5.off";

  	output_mesh_file = "/home/alexandre/output/maca.off";


	run();
	fecha_arquivo(arquivo);
	return 0;

}




