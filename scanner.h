#pragma once

#include <fstream>
#include "symboltable.h"
#include <vector>
#include <iomanip>

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
    
    public:
    //Construtor
        Scanner(string, SymbolTable*);

        int getLine();
        int getColumn();
        string getFileName();
        string getLineInput(int lineNumber);
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
        //Método que avança para o próximo caractere
        void advance();
        //Método que avança para o próximo i caracteres
        void advance(int i);
    
        //Método para manipular erros
        void lexicalError(string);
};
