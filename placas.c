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

  float L = 5, l = 2, d = 1, h = 5.0/512.0, V0 = 100, N = 2*pow((L/h), 2);
  int n = 64;
  
  //inicializa la matriz
  int i, j, k;
  int z = n/world_size;
  int first_col = z*rank-1;
  int pos_placa1 = (int)((L/2-d/2)/h)-first_col;
  int pos_placa2 = (int)((L/2+d/2)/h)-first_col;

  printf("Procesador # %d corriendo/n", rank);
  
  //primer procesador
  if (rank==0){
    int m = n/world_size+1;
    double **matriz;
    matriz = (double**) malloc(n*sizeof(double*));

    for (i=0; i<=n; i++){
      matriz[i] = (double*) malloc(m*sizeof(double));
    }

    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz[i][j] = 5;
      }
    }

    //fontera vertical
    for(i=0; i<n; i++){
      matriz[i][0] = 0;
    }

    //frontera horizontal
    for(j=0; j<m; j++){
      matriz[0][j] = 0;
      matriz[n-1][j] = 0;
    }

    //primera placa
    if(m>=(int)((L/2-d/2)/h)){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][(int)((L/2-d/2)/h)] = -V0/2;
      }
    }
    
    //segunda placa
    if(m>=(int)((L/2+d/2)/h)){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][(int)((L/2+d/2)/h)] = V0/2;
      }
    }


    //imprime la matriz
    printf("Imprimiendo desde el procesador %d\n", rank);
    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	printf("%f ", matriz[i][j]);
      }printf("\n");
    }


  }
  

  //ultimo procesador
  else if(rank == world_size-1){
    
    int m = n/world_size+1;
    double **matriz;
    matriz = (double**) malloc(n*sizeof(double*));

    for (i=0; i<=n; i++){
      matriz[i] = (double*) malloc(m*sizeof(double));
    }

    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz[i][j] = 5;
      }
    }

    /*//fontera vertical
    for(i=0; i<n; i++){
      matriz[i][m-1] = 0;
    }

    //frontera horizontal
    for(j=0; j<m; j++){
      matriz[0][j] = 0;
      matriz[n-1][j] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][pos_placa1] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][pos_placa2] = V0/2;
      }
      }*/

    //imprime la matriz
    printf("Imprimiendo desde el procesador %d\n", rank);
    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	printf("%f ", matriz[i][j]);
      }printf("\n");
    }

  }
  


  //procesadores intermedios
  else{
    
    int m = n/world_size+2;
    double **matriz;
    matriz = (double**) malloc(n*sizeof(double*));

    for (i=0; i<=n; i++){
      matriz[i] = (double*) malloc(m*sizeof(double));
    }

    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	matriz[i][j] = 5;
      }
    }
    
    //frontera horizontal
    for(j=0; j<m; j++){
      matriz[0][j] = 0;
      matriz[n-1][j] = 0;
    }

    //primera placa
    if(pos_placa1 >= 0 && pos_placa1 < m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][pos_placa1] = -V0/2;
      }
    }
    
    //segunda placa
    if(pos_placa2 >= 0 && pos_placa2 < m){
      for(i=(int)((L/2-l/2)/h); i<(int)((L/2+l/2)/h); i++){
	matriz[i][pos_placa2] = V0/2;
      }
    }
    
    //imprime la matriz
    printf("Imprimiendo desde el procesador %d\n", rank);
    for(i=0; i<n; i++){
      for(j=0; j<m; j++){
	printf("%f ", matriz[i][j]);
      }printf("\n");
    }

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

  MPI_Finalize();
  return 0;

}
