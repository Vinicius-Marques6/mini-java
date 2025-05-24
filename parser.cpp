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

void
Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
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
		error("Erro inesperado" + lToken->lexeme);
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
			error("Erro inesperado");
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
		error("Erro inesperado" + lToken->lexeme);
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
			// TODO: (ExpressionsList)?
			match(RPAREN);
		}
		else
		{
			error("Erro inesperado" + lToken->lexeme);
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
	if (lToken->name == CLASS)
	{
		advance();
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
	else
		error("Esperado 'class' no início do programa");
}

void
Parser::classDeclaration()
{
	advance();
	match(ID);
	if (lToken->name == EXTENDS)
	{
		advance();
		match(ID);
	}
	match(LBRACE);
	while (lToken->name != RBRACE)
	{
		//TODO: (VarDeclaration)* (MethodDeclaration)*
	}
}

void
Parser::error(string str)
{
	cout << "Linha " << scanner->getLine() << ": " << str << endl;

	exit(EXIT_FAILURE);
}
