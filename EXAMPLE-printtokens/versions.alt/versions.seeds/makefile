####### Template
NAME   = print_tokens

####### Compiler
CXX     = gcc
CXXFLAG = -fprofile-arcs -ftest-coverage
INCL    = -I .
LIB     =
LINK    = gcc
LINKFLAG= -fprofile-arcs
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
	$(CXX) $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c
.c.gcno:
	$(CXX) $(CXXFLAG) -c $(INCL) -o $@ $< -include dumpHandler.c

####### Build rules
all: $(TARGET)
$(TARGET): $(OBJECT)
	$(LINK) $(LINKFLAG) -o $(TARGET) $(OBJECT)
clear:
	$(RM) $(RMFLAG)  $(INTER) *~ core a.out
