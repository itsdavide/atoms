%include {

#include <iostream>
#include <cassert>
#include <cstring>
#include "parser.h"

#define STRING(X) 				(new string(*X))
#define DELETE_PTR(X) 			{ if (X) delete X; }
#define DELETE_PTR_LIST(T, X) 	{ if (X) { for (T::iterator i = X->begin() ; i != X->end() ; i++) delete (*i); delete X; } }

using namespace std;

}

/* Specifies tokens' prefix and functions to call the parser */
%name parser
%token_prefix P_

/* Specifies an extra argument to the function passing tokens to the parser */
%extra_argument{ Parser *parser }

/* Specifies what to do in case of error */
%syntax_error {
	cerr << endl << "ERROR: syntax error!" << endl << endl;
	cerr << endl << "ERROR: syntax error at line = " << parser->getLexer()->getLine() << ", column = " << parser->getLexer()->getColumn();
	cerr << ", token = " << parser->getLexer()->getToken() << endl << endl;
	exit(1);
}

/* Specifies what to do in case of failure */
%parse_failure {
	cerr << endl << "FAILURE: parsing failure!" << endl << endl;
	exit(1);
}

/* Tokens' associativity */
%left BIIMP.
%left IMP.
%left OR.
%left AND.
%left NOT.
%nonassoc ERROR EOI.

/* Grammar's start symbol */
%start_symbol start


/* Specifies token's type and destructor */
%token_type 					{ string * }
%token_destructor 				{ DELETE_PTR($$); }


%type relation					{ bdd * }
%destructor	relations			{ DELETE_PTR($$); }


/**********************************************************************************************************************************/

/* Grammar */
start ::= events relations.

/* Events */

events ::= EVENTS eventList.								{ parser->createsVariables(); }

eventList ::= EVENT(event).									{ parser->insertEvent(*event); }
eventList ::= eventList EVENT(event).						{ parser->insertEvent(*event); }

/* Relations */

relations ::= RELATIONS relationList.						

relationList ::= .
relationList ::= relationList relation(rel).			{ parser->insertLogicRelation(*rel); }

relation(rel) ::= SURE.										{ rel = new bdd(bdd_true()); }
relation(rel) ::= IMPOSSIBLE.								{ rel = new bdd(bdd_false()); }
relation(rel) ::= EVENT(event).								{ rel = new bdd(parser->getBdd(*event)); }
relation(rel) ::= NOT relation(rel1).						{ rel = new bdd(!(*rel1)); }
relation(rel) ::= relation(rel1) AND relation(rel2).		{ rel = new bdd(*rel1 & *rel2); }
relation(rel) ::= relation(rel1) OR relation(rel2).			{ rel = new bdd(*rel1 | *rel2); }
relation(rel) ::= relation(rel1) IMP relation(rel2).		{ rel = new bdd(bdd_imp(*rel1, *rel2)); }
relation(rel) ::= relation(rel1) BIIMP relation(rel2).		{ rel = new bdd(bdd_biimp(*rel1, *rel2)); }
relation(rel) ::= LPARA relation(rel1) RPARA.				{ rel = new bdd(*rel1); }