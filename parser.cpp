#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

std::string tokenToString(int t) {
	switch (t) {
		case END_OF_FILE: return "EOF";
		case LBRACE: return "{";
		case RBRACE: return "}";
		case IF: return "if";
		case ELSE: return "else";
		case WHILE: return "while";
		case SYSTEM_OUT_PRINTLN: return "System.out.println";
		case ID: return "id";
		case INTEGER: return "int";
		case TRUE: return "true";
		case FALSE: return "false";
		case THIS: return "this";
		case NEW: return "new";
		case NOT: return "!";
		case LPAREN: return "(";
		case RPAREN: return ")";
		case LBRACKET: return "[";
		case RBRACKET: return "]";
		case ASSIGN: return "=";
		case SEMICOLON: return ";";
		case DOT: return ".";
		case LENGTH: return "length";
		case PLUS: return "+";
		case MINUS: return "-";
		case MULTIPLY: return "*";
		case DIVIDE: return "/";
		case AND: return "&&";
		case LT: return "<";
		case GT: return ">";
		case EQUAL: return "==";
		case NOT_EQUAL: return "!=";
		case CLASS: return "class";
		case PUBLIC: return "public";
		case STATIC: return "static";
		case VOID: return "void";
		case MAIN: return "main";
		case STRING: return "String";
		case EXTENDS: return "extends";
		case INT: return "int";
		case BOOLEAN: return "boolean";
		case RETURN: return "return";
		case COMMA: return ",";
		default: return "undef";
	}
}

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Esperava '" + tokenToString(t) + "', encontrado '" + lToken->lexeme + "'");
}

void
Parser::run()
{
	advance();	

	program();
	
	cout << "Compilação encerrada com sucesso!\n";
}

void
Parser::program()
{
    mainClass();

	while (lToken->name != END_OF_FILE)
	{
		classDeclaration();
	}
}

//Continuar....
void
Parser::statement()
{
	if (lToken->name == LBRACE)
	{
		advance();
		while (lToken->name != RBRACE)
        {
            statement();
        }
		match(RBRACE);
	}
	else if (lToken->name == IF)
	{
		advance();
		match(LPAREN);
		expr();
		match(RPAREN);
		statement();
		match(ELSE);
		statement();
	}
	else if (lToken->name == WHILE)
	{
		advance();
		match(LPAREN);
		expr();
		match(RPAREN);
		statement();
	}
	else if (lToken->name == SYSTEM_OUT_PRINTLN)
	{
		advance();
		match(LPAREN);
		expr();
		match(RPAREN);
		match(SEMICOLON);
	}
	else if (lToken->name == ID)
	{
		advance();
		if (lToken->name == LBRACKET)
		{
			advance();
			expr();
			match(RBRACKET);
		}
		match(ASSIGN);
		expr();
		match(SEMICOLON);
	}
	else
		error("Esperava uma instrução, encontrado '" + lToken->lexeme + "'");
}

void
Parser::expr()
{
	if (lToken->name == INTEGER)
	{
		advance();
		exprLinha();
	}
	else if (lToken->name == TRUE)
	{
		advance();
		exprLinha();
	}
	else if (lToken->name == FALSE)
	{
		advance();
		exprLinha();
	}
	else if (lToken->name == ID)
	{
		advance();
		exprLinha();
	}
	else if (lToken->name == THIS)
	{
		advance();
		exprLinha();
	}
	else if (lToken->name == NEW)
	{
		advance();
		if (lToken->name == INT)
		{
			advance();
			match(LBRACKET);
			expr();
			match(RBRACKET);
		}
		else if (lToken->name == ID)
		{
			advance();
			match(LPAREN);
			match(RPAREN);
		}
		else
		{
			error("Esperava 'int' ou 'ID' após 'new', encontrado '" + lToken->lexeme + "'");
		}
		exprLinha();
	}
	else if (lToken->name == NOT)
	{
		advance();
		expr();
		exprLinha();
	}
	else if (lToken->name == LPAREN)
	{
		advance();
		expr();
		match(RPAREN);
		exprLinha();
	}
	else
	{
		error("Esperava uma expressão, encontrado '" + lToken->lexeme + "'");
	}
}

