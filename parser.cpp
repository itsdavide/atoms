#include "parser.h"

using namespace std;

#define OUT (*out)

/* Function signatures needed to make the compiler happy  */
void *parserAlloc(void *(mallocProc)(size_t));
void parserFree(void *p, void (freeProc)(void *));
void parser(void *pParser, int token, string *str, Parser *p);

/* Constructor */
Parser::Parser(istream *in, ostream *out){
	Parser::in = in;
	Parser::out = out;
	logicRelations = bdd_true();
	/* Creates the lexer */
	lexer= new Lexer(in);
	/* Creates the parser */
	pParser = parserAlloc(malloc);
}

/* Destructor */
Parser::~Parser(){
	parserFree(pParser, free);
	if (lexer != NULL) delete lexer;
}

/* Executes the parsing of the input */
void Parser::parse(bdd &L){
	int token;
	string *lval;
	while((token = lexer->scan(lval)) != P_EOI){
		parser(pParser, token, lval, this);
	}
	// Indicates the parsing end
	parser(pParser, 0, NULL, this);
	L = logicRelations;
}

void Parser::insertEvent(string event){
	events.insert(pair<string, bdd>(event, bdd_true()));
}

void Parser::insertLogicRelation(bdd rel){
	logicRelations = logicRelations & rel;
}


void Parser::createsVariables(){
	int eventCount = 0;
	bdd_setvarnum(events.size());
	for (map<string, bdd>::iterator i = events.begin() ; i != events.end() ; i++)
		(*i).second = bdd_ithvar(eventCount++);
}

bdd Parser::getBdd(string event){
	return events[event];
}
