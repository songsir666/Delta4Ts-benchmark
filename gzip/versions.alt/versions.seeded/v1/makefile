CXXFLAG = -fprofile-arcs -ftest-coverage
INCL    = -I.
LINKFLAG= -fprofile-arcs

allfile: allfile.o
	gcc $(LINKFLAG) -o allfile allfile.o
#	rm FaultSeeds.h
	ln allfile gzip.exe

allfile.o: allfile.c crypt.h getopt.h gzip.h lzw.h revision.h tailor.h
	#touch FaultSeeds.h
	gcc $(CXXFLAG) -c $(INCL) -DSTDC_HEADERS=1 -DHAVE_UNISTD_H=1 -DDIRENT=1 -DHAVE_ALLOCA_H=1 ${COMPILE_PARAMETERS} allfile.c -o allfile.o -include dumpHandler.c

clean:
	rm -f allfile.o allfile allfile.c.inst.exe allfile.int.c gzip.exe

all: clean allfile
build: all
