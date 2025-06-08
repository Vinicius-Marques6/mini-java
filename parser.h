#pragma once

#include "scanner.h"
#include <iomanip>

class Parser
{
	private:
		Scanner* scanner;
		Token* lToken;

		void advance();
		void match(int);
		
	public:
		Parser(string);
		void run();
        void program();
        void mainClass();
        //Continuar....
		void statement();
		void expr();
		void exprLinha();
		void classDeclaration();
		void varDeclaration();
		void methodDeclaration();
		void type();
		void exprList();
        void error(string);
};