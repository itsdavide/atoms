#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include "syntax.h"

class Lexer{
	std::istream *in;
	int bufmin;
	int bufsize;
	unsigned int line;
	char *buffer, *start, *offset, *cursor, *limit, *marker, *eof;
	void fill(int n);
	void step();
public:
	/* Costruttore */
	Lexer(std::istream *in = &std::cin);
	/* Distruttore */
	~Lexer();
	int scan(std::string *&val);
	void reset(std::istream *i = &std::cin);
	std::string getToken();
	int getLine();
	int getColumn();
};

#endif
