#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<fcntl.h>
#include<unistd.h>

#define ELEM(mat,i,j) (mat->elem[i*mat->cols+j])

#ifndef O_BINARY
#define O_BINARY 0
#endif

//struktura matice
typedef struct
{
	unsigned int rows;
	unsigned int cols;
	float *elem;
} MAT;

//funkcia na vymenu poloziek poli
void swap(int *a,int *b )
{
 	int x;
 	x=*a;
 	*a=*b;
 	*b=x;
}

//funkcia na generovanie permutacii
void permutacia(int *arr,int size)
{
 	int i,s;
 	
 	for(i=0;i<size-i;i++)
	 {
 		s=i+rand()%(size-1-i);
 		swap(arr+i,arr+s);
	 }
}

//generovanie nahodnej matice
char mat_create_random_permutation(MAT *mat)
{
 	unsigned int i;
 	unsigned int n=mat->rows;
 	int *perm;
 	
 	if(mat->rows!=mat->cols)
 	{	
 		return 't';
	}
 	
 	//allokacia pamate
    perm = (int*)malloc(sizeof(int)*mat->rows);
    
    //vygenerovanie indexov stlpcov jednotiek
    for(i=0;i<n;i++)
	{
 		perm[i]=i;
    }

    //vygenerovanie nahodnej permutacie
    permutacia(perm,n);

	//naplnenie matice s nulami
 	for(i=0;i<mat->rows*mat->cols;i++)
	{
        mat->elem[i]=0;
    }

	//zapis jednotiek podla permutacneho pola
    for(i=0;i<mat->rows;i++)
	{
        ELEM(mat,i,perm[i])=1;
    }

    return 'x';
}

//vypis matice
void mat_print(MAT *mat)
{
    unsigned int i;
    
    for (i = 0; i < mat->rows*mat->cols; i++)
	{
        if (i%mat->cols == 0 && i > 0)
		{
            printf("\n");
        }

        if (mat->elem[i] < 0)
		{
            printf("%01.2f ", mat->elem[i]);
        }
        else
		{
            printf("% 02.2f ", mat->elem[i]);
        }
    }
    
    printf("\n");
}

//vytvorenie prazdnej matice s rozmerom rows*cols
MAT *mat_create_with_type(unsigned int rows,unsigned int cols)
{
    MAT *mat=(MAT*)malloc(sizeof(MAT)*1);
    
    //v pripade neuspechu pamat sa uvolni
    if(mat==NULL)
	{
        free(mat);
        return NULL;
    }
    
    mat->rows=rows;
    mat->cols=cols;
    mat->elem=(float*)malloc(sizeof(float)*rows*cols);
    
    //v pripade neuspechu pamat sa uvolni
    if(mat->elem==NULL)
	{
        free(mat);
        return NULL;
    }
    
    
    return mat;
}

//uvolnenie pamate
void mat_destroy(MAT *mat)
{
	free(mat->elem);
	free(mat);
}

//nacitanie matice zo suboru
MAT *mat_create_by_file(char *filename)
{
	char type[2];
	unsigned int dim[2];
    int readresult;
    MAT *matica=NULL;

	//otvorenie suboru na binarne citanie
	int fd=open(filename,O_BINARY | O_RDONLY);
    if (fd==-1)
	{
        return NULL;
    }
    
    //citanie zo suboru
	readresult=read(fd,type,sizeof(char)*2);
	
	if (type[0]!='M' && type[1]!= '1')
	{
        return NULL;
    }
	
    if (readresult==-1)
	{
        return NULL;
    }
    
	readresult=read(fd,dim,sizeof(int)*2);
    if (readresult==-1)
	{
        return NULL;
    }
    
    //nacitanie prvkov matice
    matica=mat_create_with_type(dim[0],dim[1]);
    
	readresult=read(fd,matica->elem,sizeof(float)*dim[0]*dim[1]);
    if (readresult==-1)
	{
		mat_destroy(matica);
        return NULL;
    }
    
    //bezpecne zatvorenie suboru
    if(close(fd)==-1)
	{
		mat_destroy(matica);
        return NULL;
	}
       
	return matica;
}

//ulozenie matice do suboru
char mat_save(MAT *mat,char *filename)
{
	int writeresult;
	int fd=open(filename,O_BINARY|O_WRONLY|O_CREAT);
	
	if(fd==-1)
	{
        return 't';
	}

	unsigned int dim[2];
	dim[0]=mat->rows;
	dim[1]=mat->cols;
	
	//zapis do suboru
	writeresult=write(fd,"M1",sizeof(char)*2);
	if(writeresult==-1)
	{
        return 't';
	}
	
	writeresult=write(fd,dim,sizeof(int)*2);
	if(writeresult==-1)
	{
        return 't';
	}
	
	writeresult=write(fd,mat->elem,sizeof(float)*dim[0]*dim[1]);
	if(writeresult==-1)
	{
        return 't';
	}
	
	if(close(fd)==-1)
	{
        return 't';
	}
	return 'x';
}

//vytvorenie jednotkovej matice
void mat_unit(MAT *mat)
{
	unsigned int i;
	for(i=0;i<mat->rows*mat->cols;i++)
	{
		if(i%(mat->cols+1)==0)
		{
			mat->elem[i]=1.0f;
		}
		else
		{
			mat->elem[i]=0.0f;
		}
	}
}

//naplnenie matice s nahodnymi hodnotami
void mat_random(MAT *mat)
{
	unsigned int i;
	for(i=0;i<mat->rows*mat->cols;i++)
	{
		float random=(float)(rand()) / (float)(RAND_MAX)*(float)(pow(-1, (rand() % 2)));
		mat->elem[i]=random;
	}
}

int main(void)
{
    //nastavenie nahodneho generatora
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
    char filename[100] = { "matica.bin" };
    
    mat_save(m,filename);
    
    MAT *m2 =mat_create_by_file(filename);
    
    mat_print(m2);
    printf("\n");
    printf("%f\n", ELEM(m, 1, 3));
 	return 0;
}
