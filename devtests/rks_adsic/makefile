
all: rks_calc lsd_calc
	@./compare.sh

rks_calc:
	cd rks && cp2k.popt test.inp > test.out

lsd_calc:
	cd lsd && cp2k.popt test.inp > test.out

clean:
	- rm ???/SICTest*
	- rm ???/*.out
