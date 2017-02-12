EXECS=placas
MPICC?=mpicc

all: ${EXECS}

placas.pdf : grafica.py potencial.dat
	python grafica.py

potencial.dat : placas
	qsub submit_job.sh

placas : placas2.c
	${MPICC} -o placas -lm placas.c

clean :
	rm placas
	rm potencial.dat

