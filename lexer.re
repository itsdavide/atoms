#include <iostream>
#include <cstring>
#include <cstdlib>
#include "lexer.h"

using namespace std;

#define YYCTYPE         unsigned char
#define YYCURSOR        cursor
#define YYLIMIT         limit
#define YYMARKER        marker
#define YYFILL(n)       {fill(n);}
#define EOI -1
#define ERR 0

/* Constructor */
Lexer::Lexer(istream *in){
	Lexer::in = in;
	bufmin = 8192;
	bufsize = 0;
	buffer = start = offset = cursor = marker = limit = eof = 0;
	line = 1;
}

/* Destructor */
Lexer::~Lexer(){
}

string Lexer::getToken(){
	return string(start, cursor);
}

void Lexer::step(){
	offset = start;
	line++;
}

void Lexer::reset(istream *in){
	Lexer::in = in;
	start  = buffer;
	cursor = buffer;
	limit  = buffer;
	marker = buffer;
	eof    = 0;
	offset = buffer;
	line   = 1;
}

int Lexer::getLine(){
	return line;
}

int Lexer::getColumn(){
	return start - offset;
}

int Lexer::scan(string *&lval){
	//int nested = 0;
	lval = 0;
begin:
	start = cursor;
	/*!re2c
		ANY             = [\000-\377];
		WS              = [\t\r ]*;
		NL              = "\n";
		EVENT     		= [a-zA-Z_] [a-zA-Z0-9_]*;
		COMMENT         = "#";
		EVENTS			= "@events";
		RELATIONS		= "@relations";
		
		COMMENT         { goto comment; }
		WS              { goto begin; }
		NL              { if(eof == cursor) return P_EOI; step(); goto begin; }
		EVENTS			{ return P_EVENTS; }
		RELATIONS		{ return P_RELATIONS; }
		EVENT			{ lval = new std::string(start, cursor); return P_EVENT; }
		"<0>"			{ return P_IMPOSSIBLE; }
		"<1>"			{ return P_SURE; }
		"-"             { return P_NOT; }
		"*"				{ return P_AND; }
		"+"				{ return P_OR; }
		"<=>"			{ return P_BIIMP; }
		"=>"			{ return P_IMP; }
		"("             { return P_LPARA; }
		")"             { return P_RPARA; }
		ANY             { return P_ERROR; }
	*/
comment:
	/*!re2c
		NL				{ if(eof == cursor) return P_EOI; step(); goto begin; }
		ANY				{ goto comment;}
	*/
}

void Lexer::fill(int n){
	if(!eof){
		// make shure that the last token is completely stored in the buffer
		int shift = start - buffer;
		if(shift > 0){
			memmove(buffer, start, limit - start);
			start  = buffer;
			offset-= shift;
			marker-= shift;
			limit -= shift;
			cursor-= shift;
		}
		// make shure we can read at least max(n,bufmin) bytes
		int inc   = std::max(n, bufmin);
		if(bufsize < inc + (limit - buffer)){
			bufsize = inc + (limit - buffer);
			char *buf = (char*)realloc(buffer, bufsize * sizeof(char));
			start  = buf + (start - buffer);
			cursor = buf + (cursor - buffer);
			limit  = buf + (limit - buffer);
			marker = buf + (marker - buffer);
			offset = buf + (offset - buffer);
			buffer = buf;

		}
		// read new bytes from stream
		in->read(limit, inc);
		limit+= in->gcount();
		// handle eof specially: set last char to \n
		if(in->gcount() < inc){
			eof = limit;
			*eof++ = '\n';
		}
	}
}
