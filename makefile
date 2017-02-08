EXECS=placas
MPICC?=mpicc

all: ${EXECS}

placas.pdf : grafica.py potencial.dat
	python grafica.py

potencial.dat : placas
	qsub submit_job.sh > potencial.dat

placas : placas.c
	${MPICC} -o placas -lm placas.c

clean :
	rm placas
	rm potencial.dat

