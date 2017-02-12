EXECS=placas
MPICC?=mpicc

all: ${EXECS}


placas : placas.c
	${MPICC} -o placas -lm placas.c

#potencial.dat : placas
#	qsub submit_job.sh

plot :
	cp placas.o* potencial.dat
	python grafica.py


clean :
	rm placas
	rm potencial.dat
	rm placas.o*
	rm placas.e*

