#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pttv.h"

#define MAX_IDENT_LEN 10
#define MAX_NUMBER_LEN 6
#define KEYWORDS_COUNT 15

char Ch;
TokenType Token;
int	Num, lineNo, colNo;
char Id[MAX_IDENT_LEN + 1]; 
FILE *f;

TokenType checkKeyword(char *Id) {
	for (int i = 0; i < KEYWORDS_COUNT; i++) {
		if (strcmp(Id, keywords[i].string) == 0) {
			return keywords[i].Token;
		}
	}
	return IDENT;
}

char getCh() {
	int c = fgetc(f);
	putchar(c);
	colNo++;
	if (c == '\n') {
		lineNo++;
		colNo = 0;
	}
	return toupper(c);
}

TokenType getToken() {
	while (Ch == ' ' || Ch == '\n' || Ch == '\t') {
		Ch = getCh();
	}
	if (isalpha(Ch)) {
		int i = 0;
		do {
			if (i < MAX_IDENT_LEN) {
				Id[i] = Ch;
				i++;
			}
			Ch = getCh();
		} while (isalpha(Ch) || isdigit(Ch));
		Id[i] = '\0';
		
		return checkKeyword(Id);
	}
	else if (isdigit(Ch)) {
		Num = 0;
		while (isdigit(Ch)) {
			Num = Num*10 + (Ch - '0');
			Ch = getCh();
		}
		return NUMBER;
	}
	else if (Ch == ':') {
		Ch = getCh();
		if (Ch == '=') {
			Ch = getCh();
			return ASSIGN;
		}
		else return NONE;
	}
	else if (Ch == '=') { Ch = getCh(); return EQU; }
	else if (Ch == '+') { Ch = getCh(); return PLUS; }
	else if (Ch == '-') { Ch = getCh(); return MINUS; }
	else if (Ch == '*') { Ch = getCh(); return TIMES; }
	else if (Ch == '/') { Ch = getCh(); return SLASH; }
	else if (Ch == '<') { 
		Ch = getCh(); 
		if (Ch == '>') {
			Ch = getCh();
			return NEQ;
		}
		else if (Ch == '=') {
			Ch = getCh();
			return LEQ;
		}
		else return LSS; 
	}
	else if (Ch == '>') {
		Ch = getCh();
		if (Ch == '=') {
			Ch = getCh();
			return GEQ;
		}
		else return GTR;
	}
	else if (Ch == ';') { Ch = getCh(); return SEMICOLON; }
	else if (Ch == ',') { Ch = getCh(); return COMMA; }
	else if (Ch == '.') { Ch = getCh(); return PERIOD; }
	else if (Ch == '%') { Ch = getCh(); return PERCENT; }
	else if (Ch == '[') { Ch = getCh(); return LBRACK; }
	else if (Ch == ']') { Ch = getCh(); return RBRACK; }
	else if (Ch == '(') { Ch = getCh(); return LPARENT; }
	else if (Ch == ')') { Ch = getCh(); return RPARENT; }
	else return NONE;
}

