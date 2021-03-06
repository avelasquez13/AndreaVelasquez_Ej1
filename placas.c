#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double* cuadradaALineal(double** mCuadrada, int n, int m);
double** linealACuadrada(double* mLineal, int n, int m);

int main(){

  int world_size, rank, source, destination;
  MPI_Request send_request[2];
  MPI_Request recv_request[2];
  MPI_Status send_status[2];
  MPI_Status recv_status[2];

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  float L = 5, l = 2, d = 1, h = 5.0/256, V0 = 100, N = 2*pow((L/h), 2);
  int n = (int)L/h;
  
  //inicializa la matriz
  int i, j, k;
  int z = n/world_size;
  int first_col = z*rank-1;
  int pos_placa1 = (int)((L/2-d/2)/h)-first_col;
  int pos_placa2 = (int)((L/2+d/2)/h)-first_col;
 
  //--------------------primer procesador------------------------//
  if (rank==0){

    int m = z+1;
    double **matriz_mundo;
    matriz_mundo = (double**) malloc(n*sizeof(double*));

    for (i=0; i<=n; i++){
      matriz_mundo[i] = (double*) malloc(n*sizeof(double));
    }

    for(i=0; i<n; i++){
      for(j=0; j<n; j++){
			matriz_mundo[i][j] = -1;
      }
    }

    for(i=0; i<m; i++){
      for(j=0; j<n; j++){
	matriz_mundo[i][j] = 5;
      }
    }

    //frontera vertical
    for(i=0; i<m; i++){
      matriz_mundo[i][0] = 0;
      matriz_mundo[i][n-1] = 0;
    }

    //frontera horizontal
    for(j=0; j<n; j++){
      matriz_mundo[0][j] = 0;
    }

    //primera placa
    if(m>=(int)((L/2-d/2)/h)){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz_mundo[(int)((L/2-d/2)/h)][j] = -V0/2;
      }
    }
    //segunda placa
    if(m>=(int)((L/2+d/2)/h)){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz_mundo[(int)((L/2+d/2)/h)][j] = V0/2;
      }
    }
      
      //inicializa la matriz temporal
  double **matriz2;
  matriz2 = (double**) malloc(m*sizeof(double*));

  for (i=0; i<=m; i++){
    matriz2[i] = (double*) malloc(n*sizeof(double));
  }

  for(i=0; i<m; i++){
    for(j=0; j<n; j++){
      matriz2[i][j] = 0;
    }
  }

  //metodo de relajacion
  for(k=0; k<N; k++){
    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
	  matriz2[i][j]=0.25*(matriz_mundo[i+1][j]+matriz_mundo[i][j+1]+matriz_mundo[i-1][j]+matriz_mundo[i][j-1]);
      }
    }
      
    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
			matriz_mundo[i][j] = matriz2[i][j];
      }
    }
    
        //primera placa
    if(m>=(int)((L/2-d/2)/h)){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz_mundo[(int)((L/2-d/2)/h)][j] = -V0/2;
      }
      }
    //segunda placa
    if(m>=(int)((L/2+d/2)/h)){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz_mundo[(int)((L/2+d/2)/h)][j] = V0/2;
      }
    }
    
    //crea array del overlap enviado y recibido
    double *ol_siguiente;
    ol_siguiente=malloc(n*sizeof(double));
    ol_siguiente=matriz_mundo[m-1];
    
    double *ol_recibido;
    ol_recibido=malloc(n*sizeof(double));
    
    
    //envia y recibe el overlap
    MPI_Isend(ol_siguiente, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &send_request[1]);
    
    //printf("procesador 0 mando");
    MPI_Irecv(ol_recibido, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &recv_request[0]);
    
        	MPI_Wait(&send_request[1], &send_status[1]);
      MPI_Wait(&recv_request[0], &recv_status[0]);
       
    
    
    //guarda el ol recibido en la matriz_mundo
    for(j=0;j<n;j++){
    matriz_mundo[m-1][j]=ol_recibido[j];
    }
  }

   


    double **matriz_inter;
    matriz_inter = (double**) malloc((z+2)*sizeof(double*));

    
    for (i=0; i<=(z+2); i++){
      matriz_inter[i] = (double*) malloc(n*sizeof(double));
    }

    double *matriz_interlineal;
    matriz_interlineal = malloc(n*(m+1)*sizeof(double));

	

    for (i=0;i<m;i++){
	for (j=0; j<n;j++){
		matriz_interlineal[i*n+j]=7;
	}
    }
    
     
    //recibe los datos de los demas procesadores y los mete a matriz_mundo
    int source;
    for(source=1; source<world_size-1; source++)
    {
      MPI_Recv(matriz_interlineal, n*(z+2), MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      matriz_inter=linealACuadrada(matriz_interlineal,z+2,n);
      for(i=1; i<z+1; i++){
				for(j=0; j<n; j++){
					matriz_mundo[z*source+i-1][j] = matriz_inter[i][j];
				}
			}
    }

		double **matriz_rv;
    matriz_rv = (double**) malloc((z+1)*sizeof(double*));

    
    for (i=0; i<=(z+1); i++){
      matriz_rv[i] = (double*) malloc(n*sizeof(double));
    }

    double *matriz_linealrv;
    matriz_linealrv = malloc(n*m*sizeof(double));
   
    //recibe los datos del ultimo procesador y los mete a matriz_mundo
    MPI_Recv(matriz_linealrv, n*m, MPI_DOUBLE, world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    matriz_rv=linealACuadrada(matriz_linealrv,z+1,n);
    for(i=1; i<m; i++){
			for(j=0; j<n; j++){
	  	matriz_mundo[z*(world_size-1)+i-1][j] = matriz_rv[i][j];
			}
    }

    //imprime la matriz mundo (potencial)
    for(i=0; i<n; i++){
      for(j=0; j<n; j++){
				printf("%f ", matriz_mundo[i][j]);
      }printf("\n");
    }
    
    
      //calcula el campo electrico
  double **Ex;
  Ex = (double**) malloc(n*sizeof(double*));
  double **Ey;
  Ey = (double**) malloc(n*sizeof(double*));

  for (i=0; i<=n; i++){
    Ex[i] = (double*) malloc(n*sizeof(double));
    Ey[i] = (double*) malloc(n*sizeof(double));
  }

  for(i=0; i<n-1; i++){
    for(j=0; j<n-1; j++){
      Ey[i][j]=(matriz_mundo[i][j]-matriz_mundo[i+1][j])/h;
      Ex[i][j]=-(matriz_mundo[i][j]-matriz_mundo[i][j+1])/h;
    }
  }

  //imprime los valores del potencial y del campo
  
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f ", Ex[i][j]);
    }printf("\n");
  }

  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f ", Ey[i][j]);
    }printf("\n");
  }
    


  }

     

  //-----------------ultimo procesador-----------------------//
  else if(rank == world_size-1){
    
    int m = z+1;
    double **matriz;
    matriz = (double**) malloc(m*sizeof(double*));

    for (i=0; i<=m; i++){
      matriz[i] = (double*) malloc(n*sizeof(double));
    }

    for(i=0; i<m; i++){
      for(j=0; j<n; j++){
			matriz[i][j] = 5;
      }
    }
    
    //fontera vertical
    for(i=0; i<m; i++){
      matriz[i][0] = 0;
      matriz[i][n-1] = 0;
    }
    
    //frontera horizontal
    for(j=0; j<n; j++){
      matriz[m-1][j] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0 && pos_placa1<m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz[pos_placa1][j] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2<m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
	matriz[pos_placa2][j] = V0/2;
      }
    }
    
    
    //inicializa la matriz temporal
  double **matriz2;
  matriz2 = (double**) malloc(m*sizeof(double*));

  for (i=0; i<=m; i++){
    matriz2[i] = (double*) malloc(n*sizeof(double));
  }

  for(i=0; i<m; i++){
    for(j=0; j<n; j++){
      matriz2[i][j] = 0;
    }
  }

  //metodo de relajacion
  for(k=0; k<N; k++){
    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
	  matriz2[i][j]=0.25*(matriz[i+1][j]+matriz[i][j+1]+matriz[i-1][j]+matriz[i][j-1]);
      }
    }

      
    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
			matriz[i][j] = matriz2[i][j];
      }
    }
    
  //primera placa
    if(pos_placa1 >= 0 && pos_placa1<m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz[pos_placa1][j] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2<m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
			matriz[pos_placa2][j] = V0/2;
      }
    }
    
    //crea array del overlap enviado y recibido
    double *ol_anterior;
    ol_anterior=malloc(n*sizeof(double));
    ol_anterior=matriz[0];
    
    double *ol_recibido;
    ol_recibido=malloc(n*sizeof(double));
    
        
    //envia y recibe el overlap
    MPI_Isend(ol_anterior,n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &send_request[0]);
    MPI_Irecv(ol_recibido, n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &recv_request[1]);
    
    	MPI_Wait(&send_request[0], &send_status[0]);
	MPI_Wait(&recv_request[1], &recv_status[1]);
    


    
    //guarda el ol recibido en la matriz
    for(j=0;j<n;j++){
    matriz[0][j]=ol_recibido[j];
    }
    
  }
    
    
    
    double *matriz_linealsn;
    matriz_linealsn = malloc(n*m*sizeof(double));

		matriz_linealsn = cuadradaALineal(matriz,m,n);  
    
    //manda la matriz a matriz_mundo
    MPI_Send(matriz_linealsn, n*m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
   


  }
  






  //-----------------procesadores intermedios------------------//
  else{
    
    int m = z+2;
    double **matriz;
    matriz = (double**) malloc(m*sizeof(double*));

    for (i=0; i<=m; i++){
      matriz[i] = (double*) malloc(n*sizeof(double));
    }

    for(i=0; i<m; i++){
      for(j=0; j<n; j++){
	matriz[i][j] = 5;
      }
    }
     
    //fontera vertical
    for(i=0; i<m; i++){
      matriz[i][0] = 0;
      matriz[i][n-1] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0 && pos_placa1 < m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
	matriz[pos_placa1][j] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2 < m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
	matriz[pos_placa2][j] = V0/2;
      }
    }
    
    
    //inicializa la matriz temporal
  double **matriz2;
  matriz2 = (double**) malloc(m*sizeof(double*));

  for (i=0; i<=m; i++){
    matriz2[i] = (double*) malloc(n*sizeof(double));
  }

  for(i=0; i<m; i++){
    for(j=0; j<n; j++){
      matriz2[i][j] = 0;
    }
  }

  //metodo de relajacion
  for(k=0; k<N; k++){
    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
	  matriz2[i][j]=0.25*(matriz[i+1][j]+matriz[i][j+1]+matriz[i-1][j]+matriz[i][j-1]);
      }
    }

    for(i=1; i<m-1; i++){
      for(j=1; j<n-1; j++){
			matriz[i][j] = matriz2[i][j];
      }
    }
    
    //primera placa
    if(pos_placa1 >= 0 && pos_placa1 < m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
	matriz[pos_placa1][j] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2 < m){
      for(j=(int)((L/2-l/2)/h); j<(int)((L/2+l/2)/h); j++){
	matriz[pos_placa2][j] = V0/2;
      }
    }
      //crea arrays de overlaps enviados y recibidos
    double *ol_anterior_e;
    ol_anterior_e=malloc(n*sizeof(double));
    ol_anterior_e=matriz[0];
    
    double *ol_anterior_r;
    ol_anterior_r=malloc(n*sizeof(double));
    
    double *ol_siguiente_e;
    ol_siguiente_e=malloc(n*sizeof(double));
    ol_siguiente_e=matriz[m-1];
    
    double *ol_siguiente_r;
    ol_siguiente_r=malloc(n*sizeof(double));
    
    
    //envia y recibe los overlaps
    MPI_Isend(ol_anterior_e,n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &send_request[0]);
    MPI_Irecv(ol_anterior_r, n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &recv_request[0]);
    MPI_Isend(ol_siguiente_e,n, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &send_request[1]);
    MPI_Irecv(ol_siguiente_r, n, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &recv_request[1]);

    
    MPI_Wait(&send_request[0], &send_status[0]);
  	MPI_Wait(&recv_request[0], &recv_status[0]);
  	MPI_Wait(&send_request[1], &send_status[1]);
  	MPI_Wait(&recv_request[1], &recv_status[1]);
    
    //guarda los ol recibidos en la matriz
    for(j=0;j<n;j++){
    matriz[0][j]=ol_anterior_r[j];
    matriz[m-2][j]=ol_siguiente_r[j];
    }
  }
    
    
 		double *matriz_linealsn;
    matriz_linealsn = malloc(n*m*sizeof(double));

		matriz_linealsn = cuadradaALineal(matriz,m,n);  

    //manda la matriz a matriz_mundo
    MPI_Send(matriz_linealsn, n*m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    

 }


 

  
	if(rank==0){

	}
	else if(rank==world_size-1){

	}
  else{

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
		mlineal[i*m+j]=mCuadrada[i][j];
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
		mCuadrada[i][j]=mLineal[i*m+j];
		}
	}

	return mCuadrada;
}
