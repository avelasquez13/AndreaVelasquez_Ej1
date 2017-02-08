#PBS -N test_scheduler
#PBS -l nodes=1:ppn=4
#PBS -M a.velasquez12@uniandes.edu.co
#PBS -m abe

module load rocks-openmpi_ib
cd $PBS_O_WORKDIR
NPROCS=`wc -l < $PBS_NODEFILE`
mpiexec -v -n $NPROCS ./placas
