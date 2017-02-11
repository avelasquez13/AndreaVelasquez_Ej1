#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<mpi.h>


double* cuadradaALineal(double** mCuadrada, int n, int m);
double** linealACuadrada(double* mLineal, int n, int m);

int main()
{

  int world_size, rank, source, destination;
  MPI_Request send_request, recv_request;
  MPI_Status status;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	 int i,j,n=3,m=3;
	 
	 if(rank==0)
	 {
	 //crea matriz lineal
	 double *matriz_l;
	 matriz_l=malloc(n*m*sizeof(double));
	 
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
   printf("Imprimendo la matriz lineal enviada\n");
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 printf("%f ",matriz_l[i*n+j]);
   	}
   	printf("\n");
   }
   
   //TODO envia la matriz lineal
    MPI_Send(matriz_l, n*m, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
   }
   else if(rank==1)
   {
   //crea la matriz lineal
   	 double *matriz_l2;
	 matriz_l2=malloc(n*m*sizeof(double));
	 
   //crea la matriz cuadrada
   	 double **matriz2;
   matriz2= (double**) malloc(n*sizeof(double*));
    
   for(i=0;i<n;i++)
   {
   	matriz2[i]=(double*) malloc(m*sizeof(double));
   }
	 
   //TODO recibe la matriz lineal
   MPI_Recv(matriz_l2,n*m,MPI_DOUBLE,1,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   
   //imprime la matriz lineal
   printf("Imprimendo la matriz lineal recibida\n");
   for(i=0; i<n;i++)
   {
   	for(j=0;j<m;j++)
   	{
   	 printf("%f ",matriz_l[i*n+j]);
   	}
   	printf("\n");
   }  	
   
   //convierte la matriz lineal a cuadrada   	 
   matriz2=linealACuadrada(matriz_l2,n,m);
   
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
   
   }
   
     MPI_Finalize();
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
		printf("convirtiendo el %f\n",mCuadrada[i][j]);
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
