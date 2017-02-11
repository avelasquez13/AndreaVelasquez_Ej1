#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* cuadradaALineal(double** mCuadrada, int n, int m);
double** linealACuadrada(double* mLineal, int n, int m);

int main()
{
	 int i,j,n=3,m=3;
	 
	 
	 double *matriz_l;
	 matriz_l=malloc(n*m*sizeof(double));
	 
	 double **matriz2;
   matriz2= (double**) malloc(n*sizeof(double*));
    
   for(i=0;i<n;i++)
   {
   	matriz2[i]=(double*) malloc(m*sizeof(double));
   }
	 
	 //Crea e inicializa matriz cuadrada
	 double **matriz;
   matriz= (double**) malloc(n*sizeof(double*));
    
   for(i=0;i<n;i++)
   {
   	matriz[i]=(double*) malloc(m*sizeof(double));
   }
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 matriz[i][j]=1+(i*n+j);
   	}
   }
   
   //imprime la matriz cuadrada
   printf("Imprimendo la matriz cuadrada\n");
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 printf("%f ",matriz[i][j]);
   	}
   	printf("\n");
   }
   
   //convierte la matriz cuadrada en lineal
   matriz_l=cuadradaALineal(matriz,n,m);
   
   //imprime la matriz lineal
   printf("Imprimendo la matriz lineal\n");
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 printf("%f ",matriz_l[i*n+j]);
   	}
   	printf("\n");
   }
   
   //convierte la matriz lineal a cuadrada
   	 
   matriz2=linealACuadrada(matriz_l,n,m);
   
   //Imprime la matriz 2 cuadrada
   printf("Imprimendo la matriz 2 cuadrada\n");
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 printf("%f ",matriz2[i][j]);
   	}
   	printf("\n");
   }
   
   
	return 0;
}

double* cuadradaALineal(double** mCuadrada, int n, int m)
{
	int i,j;
	double *mlineal;
	mlineal=malloc(n*m*sizeof(double));
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
		//printf("convirtiendo el %f\n",mCuadrada[i][j]);
		mlineal[i*n+j]=mCuadrada[i][j];
		}
	}

	return mlineal;
}

double** linealACuadrada(double* mLineal, int n, int m)
{
	int i,j;
	double **mCuadrada;
	mCuadrada= (double**) malloc(n*sizeof(double*));
    
   for(i=0;i<n;i++)
   {
   	mCuadrada[i]=(double*) malloc(m*sizeof(double));
   }
   
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
		mCuadrada[i][j]=mLineal[i*n+j];
		}
	}

	return mCuadrada;
}
