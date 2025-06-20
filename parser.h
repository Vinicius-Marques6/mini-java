#pragma once

#include "scanner.h"
#include <iomanip>
#include <set>

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;
        SymbolTable* globalST;
        SymbolTable* currentST;
        bool hadError = false;

        void advance();
        void match(int);
        void initSymbolTable();
        void synch(set<int>);
        
    public:
        Parser(string);
        void run();
        void program();
        void mainClass();
        void classDeclaration();
        void varDeclaration();
        void methodDeclaration();
        void type();
        void statement();
        void expr();
        void exprLinha();
        void exprList();
        void error(string);
        void warn(string);
};