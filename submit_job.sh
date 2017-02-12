#PBS -N placas
#PBS -l nodes=1:ppn=8
#PBS -M a.velasquez12@unindes.edu.co
#PBS -m abe

module load rocks-openmpi_ib
cd $PBS_O_WORKDIR
NPROCS=`wc -l < $PBS_NODEFILE`
mpiexec -v -n $NPROCS ./placas
