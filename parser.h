#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <utility>
#include "lexer.h"
#include "syntax.h"
#include "bdd.h"

class Parser{
	void *pParser;
	std::istream *in;
	std::ostream *out;
	std::map<std::string, bdd> events;
	bdd logicRelations;
	Lexer *lexer;
public:
	/* Costruttore */
	Parser(std::istream *in = &std::cin, std::ostream *out = &std::cout);
	/* Distruttore */
	~Parser();
	void parse(bdd &L);
	void insertEvent(std::string event);
	void insertLogicRelation(bdd rel);
	void createsVariables();
	bdd getBdd(std::string event);
	Lexer *getLexer(){return lexer;}
	std::map<std::string, bdd> getEvents(){return events;}
};

#endif
