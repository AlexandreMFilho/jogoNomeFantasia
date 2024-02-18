#include"teste.h"
#include"Octree_.h"

int main(){

	arquivo = abre_arquivo();
	fscanf(arquivo,"%d %d %d\n",&size_x,&size_y,&size_z);
	fscanf(arquivo,"%f\n",&isovalue);
	printf("%d %d %d %f\n",size_x,size_y,size_z,isovalue);
	matriz = (float*) malloc(size_x*size_y*size_z*sizeof(float));
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
				matriz[indice] = mat[i][j][k];
				//printf("%f ",mat[i][j][k]);
				printf("%f ",matriz[indice]);
				indice++;
			}
			printf("]\n");
		}
	}printf("------------------------\n");

	//matriz = (float***)mat;
	//matriz = mat;

	// /home/alexandre/eclipse-workspace/Octree_/src/8x8x8.txt

	Octree* oct = new Octree(0,0,0,size_x,size_y,size_z,nullptr,isovalue,0,pot_2(fmax(size_x,fmax(size_y,size_z))));
	//oct->imprime_octree(oct);
	oct->sinaliza_octree(oct);
	printf("------------------------\n");
	//Octree* oct = new Octree(0,0,0,8,8,8,nullptr,0.5,0,8);
	oct->imprime_octree(oct);
	fecha_arquivo(arquivo);
	free(matriz);
	return 0;
}
