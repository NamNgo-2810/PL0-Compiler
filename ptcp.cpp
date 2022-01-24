#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pttv.h"

extern TokenType Token;
extern int lineNo, colNo;
extern FILE *f;
extern char Ch;

void error(int errorCode), 
	declareConst(void),
	declareVariable(void),
	declareProduce(void),
	factor(void), 
	term(void), 
	expression(void), 
	condition(void), 
	statement(void), 
	block(void), 
	program(void);

void compile(char* filename) {
	f = fopen(filename, "rt");
	lineNo = 1;
	colNo = 0;
	Ch = ' ';
	Token = getToken();
	program();
	fclose(f);
}

void declareConst() {
	if (Token == IDENT) {
		Token = getToken();
		if (Token == EQU) {
			Token = getToken();
			if (Token == NUMBER) {
				Token = getToken();
			}
			else error(17);
		}
		else error(16);
	}
	else error(15);
}

void declareVariable() {
	if (Token == IDENT) {
		Token = getToken();
		if (Token == LBRACK) {
			Token = getToken();
			if (Token == NUMBER) {
				Token = getToken();
				if (Token == RBRACK) {
					Token = getToken();
				}
				else error(21);
			}
			else error(20);
		}
	}
	else error(19);
}

void declareProcedure() {
	Token = getToken();
	if (Token == IDENT) {
		Token = getToken();
		if (Token == LPARENT) {
			Token = getToken();
			if (Token == IDENT || Token == VAR) {
				Token = getToken();
			}
			else error(24); 
		}
		if (Token == RPARENT) {
			Token = getToken();
		}
		else error(25);
		
		if (Token == SEMICOLON) {
			Token = getToken();
			block();
			if (Token == SEMICOLON) {
				Token = getToken();
			}
			else error(26);
		}
		else error(27);	
	}
	else error(23);
}

void factor() {
	if (Token == IDENT) {
		Token = getToken();
		if (Token == LBRACK) {
			Token = getToken();
			expression();
			Token = getToken();
			if (Token == RBRACK) {
				Token = getToken();
			}
			else error(46);
		}
	}
	else if (Token == NUMBER) {
		Token = getToken();
	}
	else if (Token == LPARENT) {
		Token = getToken();
		expression();
		if (Token == RPARENT) {
			Token = getToken();
		}
		else error(47);
	}
	else error(48);
}

void term() {
	factor();
	while (Token == TIMES || Token == SLASH || Token == PERCENT) {
		Token = getToken();
		factor();
	}
}

void expression() {
	if (Token == PLUS || Token == MINUS) {
		Token = getToken();
	}
	term();
	while (Token == PLUS || Token == MINUS) {
		Token = getToken();
		term();
	}
}

void condition() {
	if (Token == ODD) {
		Token = getToken();
		expression();
	}
	else {
		expression();
		if (Token == EQU || Token == NEQ || Token == LSS ||
			Token == LEQ || Token == GTR || Token == GEQ) {
			Token = getToken();
			expression();		
		}
		else error(45);	
	}
	
}

void statement() {
	if (Token == IDENT) {
		Token = getToken();
		if (Token == LBRACK) {
			Token = getToken();
			expression();
			if (Token == RBRACK) {
				Token = getToken();
			} 
			else error(31);
		}
		if (Token == ASSIGN) {
			Token = getToken();
			expression();
		}
		else error(32);
	}
	else if (Token == CALL) {
		Token = getToken();
		if (Token == IDENT) {
			Token = getToken();
			if (Token == LPARENT) {
				Token = getToken();
				do {
					if (Token == COMMA) {
						Token = getToken();
					}
					expression();
					Token = getToken();
				} while (Token == COMMA);
				Token = getToken();
				if (Token != RPARENT) {
					error(34);
				}
			}
		}
		else error(33);
	}
	else if (Token == BEGIN) {
		Token = getToken();
		statement();
		while (Token == SEMICOLON) {
			Token = getToken();
			statement();
		} 
		if (Token == END) {
			Token = getToken();
		}
		else error(35);
	}
	else if (Token == IF) {
		Token = getToken();
		condition();
		
		if (Token == THEN) {
			Token = getToken();
			statement();
			Token = getToken();
			if (Token == ELSE) {
				Token = getToken();
				statement();
			}
		}
		else error(36);
	}
	else if (Token == WHILE) {
		Token = getToken();
		condition();
		if (Token == DO) {
			Token = getToken();
			statement();
		}
		else error(37);
	}
	else if (Token == FOR) {
		Token = getToken();
		if (Token == IDENT) {
			Token = getToken();
			if (Token == ASSIGN) {
				Token = getToken();
				expression();
				if (Token == TO) {
					Token = getToken();
					expression();
					if (Token == DO) {
						Token = getToken();
						statement();
					}
					else error(41);
				}
				else error(40);
			}
			else error(39);
		}
		else error(38);
	}
}

