
all: rks_calc lsd_calc
	@./compare.sh

rks_calc:
	cd rks && /scratch/fuhlig/opt/cp2k/cp2k/exe/Linux-x86-64-gfortran-new/cp2k.popt test.inp &> test.out

lsd_calc:
	cd lsd && /scratch/fuhlig/opt/cp2k/cp2k/exe/Linux-x86-64-gfortran-new/cp2k.popt test.inp &> test.out

clean:
	- rm ???/SICTest*
	- rm ???/*.out
