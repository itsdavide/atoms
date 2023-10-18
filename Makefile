CPP  = g++
CC   = gcc
OBJ  = main.o parser.o syntax.o lexer.o
LINKOBJ  = main.o buddy-main/build/src/libbuddy.a parser.o syntax.o lexer.o
BIN  = atoms
MV = mv
RM = rm -f

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN)
	
main.o: main.cpp buddy-main/build/src/libbuddy.a bdd.h atom.h syntax.o
	$(CPP) -c main.cpp -o main.o
	
parser.o: parser.cpp parser.h bdd.h lexer.h syntax.o
	$(CPP) -c parser.cpp -o parser.o
	
lexer.o: lexer.re lexer.h syntax.h
	re2c lexer.re > lexer.cpp
	$(CPP) -c lexer.cpp -o lexer.o
	
syntax.o: lemon syntax.y
	./lemon syntax.y
	$(RM) syntax.out
	$(MV) syntax.c syntax.cpp
	$(CPP) -c syntax.cpp -o syntax.o
	
lemon: lemon.c lempar.c
	$(CC) lemon.c -o lemon

clean:
	${RM} $(OBJ) $(BIN) lexer.cpp lemon syntax.cpp syntax.h
