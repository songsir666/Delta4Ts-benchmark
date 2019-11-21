NAME   = sed
INCL	= -I .
CXXFLAG = -fprofile-arcs -ftest-coverage
LINK    = gcc
LINKFLAG= -fprofile-arcs
OBJECT  = $(NAME).o
TARGET  = $(NAME).exe
%.o: %.c
	gcc $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c $(COMPILE_PARAMETERS) 
%.gcno: $.c
	gcc $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c
$(TARGET): $(OBJECT)
	$(LINK) -std=c99 $(LINKFLAG) -o $(TARGET) $(OBJECT) -lm
build-bt:
	cfe sed.c
	il-bt-2 sed.c
	gcc -g -o sed.int.exe $(COMPILE_PARAMETERS) sed.int.c -I$(ARISTOTLE_DIR)/headers/ -L$(ARISTOTLE_DIR)/lib/sunos5/ -lIPF_bt -lglobalfunc -lm
	gcc -g -o sed.bt.int.exe $(COMPILE_PARAMETERS) sed.int.c -I$(ARISTOTLE_DIR)/headers/ -L$(ARISTOTLE_DIR)/lib/sunos5/ -lIPF_bt -lglobalfunc -lm

build-st:
	cfe sed.c
	il-st-2 sed.c
	gcc -g -o sed.st.int.exe $(COMPILE_PARAMETERS) sed.int.c -I$(ARISTOTLE_DIR)/headers/ -L$(ARISTOTLE_DIR)/lib/sunos5/ -lIPF_st -lglobalfunc -lm

clean:
	rm -f *.o *.exe

clean-bt:	clean
	rm -f *.int.c

