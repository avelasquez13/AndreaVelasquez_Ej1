#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(){

  int world_size, rank, source, destination;
  MPI_Request send_request, recv_request;
  MPI_Status status;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  float L = 5, l = 2, d = 1, h = 5.0/16, V0 = 100, N = 2*pow((L/h), 2);
  int n = 16;
  
  //inicializa la matriz
  int i, j, k;
  int z = n/world_size;
  int first_col = z*rank-1;
  int pos_placa1 = (int)((L/2-d/2)/h)-first_col;
  int pos_placa2 = (int)((L/2+d/2)/h)-first_col;

 
  //primer procesador
  if (rank==0){

    int m = z+1;
    double *matriz_mundo;
    matriz_mundo =  malloc(n*m*sizeof(double));

    for(i=0; i<n; i++){
      for(j=0; j<n; j++){
	matriz_mundo[n*i+j] = -1;
      }
    }

    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz_mundo[n*i+j] = 5;
      }
    }

    //fontera vertical
    for(i=0; i<n; i++){
      matriz_mundo[n*i] = 0;
    }

    //frontera horizontal
    for(j=0; j<m; j++){
      matriz_mundo[j] = 0;
      matriz_mundo[(n-1)+j] = 0;
    }

    //primera placa
    if(m>=(int)((L/2-d/2)/h)){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz_mundo[n*i+(int)((L/2-d/2)/h)] = -V0/2;
      }
    }
    
    //segunda placa
    if(m>=(int)((L/2+d/2)/h)){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz_mundo[n*i+(int)((L/2+d/2)/h)] = V0/2;
      }
    }

    


    double *matriz_inter;
    matriz_inter = (double*) malloc(n*(z+2)*sizeof(double));

	

		for (i=0;i<n;i++)
		{
			for (j=0; j<m;j++)
			{
				matriz_inter[n*i+j]=7;
			}
		}
    

    //recibe los datos de los demas procesadores y los mete a matriz_mundo
    int source;
    for(source=1; source<world_size-1; source++)
    {
      MPI_Recv(matriz_inter, (n*(z+2)), MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("recibio del procesador %d \n", source);  
      for(i=0; i<n; i++)
      {
				for(j=1; j<=z; j++)
				{
	  			matriz_mundo[n*i+z*source+j-1] = matriz_inter[n*i+j];
	  			printf("%f ", matriz_inter[n*i+j]);
				}printf("\n");
			}
    }


    double *matriz;
    matriz = (double*) malloc(n*m*sizeof(double));

   
    //recibe los datos del ultimo procesador y los mete a matriz_mundo
    MPI_Recv(matriz, n*m, MPI_DOUBLE, world_size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(i=0; i<n; i++){
	for(j=1; j<m; j++){
	  matriz_mundo[i*n+z*(world_size-1)+j-1] = matriz[i*n+j];
	}
      }

    //imprime la matriz mundo
    for(i=0; i<n; i++){
      for(j=0; j<n; j++){
	printf("%f ", matriz_mundo[i*n+j]);
      }printf("\n");
      }
    


  }


      

  //ultimo procesador
  else if(rank == world_size-1){
    
    int m = z+1;
    double *matriz;
    matriz = (double*) malloc(n*m*sizeof(double));


    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz[i*n+j] = 5;
      }
    }
    
    //fontera vertical
    for(i=0; i<n; i++){
      matriz[i*n+m-1] = 0;
    }
    
    //frontera horizontal
    for(j=0; j<m; j++){
      matriz[j] = 0;
      matriz[(n-1)*j] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0 && pos_placa1<m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i*n+pos_placa1] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2<m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i*n+pos_placa2] = V0/2;
      }
    }
    

    
    //manda la matriz a matriz_mundo
    MPI_Send(matriz, n*m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
   


  }
  






  //procesadores intermedios
  else{
    
    int m = z+2;
    double *matriz;
    matriz = (double*) malloc(n*m*sizeof(double));


    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz[i*n+j] = 5;
      }
    }
     
    //frontera horizontal
    for(j=0; j<m; j++){
      matriz[j] = 0;
      matriz[n-1+j] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0 && pos_placa1 < m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i*n+pos_placa1] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2 < m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i*n+pos_placa2] = V0/2;
      }
    }

    
    //manda la matriz a matriz_mundo
    MPI_Send(matriz, n*m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    

 }


  /*


  //inicializa la matriz temporal
  double **matriz2;
  matriz2 = (double**) malloc(n*sizeof(double*));

  for (i=0; i<=n; i++){
    matriz2[i] = (double*) malloc(n*sizeof(double));
  }

  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      matriz2[i][j] = 0;
    }
  }

  for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
    matriz2[i][(int)((L/2-d/2)/h)] = -V0/2;
    matriz2[i][(int)((L/2+d/2)/h)] = V0/2;
  }

  
  //metodo de relajacion
  for(k=0; k<N; k++){
    for(i=1; i<n-1; i++){
      for(j=1; j<n-1; j++){
	  matriz2[i][j]=0.25*(matriz[i+1][j]+matriz[i][j+1]+matriz[i-1][j]+matriz[i][j-1]);
      }
    }
    for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
      matriz2[i][(int)((L/2-d/2)/h)] = -V0/2;
      matriz2[i][(int)((L/2+d/2)/h)] = V0/2;
    }
    for(i=1; i<n-1; i++){
      for(j=1; j<n-1; j++){
	matriz[i][j] = matriz2[i][j];
      }
    }
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

  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      Ex[i][j]=(matriz[i][j]-matriz[i+1][j])/h;
      Ey[i][j]=-(matriz[i][j]-matriz[i][j+1])/h;
    }
  }


  //imprime los valores del potencial y del campo
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f\n", matriz[i][j]);
    }
  }
  
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f\n", Ex[i][j]);
    }
  }

  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f\n", Ey[i][j]);
    }
  }


  */

  //MPI_Wait(&send_request, &status);
  //MPI_Wait(&recv_request, &status);


  MPI_Finalize();
  return 0;

}
