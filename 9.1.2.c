#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define ELEM(mat,i,j) (mat->elem[i*mat->cols+j])
	
typedef struct{
	unsigned int rows;
	unsigned int cols;
	float *elem;
}MAT;	
 
  void swap(int *a,int *b ){
 	int x;
 	x=*a;
 	*a=*b;
 	*b=x;
 }
	
 void permutacia(int *arr,int size){
 	int i,s;
 	srand(time(NULL));
 	for(i=0;i<size-i;i++){
 		s=i+rand()%(size-1-i);
 		swap(arr+i,arr+s);
	 }
 }
 
 char mat_create_random_permutation(MAT *mat){
 	int i;
 	int n=mat->rows;
        int *perm = (int*)malloc(sizeof(int)*mat->rows);
        for(i=0;i<n;i++){
 		perm[i]=i;
	 }

        permutacia(perm,n);

 	for(i=0;i<mat->rows*mat->cols;i++){
        mat->elem[i]=0;
        }

        for(i=0;i<mat->rows;i++){
        mat->elem[i*mat->cols+perm[i]]=1;
        }

  return 0;
 }

 void mat_print(MAT *mat) {
    unsigned int i;
    for (i = 0; i < mat->rows*mat->cols; i++) {

        if (i%mat->cols == 0 && i > 0) {
            printf("\n");
        }

        if (mat->elem[i] < 0) {
            printf("%01.2f ", mat->elem[i]);
        }
        else {
            printf("% 02.2f ", mat->elem[i]);
        }
    }
    printf("\n");
}
	
 
 MAT *mat_create_with_type(unsigned int rows,unsigned int cols){
 MAT *mat=(MAT*)malloc(sizeof(MAT)*1);
 mat->rows=rows;
 mat->cols=cols;
 mat->elem=(float*)malloc(sizeof(float)*rows*cols);
 if(mat->elem==NULL){
 	free(mat);
 	return NULL;
 }
 return mat;
 }


MAT *mat_create_by_file(char *filename){
	char type[2];
	unsigned int dim[2];
	FILE *f;
	MAT *matica=NULL;
	f=fopen(filename,"rb");
	if(f==NULL){
		return NULL;
	}
	fread(type,sizeof(char),2,f);
	fread(dim,sizeof(unsigned int),2,f);
	matica=mat_create_with_type(dim[0],dim[1]);
	matica->elem=(float*)malloc(dim[0]*dim[1]*sizeof(float));
	fread(matica->elem,sizeof(float),dim[0]*dim[1],f);
	fclose(f);
	return matica;
}

char mat_save(MAT *mat,char *filename){
	FILE *f;
	f=fopen(filename,"wb");
	if(f==NULL){
		return 'e';
	}
	unsigned int dim[2];
	dim[0]=mat->rows;
	dim[1]=mat->cols;
	fwrite("M1",sizeof(char),2,f);
	fwrite(dim,sizeof(unsigned int),2,f);
	fwrite(mat->elem,sizeof(float),dim[0]*dim[1],f);
	fclose(f);
	return 'x';
}


void mat_destroy(MAT *mat){
	free(mat->elem);
	free(mat);
}


void mat_unit(MAT *mat){
	unsigned int i;
	for(i=0;i<mat->rows*mat->cols;i++){
		if(i%(mat->cols+1)==0){
			mat->elem[i]=1.0f;
		}
		else{
			mat->elem[i]=0.0f;
		}
	}
}


void mat_random(MAT *mat){
	unsigned int i;
	for(i=0;i<mat->rows*mat->cols;i++){
		float random=float(rand()) / float(RAND_MAX)*float(pow(-1, (rand() % 2)));
		mat->elem[i]=random;
	}
}


 int main(void){

    //set random generator
    srand(time(NULL));

    MAT *m = mat_create_with_type(5, 5);

    mat_print(m);
    printf("\n");

    mat_unit(m);

    mat_print(m);
    printf("\n");

    mat_random(m);

    mat_print(m);
    printf("\n");
    char filename[100] = { "agimat.bin" };
    mat_save(m, filename);
    MAT *m2 = mat_create_by_file(filename);
    mat_print(m2);
    printf("\n");
    printf("%f\n", ELEM(m, 1, 3));
 	return 0;
}