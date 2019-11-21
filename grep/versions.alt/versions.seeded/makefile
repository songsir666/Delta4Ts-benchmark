####### Template
NAME   = grep

####### Compiler
CXX     = gcc
CXXFLAG = -fprofile-arcs -ftest-coverage
INCL    = -I.
LIB     =
LINK    = gcc 
LINKFLAG= -fprofile-arcs -std=c99
GCOV    = gcov
GCOVFLAG= 

####### Tools
RM      = rm
RMFLAG  = -rf

####### Files

SOURCE  = $(NAME).c
OBJECT  = $(NAME).o
OUTPUT  = $(SOURCE).gcov

GCNO    = $(NAME).gcno
GCDA    = $(NAME).gcda
INTER   = $(GCDA) $(OUTPUT)
TARGET  = $(NAME).exe

####### Implicit rules
.SUFFIXES: c  .o .gcno

.c.o:
        $(CXX) $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c -lm
.c.gcno:
        $(CXX) $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c -lm

####### Build rules
all: $(TARGET)
$(TARGET): $(OBJECT)
        $(LINK) $(LINKFLAG) -o $(TARGET) $(OBJECT) -lm
clear:
        $(RM) $(RMFLAG) *.exe *.o $(INTER) *~ core a.out