void block() {
	if (Token == CONST) {
		Token = getToken();
		declareConst();
		while (Token == COMMA) {
			Token = getToken();
			declareConst();
		}
		if (Token == SEMICOLON) {
			Token = getToken();
			block();
		}
		else error(18);
	}	
	else if (Token == VAR) {
		Token = getToken();
		declareVariable();
		while (Token == COMMA) {
			Token = getToken();
			declareVariable();
		}
		if (Token == SEMICOLON) {
			Token = getToken();
			block();
		}
		else error(22);
	}
	else if (Token == PROCEDURE) {
		declareProcedure();
	}
	else if (Token == BEGIN) {
		Token = getToken();
		statement();
		while (Token == SEMICOLON) {
			Token = getToken();
			statement();
		 } 
		if (Token == END) {
			Token = getToken();
		}
		else error(35);
	}
}

void program() {
	if (Token == PROGRAM) {
		Token = getToken();
		if (Token == IDENT) {
			Token = getToken();
			if (Token == SEMICOLON) {
				Token = getToken();
				block();
				if (Token == PERIOD) {
					Token = getToken();
				}
				else error(14);
			}
			else error(13);
		}
		else error(12);
	}
	else error(11);
}

void error(int errorCode) {
	char Msg[80];
	switch (errorCode) {
		case 11: strcpy(Msg, "\"PROGRAM\" keyword expected"); break;
		case 12: strcpy(Msg, "PROGRAM identifier expected"); break;
		case 13: strcpy(Msg, "Declare PROGRAM: \";\" expected"); break;
		case 14: strcpy(Msg, "\".\" expected"); break;
		case 15: strcpy(Msg, "Declare CONSTANT: Identifier expected"); break;
		case 16: strcpy(Msg, "Declare CONSTANT: \"=\" expected"); break;
		case 17: strcpy(Msg, "Declare CONSTANT: Number expected"); break;
		case 18: strcpy(Msg, "Declare CONSTANT: \";\" expected"); break;
		case 19: strcpy(Msg, "Declare VARIABLE: Identifier expected"); break;
		case 20: strcpy(Msg, "Declare VARIABLE: Number expected"); break;
		case 21: strcpy(Msg, "Declare VARIABLE: \"]\" expected"); break;
		case 22: strcpy(Msg, "Declare VARIABLE: \";\" expected"); break;	
		case 23: strcpy(Msg, "Declare PROCEDURE: Identifier expected"); break;
		case 24: strcpy(Msg, "Declare PROCEDURE: Formal parameter expected"); break;
		case 25: strcpy(Msg, "Declare PROCEDURE: \")\" expected"); break;	
		case 26: strcpy(Msg, "Declare PROCEDURE: \";\" expected"); break;
		case 27: strcpy(Msg, "Declare PROCEDURE: \";\" after procedure name expected"); break;
		case 28: strcpy(Msg, ""); break;	
		case 29: strcpy(Msg, ""); break;
		case 30: strcpy(Msg, ""); break;
		case 31: strcpy(Msg, "Declare EXPRESSION: \"]\" expected"); break;	
		case 32: strcpy(Msg, "Declare EXPRESSION: \":=\" expected"); break;
		case 33: strcpy(Msg, "Call PROCEDURE: Identifier expected"); break;
		case 34: strcpy(Msg, "Call PROCEDURE: \")\" expected"); break;	
		case 35: strcpy(Msg, "Complex statement: \"END\" keyword expected"); break;
		case 36: strcpy(Msg, "IF statement: \"THEN expected\""); break;
		case 37: strcpy(Msg, "WHILE statement: \"DO\" expected"); break;	
		case 38: strcpy(Msg, "FOR statement: Variable identifier expected"); break;
		case 39: strcpy(Msg, "FOR statement: \":=\" expected"); break;
		case 40: strcpy(Msg, "FOR statement: \"TO\" keyword expected"); break;	
		case 41: strcpy(Msg, "FOR statement: \"DO\" keyword expected"); break;
		case 42: strcpy(Msg, ""); break;
		case 43: strcpy(Msg, ""); break;	
		case 44: strcpy(Msg, ""); break;
		case 45: strcpy(Msg, "Condition: Relation operator expected"); break;
		case 46: strcpy(Msg, "Array variable: \"]\" expected"); break;	
		case 47: strcpy(Msg, "\")\" expected"); break;
		case 48: strcpy(Msg, "Error in expression"); break;
	}
	
	printf("\n\nError %d: %s at row %d, col %d\n", errorCode, Msg, lineNo, colNo);
	exit(1);
}