void
Parser::exprLinha()
{
	if (lToken->name == DOT)
	{
		advance();
		if (lToken->name == LENGTH)
		{
			advance();
		}
		else if (lToken->name == ID)
		{
			advance();
			match(LPAREN);
			if (lToken->name == INTEGER || lToken->name == TRUE || lToken->name == FALSE || lToken->name == ID || lToken->name == THIS || lToken->name == NEW || lToken->name == NOT || lToken->name == LPAREN)
			{
				exprList();
			}
			match(RPAREN);
		}
		else
		{
			error("Esperava 'length' ou 'ID' após '.', encontrado '" + lToken->lexeme + "'");
		}
		exprLinha();
	}
	else if (lToken->name == PLUS || lToken->name == MINUS || lToken->name == MULTIPLY || lToken->name == DIVIDE || lToken->name == AND || lToken->name == LT || lToken->name == GT || lToken->name == EQUAL || lToken->name == NOT_EQUAL)
	{
		advance();
		expr();
		exprLinha();
	}
	else if (lToken->name == LBRACKET)
	{
		advance();
		expr();
		match(RBRACKET);
		exprLinha();
	}
}

void
Parser::mainClass()
{
	match(CLASS);
	match(ID);
	match(LBRACE);
	match(PUBLIC);
	match(STATIC);
	match(VOID);
	match(MAIN);
	match(LPAREN);
	match(STRING);
	match(LBRACKET);
	match(RBRACKET);
	match(ID);
	match(RPAREN);
	match(LBRACE);
	statement();
	match(RBRACE);
	match(RBRACE);
}

void
Parser::classDeclaration()
{
	match(CLASS);
	match(ID);
	if (lToken->name == EXTENDS)
	{
		advance();
		match(ID);
	}
	match(LBRACE);
	while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
    {
        varDeclaration();
    }
    while (lToken->name == PUBLIC)
    {
        methodDeclaration();
    }
    match(RBRACE);
}

void
Parser::varDeclaration()
{
	type();
	match(ID);
	match(SEMICOLON);
}

void
Parser::type()
{
	if (lToken->name == INT)
	{
		advance();
		if (lToken->name == LBRACKET)
		{
			advance();
			match(RBRACKET);
		}
	}
	else if (lToken->name == BOOLEAN)
	{
		advance();
	}
	else if (lToken->name == ID)
	{
		advance();
	}
	else
		error("Esperava 'int', 'boolean' ou 'ID', encontrado '" + lToken->lexeme + "'");
}

void
Parser::methodDeclaration()
{
	match(PUBLIC);
	type();
	match(ID);
	match(LPAREN);
	if (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
	{
		type();
		match(ID);
		while (lToken->name == COMMA)
		{
			advance();
			type();
			match(ID);
		}
	}
	match(RPAREN);
	match(LBRACE);
	while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
	{
		varDeclaration();
	}
	while (lToken->name != RBRACE)
	{
		statement();
	}
	match(RETURN);
	expr();
	match(SEMICOLON);
	match(RBRACE);
}

void
Parser::exprList()
{
	if (lToken->name == INTEGER || lToken->name == TRUE || lToken->name == FALSE || lToken->name == ID || lToken->name == THIS || lToken->name == NEW || lToken->name == NOT || lToken->name == LPAREN)
	{
		expr();
		while (lToken->name == COMMA)
		{
			advance();
			expr();
		}
	}
	else if (lToken->name != RPAREN)
	{
		error("Esperava uma expressão ou ')', encontrado '" + lToken->lexeme + "'");
	}
}

void
Parser::error(string str)
{
	const int line = scanner->getLine();
	const int column = scanner->getColumn();

	cout << scanner->getFileName() << ":" << line << ":" << column << ": erro" << ": " << str << endl;

    cout << setw(5) << line << " | " << scanner->getLineInput(line);
    cout << setw(5) << line + 1 << " | " << string(column - 1, ' ') << "^" << endl;

	exit(EXIT_FAILURE);
}
