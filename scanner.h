#pragma once

#include <fstream>
#include "symboltable.h"
#include <vector>
#include <iomanip>

#define FILE_NAME "\033[37m\033[1m"
#define ERROR "\033[31m\033[1m"
#define WARNING "\033[33m\033[1m"
#define LINE "\033[90m"
#define RESET "\033[0m"


class Scanner 
{
    private:
        vector<size_t> lineOffsets; // Armazena o início de cada linha
        string fileName; // Nome do arquivo
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
        int column;
        SymbolTable* st;
        bool hadError = false;
    
    public:
    //Construtor
        Scanner(string, SymbolTable*);

        int getLine();
        int getColumn();
        bool getHadError();
        string getFileName();
        string getLineInput(int lineNumber);
    
        //Método que retorna o próximo token da entrada sem avançar
        Token* peekToken();
        //Método que retorna o próximo token da entrada
        Token* nextToken();
        //Método que avança para o próximo caractere
        void advance();
        //Método que avança para o próximo i caracteres
        void advance(int i);
    
        //Método para manipular erros
        void lexicalError(string);
};
